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



#define DISPLAY_TASK_STACK_SIZE     (1024u)
#define TOUCH_TASK_STACK_SIZE       (configMINIMAL_STACK_SIZE)




// vecteur de 160 elements, LED rouge 
int32_t vecteurData[] = {205973,207305,206168,208001,206188,208050,206371,208225,206544,208577,206719,208501,206990,208473,207183,208417,207367,208379,207724,208151,207986,207993,208303,207802,208682,207622,208730,207587,209365,207455,209209,207541,209337,207549,209635,207806,209501,207957,209461,207996,209317,208103,209140,208491,209000,208672,208748,209006,208491,209357,208350,209326,208242,209947,208089,209826,208216,210028,208306,210395,208429,210203,208596,210292,208686,210169,208954,210055,209354,209987,209644,209821,210027,209660,210440,209483,210556,209554,211304,209484,211253,209696,211585,209817,211970,210011,211842,210178,211772,209972,211207,209606,210230,208954,209085,208043,207710,207263,206266,206439,205001,205392,203820,204970,202756,203955,202073,203512,201459,203377,201194,202759,200993,202713,201002,202459,201198,202302,201513,202188,201835,202108,202327,202081,202824,201965,203003,202176,203932,202267,203918,202500,204436,202777,204921,203032,204880,203312,205157,203520,205057,203810,205034,204221,204997,204527,204912,204994,204917,205416,204837,205671,204732,206338,204657,206251,204614,206460,204591,206662};
int16_t vecteurConverti[160] = {};  
int16_t vecteurAffichage[100] = {}; // a redefinir : taille = longueurData/espacement
size_t longueurData = sizeof(vecteurData)/sizeof(int32_t); // 160
size_t espacement = 3;
    

    // quand je mets en argument qqch pour le task, ca ne marche plus
    // l'ecran refresh, mais n'affiche rien

    // quand je mets les 2 vecteurs vides dans task, meme le refresh s'arrete

int SPO2 = 35; 
int BPM = 85;


void Task_AffichageGraphique(void *data){
   
    //int32_t * vecteurData = (int32_t *)data;
    DisplayInit();
    uint8_t optionMenuSecondaire = 0;
    int longueurAffichage = longueurData/espacement;

    
    for(;;){
        
    convertirVecteurEnInt16(vecteurConverti, vecteurData, longueurData);
    creerVecteurAffichage(vecteurConverti, vecteurAffichage, longueurData, espacement);
    GUI_DrawGraph(vecteurAffichage, longueurAffichage, 0, 0); 
    UpdateDisplay(CY_EINK_FULL_4STAGE, true);
    
    updateParametres(SPO2, BPM);
    
    afficherMenuSecondaire(&optionMenuSecondaire);
    updateMenuSecondaire(&optionMenuSecondaire);
    updateMenuSecondaire(&optionMenuSecondaire);
    updateMenuSecondaire(&optionMenuSecondaire);
    updateMenuSecondaire(&optionMenuSecondaire);
    updateMenuSecondaire(&optionMenuSecondaire);
    updateMenuSecondaire(&optionMenuSecondaire);
    updateMenuSecondaire(&optionMenuSecondaire);
    
    GUI_Clear();
    
    }
}







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
void WaitforSwitchPressAndRelease(void)
{
    /* Wait for SW2 to be pressed */
    while(Status_SW2_Read() != 0);
    
    /* Wait for SW2 to be released */
    while(Status_SW2_Read() == 0);
}

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





int main(void)
{
    
    __enable_irq(); /* Enable global interrupts. */
    
    
    
    
    
    /* Initialisation de CapSense */
    
    //CapSense_Start();
    //CapSense_ScanAllWidgets();
    
    /* Initialisation des tasks */
        /* Create the queues. See the respective data-types for details of queue
       contents */
    
    touchDataQ = xQueueCreate(10, sizeof(touch_data_t));
         
    /* Create the user Tasks. See the respective Task definition for more
       details of these tasks */       
    //xTaskCreate(Task_Touch, "Touch Task", TASK_TOUCH_STACK_SIZE,
                //NULL, TASK_TOUCH_PRIORITY, NULL);

    xTaskCreate(Task_AffichageGraphique, "Task A", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
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
