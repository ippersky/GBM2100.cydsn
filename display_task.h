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

    
#include "touch_task.h"     // pour avoir access a la queue et MACROs de touch_data_t
    

  
extern uint8 imageBufferCache[CY_EINK_FRAME_SIZE];  // declaration dans affichage.c 
//extern int16_t vecteurConverti[160];
//extern int16_t vecteurAffichage[100];


#define GET_SIZE(data)      sizeof(data)/sizeof(int32_t);

typedef enum
{
    MENU_PRINCIPAL      = 0u,
    MENU_SECONDAIRE     = 1u,  
    MENU_TERTIAIRE_1    = 2u, 
    MENU_TERTIAIRE_2    = 3u,
    MENU_TERTIAIRE_3    = 4u,          
    MENU_TERTIAIRE_4    = 5u,  
    MENU_QUAT_1_1       = 6u,
    MENU_QUAT_1_2       = 7u,
    MENU_QUAT_2_1       = 8u,
    MENU_QUAT_2_2       = 9u,    
    MENU_QUAT_3_1       = 10u,    
    MENU_QUAT_3_2       = 11u
    
}   page_data_t; 

#define ON      0u
#define OFF     1u

#define RED     0u
#define IR      1u

char *gcvt(double val, int precision, char *buf);
char *gcvtf(float val, int precision, char *buf);


void UpdateDisplay(cy_eink_update_t updateMethod, bool powerCycle);



void DisplayInit(void);

void convertirVecteurEnInt16(int16_t* vConverti, uint32_t* vData, size_t lData);
void creerVecteurAffichage(int16_t* vConverti, int16_t* vAffichage, size_t lData, size_t espacement);
void updateParametres(float32_t SPO2, float32_t BPM, uint8_t currentCourbe);


//void afficherMenuPrincipal(void);


void Task_Display(void *data);


///// essaie ////
//void Task_Touch(void * arg);
    
#endif




/* [] END OF FILE */
