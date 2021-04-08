/* ========================================
 *
 * Copyright EQUIPE 5, 2021
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF EQUIPE 5.
 *
 * ========================================
*/
#include "MAX30102.h"
#include "project.h"


void MAX30102_config()
{
writeRegistre(REG_INTR_ENABLE_1, 0x00); 
        
writeRegistre(REG_INTR_ENABLE_2, 0x00);
        
writeRegistre(REG_FIFO_CONFIG, 0x1f); //no smp averaging, fifo rollover on, fifo fill = 15 empty data sample;
        
writeRegistre(REG_MODE_CONFIG, 0x03); //SPO2 Mode (Red and IR)
       
writeRegistre(REG_SPO2_CONFIG, 0x29); //ADC Range control=4096nA, Sample rate=200Hz, Pulse width =118microsec

writeRegistre(REG_LED_AMP_1, 0x1f); //6.2 mA

writeRegistre(REG_LED_AMP_2, 0x1f); //6.2 mA



}    
uint8_t readRegistre (uint8_t adresse){
    cy_en_scb_i2c_status_t status;
    uint8_t regValue = 0;
    
    status = I2C_MasterSendStart(ADRESSE_I2C,CY_SCB_I2C_WRITE_XFER,I2C_TIMEOUT);
    if(status==CY_SCB_I2C_SUCCESS){
    I2C_MasterWriteByte(adresse, I2C_TIMEOUT); 
    }
    status = I2C_MasterSendReStart(ADRESSE_I2C,CY_SCB_I2C_READ_XFER,I2C_TIMEOUT);  
    if(status==CY_SCB_I2C_SUCCESS){
        do{
        status=I2C_MasterReadByte(CY_SCB_I2C_ACK,(uint8_t*)&regValue,I2C_TIMEOUT); //Pourquoi NACK ?
        }
        while(status != CY_SCB_I2C_MASTER_MANUAL_NAK);
    }
    status = I2C_MasterSendStop(I2C_TIMEOUT);
    return regValue;
}

void writeRegistre(uint8_t adresse, uint8_t data){ 
    
    cy_en_scb_i2c_status_t status;
    
    status = I2C_MasterSendStart(ADRESSE_I2C,CY_SCB_I2C_WRITE_XFER,I2C_TIMEOUT);
    if(status==CY_SCB_I2C_SUCCESS){
        I2C_MasterWriteByte(adresse,I2C_TIMEOUT);
    }
    
    I2C_MasterWriteByte(data,I2C_TIMEOUT);
    I2C_MasterSendStop(I2C_TIMEOUT);
    
} 

void readFIFO(uint32_t *red_LED, uint32_t *ir_LED, uint8_t dataAdress, uint8_t nSamples){
    
    uint8_t i2c_data [6];
    
    
    I2C_MasterSendStart(ADRESSE_I2C,CY_SCB_I2C_WRITE_XFER,I2C_TIMEOUT);
    I2C_MasterWriteByte(dataAdress,I2C_TIMEOUT);
    I2C_MasterSendReStart(ADRESSE_I2C,CY_SCB_I2C_READ_XFER,I2C_TIMEOUT);
    
    uint8_t index1=0;
    uint8_t index2=0;
    
    for(index1=0; index1<nSamples-1; index1++)//exemple ça lit 5 samples de 24 bits chaque
    {
        for(index2=0; index2<5; index2++) //explique pk
            {
            I2C_MasterReadByte(CY_SCB_I2C_ACK,(uint8_t*)&i2c_data[index2],I2C_TIMEOUT); // ecq il va y avoir plus que 24 bit?
            }
            *red_LED=(i2c_data[0]<<16)+(i2c_data[1]<<8)+(i2c_data[2]);
            *ir_LED= (i2c_data[3]<<16)+(i2c_data[4]<<8)+(i2c_data[5]);
            red_LED++;
            ir_LED++;
            uint8_t i2c_data = { 0 }; // comme ça?
    }
    I2C_MasterReadByte(CY_SCB_I2C_NAK,(uint8_t*)&i2c_data[index1],I2C_TIMEOUT);
    
    I2C_MasterSendStop(I2C_TIMEOUT);
}

void changeLED1 (short int ledAmp1)//ecq une données en chiffre genre 3.6 ou 0x0F
{
    ledAmp1=ledAmp1/0.2;

    
    writeRegistre(REG_LED_AMP_1, ledAmp1); 
        
    
}
    
void changeLED2 (short ledAmp2)
{
    ledAmp2=ledAmp2/0.2;
    writeRegistre(REG_LED_AMP_2, ledAmp2); 
        
    
}



/* [] END OF FILE */
