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





void Task_Touch(void * arg)      // void ou void * arg ??        
{
    //cy_status capSenseApiResult;
    
    //capSenseApiResult = CapSense_Start();
    CapSense_Start();
    /*
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
    
    */
    CapSense_ScanAllWidgets();  // pas dans le code exemple 
    
    touch_data_t currentTouch;  // à initialiser ailleurs? car interupt bouton 
                                // a besoin de ça aussi...?
    static bool oldBTN0=false;
    static bool oldBTN1=false;
    
    for(;;)
    {
        if(!CapSense_IsBusy()){       ////// du CapSense test 1
            CapSense_ProcessAllWidgets();
            
            currentTouch = NO_TOUCH;

            if(CapSense_IsWidgetActive(CapSense_BUTTON0_WDGT_ID)){
                if(!oldBTN0)
                    currentTouch = BUTTON0_TOUCHED;                
                oldBTN0 = true;
            }
            else 
                oldBTN0 = false;
            
            
            if(CapSense_IsWidgetActive(CapSense_BUTTON1_WDGT_ID)){
                if(!oldBTN1)
                    currentTouch = BUTTON1_TOUCHED; 
                oldBTN1 = true;
            }
            else
                oldBTN1 = false;
            
            if (currentTouch != NO_TOUCH){
                xQueueSend( touchDataQ, &currentTouch, ( TickType_t ) 0 );            
            }
            
            CapSense_UpdateAllBaselines();
            CapSense_ScanAllWidgets();
            vTaskDelay(pdMS_TO_TICKS(100));
        }
    }
}
  




/* [] END OF FILE */
