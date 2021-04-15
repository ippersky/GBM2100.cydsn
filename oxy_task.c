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
    uint16_t halfbufferFlag = 0; //Utiliser ou non?
    uint16_t endbufferFlaf = 0; //Utiliser ou non?
    
    for(bufferIndex=0; bufferIndex<BUFFER_LENGTH;bufferIndex++)
    {
        readFIFO(redLED_buffer, irLED_buffer, bufferIndex);
        
        
        
        if (bufferIndex==BUFFER_LENGTH/2)
        {
            BPM=HeartRate(redLED_buffer,0,BUFFER_LENGTH/2);
            SPO2=calculSpO2(redLED_buffer,irLED_buffer,0,BUFFER_LENGTH/2);
            
        }
        
        if (bufferIndex==BUFFER_LENGTH)
        {
            BPM=HeartRate(redLED_buffer,BUFFER_LENGTH/2, BUFFER_LENGTH);
            SPO2=calculSpO2(redLED_buffer,irLED_buffer,BUFFER_LENGTH/2,BUFFER_LENGTH);
            bufferIndex=0;
        }
        
    }
    
    
    
}











/* [] END OF FILE */
