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
#include <MAX30102.h>
#include <cy_scb_i2c.h>
/*
master sendstart
masterwritebyte
mastersendrestart
masterreadbyte
masterstop
*/
void ReadRegistre (){
    cy_en_scb_i2c_status_t status;
    cy_en_scb_i2c_command_t commande;
    status = I2C_MasterSendStart(ADRESSE_I2C,CY_SCB_I2C_READ_XFER,I2C_TIMEOUT);
    if(status==CY_SCB_I2C_SUCCESS){
        do{
        status=I2C_MasterReadByte(CY_SCB_I2C_ACK,
        }
        while(status != CY_SCB_I2C_MASTER_MANUAL_BUS_ERR)
    }
        
}
// I2C_TIMEOUT ou CY_SCB_I2C_MASTER_MANUAL_TIMEOUT
void WriteRegistre(uint8_t Data){ //buffer ? pointeur?
    
    cy_en_scb_i2c_status_t status;
        status = I2C_MasterSendStart(ADRESSE_I2C,CY_SCB_I2C_WRITE_XFER,I2C_TIMEOUT);
    if(status == CY_SCB_I2C_SUCCESS){
        do{
        status=I2C_MasterWriteByte(Data,I2C_TIMEOUT);
        }
        while (status != CY_SCB_I2C_MASTER_MANUAL_NAK);
    }
   
}


/* [] END OF FILE */
