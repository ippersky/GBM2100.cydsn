/******************************************************************************

********************************************************************************/

#include "project.h"
#include "GUI.h"
#include "pervasive_eink_hardware_driver.h"
#include "cy_eink_library.h"
#include "LCDConf.h"
#include <stdlib.h>
#include "algorithme.h"

//#include <stdio.h>  //?
//#include <unistd.h> //?

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"     
#include "semphr.h"




#include "display_task.h"
#include "touch_task.h"
#include "oxy_task.h"
#include "MAX30102.h"
//#include "sample_task.h"


/* Priorities of user tasks in this project */
#define TASK_TOUCH_PRIORITY         (10u)
#define TASK_DISPLAY_PRIORITY       (5u)
#define TASK_BOUTON_PRIORITY        (4u)


/* Stack sizes of user tasks in this project */
#define DISPLAY_TASK_STACK_SIZE     (1024u)//1024 
#define TOUCH_TASK_STACK_SIZE       (configMINIMAL_STACK_SIZE)
#define BOUTON_TASK_STACK_SIZE      (configMINIMAL_STACK_SIZE)




volatile SemaphoreHandle_t bouton_semph;

// Image buffer cache //
//uint8 imageBufferCache[CY_EINK_FRAME_SIZE] = {0};



/*************************************************************************/
void isr_bouton(void){
    //touch_data_t currentTouch = BUTTON2_TOUCHED;
    //xQueueSendFromISR(touchDataQ, &currentTouch, NULL);


    
    xSemaphoreGiveFromISR(bouton_semph, NULL);
    CyDelay(1000);      // more or less ??
    Cy_GPIO_ClearInterrupt(Bouton_0_PORT, Bouton_0_NUM);
    NVIC_ClearPendingIRQ(Bouton_ISR_cfg.intrSrc);

}


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

uint32_t red[BUFFER_LENGTH];
uint32_t ir[BUFFER_LENGTH];
uint16_t indexBuffer = 0;

/*************************************************************************/
void vTask_sample(){
    
    
    
    for(indexBuffer=0; indexBuffer<BUFFER_LENGTH; indexBuffer++)
    {
        
        readFIFO(red, ir, indexBuffer);
        
        uint16_t i = 0;
        for(i=0; i<2000; i++){
            char cRed[6];
            itoa(red[i], cRed, 2);
            UART_1_PutString(cRed);
        }
    }
    
    vTaskDelay(pdMS_TO_TICKS(100));
}

void vTraitement(){
    
    float spo2; 
    float bpm;
    if(indexBuffer == 0){
        spo2 = calculSpO2(red, ir, 0, BUFFER_LENGTH);
        bpm = HeartRate(red, 0, BUFFER_LENGTH);
        char sSpo2[5];
        itoa(spo2, sSpo2, 10);
        UART_1_PutString(sSpo2);
    }
    else{
        spo2 = calculSpO2(red, ir, BUFFER_LENGTH/2, BUFFER_LENGTH);
        bpm = HeartRate(red, BUFFER_LENGTH/2, BUFFER_LENGTH);
        char sSpo2[5];
        itoa(spo2, sSpo2, 10);
        UART_1_PutString(sSpo2);
    }
}




int main(void)
{
    bouton_semph = xSemaphoreCreateBinary();
    
    
    __enable_irq(); /* Enable global interrupts. */
    
    DisplayInit();
    I2C_MAX_Start();
    MAX30102_config();
    UART_1_Start();

    
    /* Initialisation de CapSense */
    
    //CapSense_Start();
    //CapSense_ScanAllWidgets();
    
    /* Initialisation des tasks */
        /* Create the queues. See the respective data-types for details of queue
       contents */
    
    touchDataQ = xQueueCreate(10, sizeof(touch_data_t));
    
        
    Cy_SysInt_Init(&Bouton_ISR_cfg, isr_bouton);
    NVIC_ClearPendingIRQ(Bouton_ISR_cfg.intrSrc);
    NVIC_EnableIRQ(Bouton_ISR_cfg.intrSrc);
    
         
    /* Create the user Tasks. See the respective Task definition for more
       details of these tasks */       
    //xTaskCreate(Task_Touch, "Touch Task", TOUCH_TASK_STACK_SIZE, NULL, TASK_TOUCH_PRIORITY, NULL);

    //xTaskCreate(Task_AffichageGraphique, "Task A", DISPLAY_TASK_STACK_SIZE, NULL, TASK_DISPLAY_PRIORITY, NULL);
    
    //xTaskCreate(Task_Bouton2, "Task Bouton 2", BOUTON_TASK_STACK_SIZE, NULL, TASK_BOUTON_PRIORITY, NULL);
    
    //xTaskCreate(vtraitement,"Traitement du signal",5000,NULL, TASK_DISPLAY_PRIORITY,NULL); //ERREUR ICI
    
    xTaskCreate(vTask_sample, "Acquisition",5000,NULL, TASK_DISPLAY_PRIORITY,NULL);
    
    //xTaskCreate(vTraitement, "Traitement", 1024, NULL, TASK_DISPLAY_PRIORITY, NULL);
    
    /* Initialize thread-safe debug message printing. See uart_debug.h header file
       to enable / disable this feature */
    //DebugPrintfInit();
    
    /* Start the RTOS scheduler. This function should never return */
    vTaskStartScheduler();
    
    /* Should never get here! */ 
    //DebugPrintf("Error!   : RTOS - scheduler crashed \r\n");

    /* Halt the CPU if scheduler exits */
    CY_ASSERT(0);
    
    
    
    
    while(CapSense_IsBusy()); // ?? du CapSense Example
    for(;;)
    {
        
    }
}

/* [] END OF FILE */
