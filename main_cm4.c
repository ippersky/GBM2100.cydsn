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

#include <stdio.h>  //?
#include <unistd.h> //?

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"     
#include "semphr.h"
#include "params.h"



#include "display_task.h"
#include "touch_task.h"

//vecteur sinus de 150 élément entre -1 et 1
// float vectorSinus[]={0.099833,0.29552,0.47943,0.64422,0.78333,0.89121,0.96356,0.99749,0.99166,0.9463,0.86321,0.74571,0.59847,0.42738,0.23925,0.041581,-0.15775,-0.35078,-0.52984,-0.68777,-0.81828,-0.91617,-0.97753,-0.99992,-0.98245,-0.92581,-0.83227,-0.70554,-0.55069,-0.37388,-0.18216,0.016814,0.21512,0.40485,0.57844,0.72897,0.85044,0.938,0.98817,0.99894,0.96989,0.90217,0.79849,0.66297,0.50102,0.3191,0.12445,-0.075151,-0.27176,-0.45754,-0.62507,-0.76769,-0.8797,-0.95664,-0.99544,-0.99455,-0.95402,-0.87545,-0.76198,-0.61814,-0.44965,-0.26323,-0.066322,0.13323,0.32747,0.50866,0.66957,0.80378,0.90595,0.97201,0.99931,0.98677,0.9349,0.84575,0.72288,0.5712,0.39674,0.20647,0.0079632,-0.19086,-0.38207,-0.55805,-0.71179,-0.83714,-0.92912,-0.98407,-0.99977,-0.97563,-0.91258,-0.81316,-0.68131,-0.52231,-0.34248,-0.149,0.050423,0.24783,0.43537,0.60554,0.75157,0.86764,0.94912,0.99277,0.99683,0.96115,0.88716,0.77779,0.63742,0.47164,0.28705,0.091022,-0.10864,-0.30396,-0.48717,-0.65096,-0.7888,-0.89519,-0.96589,-0.99808,-0.99049,-0.9434,-0.85871,-0.73978,-0.59136,-0.41936,-0.23065};


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

    
    int longueurAffichage = longueurData/espacement;

    
    for(;;){
        
    convertirVecteurEnInt16(vecteurConverti, vecteurData, longueurData);
    creerVecteurAffichage(vecteurConverti, vecteurAffichage, longueurData, espacement);
    GUI_DrawGraph(vecteurAffichage, longueurAffichage, 0, 0); 
    UpdateDisplay(CY_EINK_FULL_4STAGE, true);
    
    updateParametres(SPO2, BPM);
    
    afficherMenuSecondaire();
    
    GUI_Clear();
    
    }
}



/*
void convertirVecteurEnInt16(int16_t* vConverti, int32_t* vData, size_t lData){
    for(int i =0; i < lData; i++){
        vConverti[i] = vData[i]/100;
    }
}


int16_t trouverMinimumVecteur(int16_t* vConverti, size_t lData){
    int16_t min = vConverti[0];
    for(int i =1; i < lData; i++){
        if(vConverti[i] < min){
            min = vConverti[i];
        }
    }
    return min;
}


int16_t trouverMaximumVecteur(int16_t* vConverti, size_t lData){
    int16_t max = vConverti[0];
    for(int i =1; i < lData; i++){
        if(vConverti[i] > max){
            max = vConverti[i];
        }
    }
    return max;
}


void creerVecteurAffichage(int16_t* vConverti, int16_t* vAffichage, size_t lData, size_t espacement){
    
    int16_t min = trouverMinimumVecteur(vConverti, lData);
    int16_t max = trouverMaximumVecteur(vConverti, lData);
    
    int j = 0;
    for(int i=0; i < lData; i+=espacement){
        vAffichage[j] = (150*(vConverti[i]-min))/(max-min); // graphique : 150 pixels en y
        j++;                                                      // probleme de convertion int-nbre a virgule? 
    }
}




// Image buffer cache //
uint8 imageBufferCache[CY_EINK_FRAME_SIZE] = {0};

*/


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
    
    
    /* Initialize emWin Graphics */
    GUI_Init();
    
    /* Start the eInk display interface and turn on the display power */
    Cy_EINK_Start(20);
    Cy_EINK_Power(1);
    
    GUI_SetPenSize(1);
    GUI_SetColor(GUI_BLACK);
    GUI_SetBkColor(GUI_WHITE);
    GUI_Clear();
    
    
    /* Initialisation de CapSense */
    
    //CapSense_Start();
    //CapSense_ScanAllWidgets();
    
    /* Initialisation des tasks */
        /* Create the queues. See the respective data-types for details of queue
       contents */
    //extern QueueHandle_t touchDataQ;
    //touchDataQ = xQueueCreate(TOUCH_ELEMENT_QUEUE_LEN, sizeof(touch_data_t));
         
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
