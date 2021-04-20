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
CyDelay(100);    
    
writeRegistre(REG_INTR_ENABLE_1, 0xd0); //enable ...
        
writeRegistre(REG_INTR_ENABLE_2, 0x00);

writeRegistre(REG_FIFO_WR,0x00);

writeRegistre(REG_OVFLOW_COUNTER, 0x00);

writeRegistre(REG_FIFO_RD,0x00);
        
writeRegistre(REG_FIFO_CONFIG, 0x1f); //no smp averaging, fifo rollover on, fifo fill = 15 empty data sample;
        
writeRegistre(REG_MODE_CONFIG, 0x03); //SPO2 Mode (Red and IR)
       
writeRegistre(REG_SPO2_CONFIG, 0x29); //ADC Range control=4096nA, Sample rate=200Hz, Pulse width =118microsec

writeRegistre(REG_LED_AMP_1, 0x32); //6.2 mA = 0x1f?? 0x32 = 50 mA

writeRegistre(REG_LED_AMP_2, 0x32); //6.2 mA = 0x1f??



}    
uint8_t readRegistre (uint8_t adresse){
    cy_en_scb_i2c_status_t status;
    uint8_t regValue = 0;
    
    status = I2C_MAX_MasterSendStart(ADRESSE_MAX,CY_SCB_I2C_WRITE_XFER,I2C_TIMEOUT);
    if(status==CY_SCB_I2C_SUCCESS){
    I2C_MAX_MasterWriteByte(adresse, I2C_TIMEOUT); 
    }
    status = I2C_MAX_MasterSendReStart(ADRESSE_MAX,CY_SCB_I2C_READ_XFER,I2C_TIMEOUT);  
    if(status==CY_SCB_I2C_SUCCESS){
        do{
        status=I2C_MAX_MasterReadByte(CY_SCB_I2C_ACK,(uint8_t*)&regValue,I2C_TIMEOUT); //Pourquoi NACK ?
        }
        while(status != CY_SCB_I2C_MASTER_MANUAL_NAK);
    }
    status = I2C_MAX_MasterSendStop(I2C_TIMEOUT);
    return regValue;
}

void writeRegistre(uint8_t adresse, uint8_t data){ 
    
    cy_en_scb_i2c_status_t status;
    
    status = I2C_MAX_MasterSendStart(ADRESSE_MAX,CY_SCB_I2C_WRITE_XFER,I2C_TIMEOUT);
    if(status==CY_SCB_I2C_SUCCESS){
        I2C_MAX_MasterWriteByte(adresse,I2C_TIMEOUT); 
    }
    
    I2C_MAX_MasterWriteByte(data,I2C_TIMEOUT);
    I2C_MAX_MasterSendStop(I2C_TIMEOUT);
    
} 

void readMultipleBytes(uint8_t baseAddress, uint8_t *buffer, uint8_t length)
{
        I2C_MAX_MasterSendStart(ADRESSE_MAX,CY_SCB_I2C_WRITE_XFER,I2C_TIMEOUT); 
        I2C_MAX_MasterWriteByte(baseAddress,I2C_TIMEOUT);       
        I2C_MAX_MasterSendReStart(ADRESSE_MAX,CY_SCB_I2C_READ_XFER,I2C_TIMEOUT);
        uint8_t idx = 0;
        for(idx=0 ; idx < length-1 ; idx++){ 
            I2C_MAX_MasterReadByte(CY_SCB_I2C_ACK,(uint8_t*)&buffer[idx],I2C_TIMEOUT);  
        }
        I2C_MAX_MasterReadByte(CY_SCB_I2C_NAK,(uint8_t*)&buffer[idx],I2C_TIMEOUT);
        I2C_MAX_MasterSendStop(I2C_TIMEOUT); 
}

void readFIFO(uint32_t *red_LED, uint32_t *ir_LED, uint16_t compteur){
    //ou remplacer par tableau
    uint8_t i2c_data [6];
        
    readMultipleBytes(REG_FIFO_DATA,i2c_data,6);
    red_LED[compteur]=((0b00000011&i2c_data[0])<<16)+(i2c_data[1]<<8)+(i2c_data[2]);
    ir_LED[compteur]=((i2c_data[3]<<16)+(i2c_data[4]<<8)+(i2c_data[5]));
    //changement en uint32_t et enlever le cast en (float)
    
    // vérification bonnes données
    char cRed[6];
    itoa(red_LED[compteur], cRed, 10);
    UART_1_PutString(cRed);
    UART_1_PutString("\n\r");
    
    
}

void changeLED_red (short int ledAmp_red)
{
    
    writeRegistre(REG_LED_AMP_1, ledAmp_red); 
  
    
}
    

void changeLED_IR (short int ledAmp_IR)
{
    
    
    writeRegistre(REG_LED_AMP_2, ledAmp_IR); 
    
}
    


/* [] END OF FILE */
