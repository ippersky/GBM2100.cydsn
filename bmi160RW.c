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
#include "bmi160RW.h"

static int8_t bmi160Write(uint8_t reg_adresse,uint8_t *data,uint8_t length)
{
    I2C_BMI_MasterSendStart(ADRESSE_BMI,CY_SCB_I2C_WRITE_XFER,0);
    I2C_BMI_MasterWriteByte(reg_adresse,0);
    for(int idx = 0; idx<length; idx++)
    {
        I2C_BMI_MasterWriteByte(ADRESSE_BMI,data[idx]);
        
    }
    I2C_BMI_MasterSendStop(0);
    
 return 0;   
} // pk juste pas un void

static int8_t bmi160Read(uint8_t reg_adresse, uint8_t *data, uint8_t length)
{
    I2C_BMI_MasterSendStart(ADRESSE_BMI,CY_SCB_I2C_WRITE_XFER,0);
    I2C_BMI_MasterWriteByte(reg_adresse,0);
    I2C_BMI_MasterSendReStart(ADRESSE_BMI,CY_SCB_I2C_READ_XFER,0);
    for(int idx = 0; idx<length-1; idx++)
    {
        I2C_BMI_MasterReadByte(CY_SCB_I2C_ACK,&data[idx],0);
    }
    I2C_BMI_MasterReadByte(CY_SCB_I2C_NAK,&data[length-1],0);
    I2C_BMI_MasterSendStop(0);
    return 0;
}//pk pas void

static void bmi160Config ()
{
    
    
    
}


/* [] END OF FILE */
