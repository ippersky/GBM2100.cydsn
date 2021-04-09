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


QueueHandle_t touchDataQ;



//volatile uint16_t bouton0=0;    // est-ce qu'on peut mettre variable globale dans un fichier .c ??
//volatile uint16_t bouton1=0;

void capSense_task(void * arg)      // void ou void * arg ??        
{
    cy_status capSenseApiResult;
    
    capSenseApiResult = CapSense_Start();
    //CapSense_Start();
    
    if (capSenseApiResult== CY_RET_SUCCESS)
    {
        GUI_Clear();
        GUI_SetFont(GUI_FONT_20_1);
        GUI_DispStringAt("CapSense init reussi", 10, 10);
        UpdateDisplay(CY_EINK_FULL_4STAGE, true);
    }
    else
    {
        GUI_Clear();
        GUI_SetFont(GUI_FONT_20_1);
        GUI_DispStringAt("CapSense init pas reussi", 10, 10);
        UpdateDisplay(CY_EINK_FULL_4STAGE, true);
    }
    
    
    CapSense_ScanAllWidgets();  // pas dans le code exemple 
    
    for(;;)
    {
        if(!CapSense_IsBusy()){       ////// du CapSense test 1
            CapSense_ProcessAllWidgets();
       

            if(CapSense_IsWidgetActive(CapSense_BUTTON0_WDGT_ID)){
                //bouton0++; //mettre dans une queue ou declencher un semaphore
                // xQueueSend(1); 
            }
            if(CapSense_IsWidgetActive(CapSense_BUTTON1_WDGT_ID)){
                //bouton1++; //mettre dans une queue ou declencher un semaphore
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
