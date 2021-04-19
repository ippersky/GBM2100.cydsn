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
#include "project.h"
#include "algorithme.h"
#include "MAX30102.h"
#include "oxy_task.h"
#include "FreeRTOS.h"

void vtraitement ()
{
    
    uint16_t bufferIndex = 0;
    
    for(bufferIndex=0; bufferIndex<BUFFER_LENGTH;bufferIndex++)
    {
        
        
        readFIFO(redLED_buffer, irLED_buffer, bufferIndex);
        
        
        
        if (bufferIndex==BUFFER_LENGTH/2)
        {
            BPM=HeartRate(redLED_buffer,0,BUFFER_LENGTH/2);
            SPO2=calculSpO2(redLED_buffer,irLED_buffer,0,BUFFER_LENGTH/2);
            //afficherMenuPrincipal()
        }
        
        if (bufferIndex==BUFFER_LENGTH)
        {
            BPM=HeartRate(redLED_buffer,BUFFER_LENGTH/2, BUFFER_LENGTH);
            SPO2=calculSpO2(redLED_buffer,irLED_buffer,BUFFER_LENGTH/2,BUFFER_LENGTH);
            bufferIndex=0;
            //afficher menu pricipal
        }
        
        //char cBPM[5];
        //itoa(BPM, cBPM, 10);
        
        char cSPO2[5];
        itoa(SPO2, cSPO2, 10);
        
        UART_1_PutString(cSPO2);
        
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    
    //priorité plus basse
    
    
    
    
}











/* [] END OF FILE */
