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
#ifndef  TOUCH_TASK_H
#define  TOUCH_TASK_H

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
    
#include "display_task.h"   

/* Data-type for touch data */
typedef enum
{
    BUTTON0_TOUCHED     = 0x00u,
    BUTTON1_TOUCHED     = 0x01u,  
    BUTTON2_TOUCHED     = 0x02u, 
    NO_TOUCH            = 0x03u
    
}   touch_data_t; 
    
    
    
/* Queue handle for sending touch data */
extern QueueHandle_t touchDataQ;
    

void Task_Touch(void * arg);

    
#endif


/* [] END OF FILE */
