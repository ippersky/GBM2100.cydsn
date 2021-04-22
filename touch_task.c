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





// initialisation de la queue pour envoyer information sur le bouton appuyé
QueueHandle_t touchDataQ;




/*******************************************************************************
Fonction : void Task_Touch(void * arg) 
Sommaire : Fonction de la tâche du touch CapSense. Vérifie s'il y a un bouton 
touché et écrit l'information du bouton touché dans une queue. 
Parametres : void *data  (pas utilisé dans la tâche)
Return: None
*******************************************************************************/
void Task_Touch(void * arg)          
{

    CapSense_Start();

    CapSense_ScanAllWidgets();  
    
    touch_data_t currentTouch;  
    
    static bool oldBTN0=false;
    static bool oldBTN1=false;
    
    for(;;)
    {
        if(!CapSense_IsBusy()){       
            CapSense_ProcessAllWidgets();
            
            currentTouch = NO_TOUCH;

            if(CapSense_IsWidgetActive(CapSense_BUTTON0_WDGT_ID)){ // si le bouton 0 est touché 
                if(!oldBTN0)
                    currentTouch = BUTTON0_TOUCHED;                
                oldBTN0 = true; // empêcher que le bouton 0 soit lu 2 fois de suite
            }
            else 
                oldBTN0 = false; // arrivera si aucun bouton est touché ou si bouton 1 est touché
            
            
            if(CapSense_IsWidgetActive(CapSense_BUTTON1_WDGT_ID)){  // si le bouton 1 est touché 
                if(!oldBTN1)
                    currentTouch = BUTTON1_TOUCHED; 
                oldBTN1 = true; // empêcher que le bouton 1 soit lu 2 fois de suite
            }
            else
                oldBTN1 = false;  // arrivera si aucun bouton est touché ou si bouton 0 est touché
            
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
