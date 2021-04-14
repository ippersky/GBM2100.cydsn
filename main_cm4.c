/******************************************************************************
* File Name: main_cm4.c
*
* Version: 1.2
*
* Description: This file main application code for the CE223727 EmWin Graphics
*				library EInk Display 

********************************************************************************
*************MODIFIED by Christophe Cloutier-Tremblay.**************************
********************************************************************************
* 
*
* Hardware Dependency: CY8CKIT-028-EPD E-Ink Display Shield
*					   CY8CKIT-062-BLE PSoC6 BLE Pioneer Kit
*
******************************************************************************* 
* Copyright (2019), Cypress Semiconductor Corporation. All rights reserved. 
******************************************************************************* 
* This software, including source code, documentation and related materials 
* (“Software”), is owned by Cypress Semiconductor Corporation or one of its 
* subsidiaries (“Cypress”) and is protected by and subject to worldwide patent 
* protection (United States and foreign), United States copyright laws and 
* international treaty provisions. Therefore, you may use this Software only 
* as provided in the license agreement accompanying the software package from 
* which you obtained this Software (“EULA”). 
* 
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive, 
* non-transferable license to copy, modify, and compile the Software source 
* code solely for use in connection with Cypress’s integrated circuit products. 
* Any reproduction, modification, translation, compilation, or representation 
* of this Software except as specified above is prohibited without the express 
* written permission of Cypress. 
* 
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND, 
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED 
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress 
* reserves the right to make changes to the Software without notice. Cypress 
* does not assume any liability arising out of the application or use of the 
* Software or any product or circuit described in the Software. Cypress does 
* not authorize its products for use in any products where a malfunction or 
* failure of the Cypress product may reasonably be expected to result in 
* significant property damage, injury or death (“High Risk Product”). By 
* including Cypress’s product in a High Risk Product, the manufacturer of such 
* system or application assumes all risk of such use and in doing so agrees to 
* indemnify Cypress against all liability.
********************************************************************************/

#include "project.h"
#include "GUI.h"
#include "pervasive_eink_hardware_driver.h"
#include "cy_eink_library.h"
#include "LCDConf.h"
#include <stdlib.h>

//#include <stdio.h>  //?
//#include <unistd.h> //?

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"     
#include "semphr.h"
#include "params.h"



#include "display_task.h"
#include "touch_task.h"



/* Priorities of user tasks in this project */
#define TASK_TOUCH_PRIORITY         (10u)
#define TASK_DISPLAY_PRIORITY       (5u)


/* Stack sizes of user tasks in this project */
#define DISPLAY_TASK_STACK_SIZE     (1024u)
#define TOUCH_TASK_STACK_SIZE       (configMINIMAL_STACK_SIZE)




volatile SemaphoreHandle_t bouton_semph;






// Image buffer cache //
//uint8 imageBufferCache[CY_EINK_FRAME_SIZE] = {0};







/*******************************************************************************
* Function Name: void UpdateDisplay(void)
********************************************************************************
*
* Summary: This function updates the display with the data in the display 
*			buffer.  The function first transfers the content of the EmWin
*			display buffer to the primary EInk display buffer.  Then it calls
*			the Cy_EINK_ShowFrame function to update the display, and then
*			it copies the EmWin display buffer to the Eink display cache buffer
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  It takes about a second to refresh the display.  This is a blocking function
*  and only returns after the display refresh
*
*******************************************************************************/


/*******************************************************************************
* Function Name: void ClearScreen(void)
********************************************************************************
*
* Summary: This function clears the screen
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/



/*******************************************************************************
* Function Name: void WaitforSwitchPressAndRelease(void)
********************************************************************************
*
* Summary: This implements a simple "Wait for button press and release"
*			function.  It first waits for the button to be pressed and then
*			waits for the button to be released.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  This is a blocking function and exits only on a button press and release
*
*******************************************************************************/
/*
void WaitforSwitchPressAndRelease(void)
{
    // Wait for SW2 to be pressed 
    while(Status_SW2_Read() != 0);
    
    // Wait for SW2 to be released
    while(Status_SW2_Read() == 0);
}
*/

/*******************************************************************************
* Function Name: int main(void)
********************************************************************************
*
* Summary: À Remplir
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/



void isr_bouton(void){
    //touch_data_t currentTouch = BUTTON2_TOUCHED;
    //xQueueSendFromISR(touchDataQ, &currentTouch, NULL);


    
    xSemaphoreGiveFromISR(bouton_semph, NULL);
    CyDelay(1000);      // more or less ??
    Cy_GPIO_ClearInterrupt(Bouton_0_PORT, Bouton_0_NUM);
    NVIC_ClearPendingIRQ(Bouton_ISR_cfg.intrSrc);

}


void Task_Bouton2(void *arg){
    
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






int main(void)
{
    bouton_semph = xSemaphoreCreateBinary();
    
    
    __enable_irq(); /* Enable global interrupts. */
    
    DisplayInit();
    

    
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
    xTaskCreate(Task_Touch, "Touch Task", TOUCH_TASK_STACK_SIZE, NULL, TASK_TOUCH_PRIORITY, NULL);

    xTaskCreate(Task_AffichageGraphique, "Task A", DISPLAY_TASK_STACK_SIZE, NULL, TASK_DISPLAY_PRIORITY, NULL);
    
    xTaskCreate(Task_Bouton2, "Task Bouton 2", TOUCH_TASK_STACK_SIZE, NULL, 4u, NULL);
    
    /* Initialize thread-safe debug message printing. See uart_debug.h header file
       to enable / disable this feature */
    //DebugPrintfInit();
    
    /* Start the RTOS scheduler. This function should never return */
    vTaskStartScheduler();
    
    /* Should never get here! */ 
    //DebugPrintf("Error!   : RTOS - scheduler crashed \r\n");

    /* Halt the CPU if scheduler exits */
    CY_ASSERT(0);
    
    
    
    
    
    
    

    
    /*
    
    //Draw a rectangle
    GUI_DrawRect(1,1,263,175);
    
    //Draw a line
    GUI_SetPenSize(4);
    GUI_DrawLine(20,65,243,65);
    
    GUI_SetColor(GUI_BLACK);
    GUI_SetBkColor(GUI_WHITE);
    
   //Write a title and subtile	
    GUI_SetFont(GUI_FONT_32B_1);
    GUI_SetTextAlign(GUI_TA_CENTER);
    GUI_DispStringAt("GBM2100", 132, 30);
    GUI_SetFont(GUI_FONT_16_1);
    GUI_DispStringAt("Laboratoire 3", 132, 70);
    
    UpdateDisplay(CY_EINK_FULL_4STAGE, true);
    
    */
    while(CapSense_IsBusy()); // ?? du CapSense Example
    for(;;)
    {
        
    }
}

/* [] END OF FILE */
