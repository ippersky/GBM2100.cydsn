/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#include "touch_task.h"

////////////////// du CapSense Example //////////////////////

#include "project.h"
#include "project.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"   




volatile uint16_t bouton0=0;    // est-ce qu'on peut mettre variable globale dans un fichier .c ??
volatile uint16_t bouton1=0;

void capSense_task(void * arg)      // void ou void * arg ??        
{
    for(;;)
    {
        if(!CapSense_IsBusy){       ////// du CapSense test 1
            CapSense_ProcessAllWidgets();
       

            if(CapSense_IsWidgetActive(CapSense_BUTTON0_WDGT_ID)){
                bouton0++; //mettre dans une queue ou declencher un semaphore
                // xQueueSend(1); 
            }
            if(CapSense_IsWidgetActive(CapSense_BUTTON1_WDGT_ID)){
                bouton1++; //mettre dans une queue ou declencher un semaphore
                // xQueueSend(2); 
            }
            CapSense_UpdateAllBaselines();
            CapSense_ScanAllWidgets();
            vTaskDelay(pdMS_TO_TICKS(100));
        }
    }
}


// Initialisation du task dans le main


/* [] END OF FILE */
