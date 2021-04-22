/******************************************************************************

********************************************************************************/

#include "project.h"
#include "GUI.h"
#include "pervasive_eink_hardware_driver.h"
#include "cy_eink_library.h"
#include "LCDConf.h"
#include <stdlib.h>
#include "algorithme.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"     
#include "semphr.h"
#include "display_task.h"
#include "touch_task.h"
#include "MAX30102.h"
#include "variables.h"
#include "bmi160_task.h"
#include "bmi160.h"
#include "bmi160_defs.h"


/* Priorities of user tasks in this project */
#define TASK_TOUCH_PRIORITY         (10u)
#define TASK_DISPLAY_PRIORITY       (5u)
#define TASK_BOUTON_PRIORITY        (4u)
#define TASK_SAMPLE_PRIORITY        (4u)
#define TASK_FILTER_PRIORITY        (4u)
#define TASK_RESULT_PROIRITY        (4u)


/* Stack sizes of user tasks in this project */
#define DISPLAY_TASK_STACK_SIZE     (1024u)
#define TOUCH_TASK_STACK_SIZE       (configMINIMAL_STACK_SIZE)
#define BOUTON_TASK_STACK_SIZE      (configMINIMAL_STACK_SIZE)


/* Variables globales*/
volatile SemaphoreHandle_t bouton_semph;
volatile SemaphoreHandle_t active_task;


/*************************************************************************/
/*!
 * @ Résumé Cette fonction permet d'écrire une valeur 
 * dans un registre du BMI160.
 *
 * @param[in] dev_adresse  : adresse fixe du capteur
 * @param[in] reg_adresse  : adresse du registre dans lequel il faut écrire
 * @param[in] data         : tableau où les données contenant les valeurs à écrire
 * @param[in] length       : nombre de bits à écrire
 * 
 *
 * @return 0
 * 
 */

void isr_bouton(void){
    
    xSemaphoreGiveFromISR(bouton_semph, NULL);
    CyDelay(1000);      // more or less ??
    Cy_GPIO_ClearInterrupt(Bouton_0_PORT, Bouton_0_NUM);
    NVIC_ClearPendingIRQ(Bouton_ISR_cfg.intrSrc);

}

/*!
 * @ Résumé Cette fonction permet d'écrire une valeur 
 * dans un registre du BMI160.
 *
 * @param[in] dev_adresse  : adresse fixe du capteur
 * @param[in] reg_adresse  : adresse du registre dans lequel il faut écrire
 * @param[in] data         : tableau où les données contenant les valeurs à écrire
 * @param[in] length       : nombre de bits à écrire
 * 
 *
 * @return 0
 * 
 */

void Task_Bouton2(void *arg){
    
    (void) arg;
    
    touch_data_t currentTouch = NO_TOUCH;
    
    for(;;){
        
        if(xSemaphoreTake(bouton_semph, portMAX_DELAY) == pdTRUE){
            
            currentTouch =  BUTTON2_TOUCHED;
            xQueueSend(touchDataQ, &currentTouch, ( TickType_t ) 0);
            currentTouch = NO_TOUCH;
            vTaskDelay(pdMS_TO_TICKS(100));
            
            //vTaskDelay(pdMS_TO_TICKS(20));
        }

    }
}

/*!
 * @ Résumé Cette tâche permet l'acquisition des données
 * et la vérification de l'interruption.
 *
 * @param[in] : none
 *
 * @return none
 * 
 */
void vSample_task(void *arg){
    
    (void) arg;
    for (;;){
           
        for(indexBuffer=0; indexBuffer<BUFFER_LENGTH; indexBuffer++)
        {
            get_accData ();                             // Regarde s'il y a du mouvement et si oui active l'interruption
            readFIFO(red, ir, indexBuffer);             // Procède à la lecture des données
            vTaskDelay(pdMS_TO_TICKS(3));
               
        }
        if(indexBuffer == BUFFER_LENGTH){
            indexBuffer = 0;                            // Lorsque le buffer est plein, on reprend depuis le début
            vTaskResume(xFiltering);
            vTaskSuspend(xSample); 
        }
    
    }

}

/*!
 * @ Résumé Cette tâche permet de filtre les signaux acquis.
 *
 * @param[in] : none
 *
 * @return none
 * 
 */

void vFiltering_task(void *arg){
    (void) arg;
    for(;;){
 
        filtre(red, filteredRED);
        filtre(ir, filteredIR);
        
        vTaskResume(xResults);
        vTaskSuspend(xFiltering);
    }
    
}

/*!
 * @ Résumé Cette tâche calcule le BPM et le SpO2 
 *
 * @param[in] : none
 * 
 * @return none
 * 
 */

void vResults(void *arg){
    (void) arg;
    for(;;){
    
        if(indexBuffer == 0){
            SPO2 = calculSpO2(red, ir, 0, BUFFER_LENGTH);
            BPM = HeartRate(filteredIR, 0, BUFFER_LENGTH);
        }
        
        vTaskSuspend(xResults);
    }
       
}

/***************************************************************/

int main(void)
{
    bouton_semph = xSemaphoreCreateBinary();
    
    __enable_irq(); /* Enable global interrupts. */
    
    DisplayInit();
    I2C_MAX_Start();
    MAX30102_config();
    UART_1_Start();
    Task_Motion();

    touchDataQ = xQueueCreate(10, sizeof(touch_data_t));
    
    Cy_SysInt_Init(&Bouton_ISR_cfg, isr_bouton);
    NVIC_ClearPendingIRQ(Bouton_ISR_cfg.intrSrc);
    NVIC_EnableIRQ(Bouton_ISR_cfg.intrSrc);
    
         
    /* Create the user Tasks. See the respective Task definition for more
       details of these tasks */       
    
    xTaskCreate(Task_Touch, "Touch Task", TOUCH_TASK_STACK_SIZE, NULL, TASK_TOUCH_PRIORITY, NULL);

    xTaskCreate(Task_AffichageGraphique, "Task A", DISPLAY_TASK_STACK_SIZE, NULL, TASK_DISPLAY_PRIORITY, NULL);
    
    xTaskCreate(Task_Bouton2, "Task Bouton 2", BOUTON_TASK_STACK_SIZE, NULL, TASK_BOUTON_PRIORITY, NULL);
    
    xTaskCreate(vSample_task, "Acquisition",1024,NULL, TASK_SAMPLE_PRIORITY,&xSample);
    
    xTaskCreate(vFiltering_task, "Filtrage", 1024, NULL, TASK_FILTER_PRIORITY, &xFiltering);
    
    xTaskCreate(vResults, "Resultat", 1024, NULL, TASK_RESULT_PROIRITY, &xResults);
    
    vTaskSuspend(xFiltering);
    vTaskSuspend(xResults);
    
    /* Start the RTOS scheduler. This function should never return */
    vTaskStartScheduler();
    

    /* Halt the CPU if scheduler exits */
    CY_ASSERT(0);

}

/* [] END OF FILE */
