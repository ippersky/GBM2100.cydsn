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
#include <MAX30102.h>
#include "FreeRTOS.h"
#include "task.h"
#include "bmi160_task.h"
//#include <sdtio.h>



int main(void)
{
    
    __enable_irq(); /* Enable global interrupts. */
    /*Définitions*/
    #define BUFFER_LENGTH 2000
    
    /*Variables*/
    
   /* float32_t red_LED_buffer [2000]; //pour 10 secondes d'acquisition
    float32_t ir_LED_buffer [2000];
    float32_t nSamples=1;
    */
    
    /*Start-up code*/
    
   // I2C_MAX_Start();
    UART_1_Start();
    xTaskCreate(get_accData, "getAcc task",400,0,1,0);
    vTaskStartScheduler();
    /*Configure I2C*/
    //MAX30102_config();
    
    
    
    
    for(;;)
    {
       /* uint16_t bufferIndex=0;
        uint16_t halfbufferIndex=0;
        
        
            //readFIFO(red_LED_buffer, ir_LED_buffer,REG_FIFO_DATA, nSamples);
            
            for (bufferIndex=0; bufferIndex<BUFFER_LENGTH ; bufferIndex++)
        {
            readFIFO(red_LED_buffer, ir_LED_buffer,REG_FIFO_DATA, nSamples);
            char red_LED_data [50];
            char ir_LED_data [50];
            itoa (red_LED_buffer[bufferIndex], red_LED_data, 10);
            itoa (ir_LED_buffer[bufferIndex], ir_LED_data, 10);
            UART_1_PutString(red_LED_data);
            UART_1_PutString(ir_LED_data);
            UART_1_PutString("\n\r");
           
            CyDelay(100);
            
            

            if (bufferIndex == BUFFER_LENGTH-1){//vérifier
                bufferIndex=0;
            }
            
        }
        */
    }
}

/* [] END OF FILE */
