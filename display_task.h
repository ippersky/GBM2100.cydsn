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

#ifndef  DISPLAY_TASK_H
#define  DISPLAY_TASK_H


#include "project.h"
#include "GUI.h"
#include "pervasive_eink_hardware_driver.h"
#include "cy_eink_library.h"
#include "LCDConf.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
    
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"     
#include "semphr.h"
//#include "params.h"
    
#include "touch_task.h"     // pour avoir access a la queue et MACROs de touch_data_t
    

  
extern uint8 imageBufferCache[CY_EINK_FRAME_SIZE];  // declaration dans affichage.c 
//extern int16_t vecteurConverti[160];
//extern int16_t vecteurAffichage[100];


#define GET_SIZE(data)      sizeof(data)/sizeof(int32_t);
//#define MENU_PRINCIPAL      0       // en hexa ??
//#define MENU_SECONDAIRE     1 

typedef enum
{
    MENU_PRINCIPAL      = 0u,
    MENU_SECONDAIRE     = 1u,  
    MENU_TERTIAIRE_1    = 2u, 
    MENU_TERTIAIRE_2    = 3u,
    MENU_TERTIAIRE_3    = 4u,          //0x04u,
    MENU_TERTIAIRE_4    = 5u,  
    MENU_QUAT_1_1       = 6u,
    MENU_QUAT_1_2       = 7u,
    MENU_QUAT_2_1       = 8u,
    MENU_QUAT_2_2       = 9u,    
    MENU_QUAT_3_1       = 10u,    
    MENU_QUAT_3_2       = 11u
    
}   page_data_t; 


void UpdateDisplay(cy_eink_update_t updateMethod, bool powerCycle);
void ClearScreen(void);
void WaitforSwitchPressAndRelease(void);

void DisplayInit(void);

void convertirVecteurEnInt16(int16_t* vConverti, int32_t* vData, size_t lData);
void creerVecteurAffichage(int16_t* vConverti, int16_t* vAffichage, size_t lData, size_t espacement);
void updateParametres(int SPO2, int BPM);


void afficherMenuPrincipal(void);


void afficherMenuSecondaire(uint8_t * ptrOptionPresent);
void updateMenuSecondaire(uint8_t * ptrOptionPresent);


void afficherMenuTertiaire1(uint8_t * ptrOptionPresent);
void afficherMenuTertiaire2(uint8_t * ptrOptionPresent);
void afficherMenuTertiaire3(uint8_t * ptrOptionPresent);
void afficherMenuTertiaire4(uint8_t * ptrOptionPresent);
void updateMenuTertiaire(uint8_t * ptrOptionPresent);





void Task_AffichageGraphique(void *data);


///// essaie ////
void Task_Touch(void * arg);
    
#endif




/* [] END OF FILE */
