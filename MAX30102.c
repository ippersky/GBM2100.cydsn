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


void MAX30102_config()
{
WriteRegistre(REG_INTR_ENABLE_1, 0xe0); //je ne comprend pas tres bien les trois premier bits d'interrupt
        
WriteRegistre(REG_INTR_ENABLE_1, 0x02);//je ne comprend pas le B1 de interrupt

WriteRegistre(REG_FIFO_WR, 0x00); //still not catching FIFO+WR_PTR[4:0]
       
WriteRegistre(REG_OVFLOW_COUNTER, 0x00); //hein
        
WriteRegistre(REG_FIFO_RD, 0x00); //
        
WriteRegistre(REG_FIFO_CONFIG, 0x1f); //no smp averaging, fifo rollover on, fifo fill = 15 empty data sample;
        
WriteRegistre(REG_MODE_CONFIG, 0x03); //SPO2 Mode (Red and IR)
       
WriteRegistre(REG_SPO2_CONFIG, 0x29); //ADC Range control=4096nA, Sample rate=200Hz, Pulse width =118microsec

WriteRegistre(REG_LED_AMP_1, 0x1f); //6.2 mA

WriteRegistre(REG_LED_AMP_2, 0x1f); //6.2 mA

//temps config ??

}    
void ReadRegistre (uint8_t* data, uint8_t adresse){
    cy_en_scb_i2c_status_t status;
    
    status = I2C_MasterSendStart(ADRESSE_I2C,CY_SCB_I2C_WRITE_XFER,I2C_TIMEOUT);
    if(status==CY_SCB_I2C_SUCCESS){
    I2C_MasterWriteByte(adresse, I2C_TIMEOUT); // ou FIFO_WR_PTR
    }
    status = I2C_MasterSendReStart(adresse,CY_SCB_I2C_READ_XFER,I2C_TIMEOUT);  
    if(status==CY_SCB_I2C_SUCCESS){
        do{
        status=I2C_MasterReadByte(CY_SCB_I2C_ACK,data,I2C_TIMEOUT); //Qu'est-ce que j'ai read(Le FIFO_WR_PTR ou la donnée)
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

void ReadFIFO(uint32_t *red_LED, uint32_t *ir_LED){
    
    char i2c_data [6];
    uint8_t *WR_PTR;
    
    ReadRegistre(WR_PTR,REG_FIFO_WR); //à vérifier
    
    //blablabla
    
    *red_LED=(i2c_data[0]<<16)+(i2c_data[1]<<8)+(i2c_data[2]);
    *ir_LED= (i2c_data[3]<<16)+(i2c_data[4]<<8)+(i2c_data[5]);
    
    
}

    
                
    



/* [] END OF FILE */
