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
#include "MAX30102.h"
#include "project.h"

/*
master sendstart
masterwritebyte
mastersendrestart
masterreadbyte
masterstop
*/
void ReadRegistre (uint8_t* data, uint8_t adresse){
    cy_en_scb_i2c_status_t status;
    
    status = I2C_MasterSendStart(ADRESSE_I2C,CY_SCB_I2C_READ_XFER,I2C_TIMEOUT);
    if(status==CY_SCB_I2C_SUCCESS){
    I2C_MasterWriteByte(adresse,I2C_TIMEOUT);
    }
    status = I2C_MasterSendReStart(adresse,CY_SCB_I2C_WRITE_XFER,I2C_TIMEOUT);  
    if(status==CY_SCB_I2C_SUCCESS){
        do{
        status=I2C_MasterReadByte(CY_SCB_I2C_ACK,data,I2C_TIMEOUT);
        }
        while(status != CY_SCB_I2C_MASTER_MANUAL_NAK);
    }
        
}

void WriteRegistre(uint8_t data, uint8_t adresse){ 
    
    cy_en_scb_i2c_status_t status;
    
    status = I2C_MasterSendStart(ADRESSE_I2C,CY_SCB_I2C_WRITE_XFER,I2C_TIMEOUT);
    if(status==CY_SCB_I2C_SUCCESS){
        I2C_MasterWriteByte(adresse,I2C_TIMEOUT);
    }
    status = I2C_MasterSendReStart(adresse,CY_SCB_I2C_WRITE_XFER,I2C_TIMEOUT);
    if(status == CY_SCB_I2C_SUCCESS){
        do{
        status=I2C_MasterWriteByte(data,I2C_TIMEOUT);
        }
        while (status != CY_SCB_I2C_MASTER_MANUAL_NAK);
    }
   
}


/* [] END OF FILE */
