/******************************************************************************

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
#include "bmi160.h"
//#include <sdtio.h>



int main(void)
{
    
    __enable_irq(); /* Enable global interrupts. */
    /*Définitions*/
    #define BUFFER_LENGTH 2000
    
    /*Variables*/
    
    /*
    float32_t red_LED_buffer [2000]; //pour 10 secondes d'acquisition
    float32_t ir_LED_buffer [2000];
    
    uint16_t compteur=0;
    */
    
    /*Start-up code*/
    
    //I2C_MAX_Start();
    UART_1_Start();
    xTaskCreate(Task_Motion, "Motion task",400,0,1,0);
    vTaskStartScheduler();
    /*Configure I2C*/
    //MAX30102_config();
    
    
    
    
    for(;;)
    {
        /*
        uint16_t bufferIndex=0;
        uint16_t halfbufferIndex=0;
        
        
            
            
            for (bufferIndex=0; bufferIndex<BUFFER_LENGTH ; bufferIndex++)
        {
            
            readFIFO(red_LED_buffer, ir_LED_buffer,bufferIndex);
            printf("%1.2f\r\n",ir_LED_buffer[bufferIndex]);
           
            CyDelay(5);
            
           
            if (bufferIndex == BUFFER_LENGTH-1){//vérifier
                bufferIndex=0;
            }
            
        }
        */
    }
}

/* [] END OF FILE */
