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

#ifndef  AFFICHAGE_H
#define  AFFICHAGE_H


#include "project.h"
#include "GUI.h"
#include "pervasive_eink_hardware_driver.h"
#include "cy_eink_library.h"
#include "LCDConf.h"
#include <stdlib.h>

    
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"     
#include "semphr.h"
//#include "params.h"
    
    
//#include ... task, RTOS stuff
  
extern uint8 imageBufferCache[CY_EINK_FRAME_SIZE];  // declaration dans affichage.c 
//extern int16_t vecteurConverti[160];
//extern int16_t vecteurAffichage[100];


#define GET_SIZE(data)      sizeof(data)/sizeof(int32_t);
#define MENU_PRINCIPAL      0       // en hexa ??
#define MENU_SECONDAIRE     1 


void UpdateDisplay(cy_eink_update_t updateMethod, bool powerCycle);
void ClearScreen(void);
void WaitforSwitchPressAndRelease(void);

void DisplayInit(void);

void convertirVecteurEnInt16(int16_t* vConverti, int32_t* vData, size_t lData);
void creerVecteurAffichage(int16_t* vConverti, int16_t* vAffichage, size_t lData, size_t espacement);

void updateParametres(int SPO2, int BPM);
void afficherMenuSecondaire(uint8_t * ptrOptionPresent);
void updateMenuSecondaire(uint8_t * ptrOptionPresent);

void Task_AffichageGraphique(void *data);


    
#endif




/* [] END OF FILE */
