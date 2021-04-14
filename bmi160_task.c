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
#include "FreeRTOS.h"
#include "task.h"
#include "bmi160_task.h"
#include "bmi160.h"
#include <stdio.h>




static int8_t bmi160Write(uint8_t reg_adresse, uint8_t *data, uint16_t length){
    
    I2C_BMI_MasterSendStart(BMI160_I2C_ADDR,CY_SCB_I2C_WRITE_XFER,0);
    I2C_BMI_MasterWriteByte(reg_adresse,0);
    for(int idx; idx<length; idx++)
    {
        I2C_BMI_MasterWriteByte(data[idx],0);
        
    }
    I2C_BMI_MasterSendStop(0);
    return 0;
} // pk pas void

static int8_t bmi160Read(uint8_t reg_adresse, uint8_t *data, uint16_t length){
    
    I2C_BMI_MasterSendStart(BMI160_I2C_ADDR,CY_SCB_I2C_WRITE_XFER,0);
    I2C_BMI_MasterWriteByte(reg_adresse,0);
    I2C_BMI_MasterSendReStart(BMI160_I2C_ADDR,CY_SCB_I2C_READ_XFER,0);
    for (int idx=0; idx<length-1; idx++)
    {
        I2C_BMI_MasterReadByte(CY_SCB_I2C_ACK,&data[idx],0);
        
    }
    I2C_BMI_MasterReadByte(CY_SCB_I2C_NAK,&data[length-1],0);
    
    I2C_BMI_MasterSendStop(0);
    
    return 0 ;
    
    
}

static void bmi160Config ()
{
    vTaskDelay(100); //guess
    /*BMI160*/

    bmi160Sensor.read       = (bmi160_read_fptr_t)bmi160Read;
    bmi160Sensor.write      = (bmi160_write_fptr_t)bmi160Write;
    bmi160Sensor.delay_ms   = (bmi160_delay_fptr_t)vTaskDelay;
    bmi160Sensor.id         = BMI160_I2C_ADDR; //I2C device address
    
    bmi160_init(&bmi160Sensor);// initialize the device
    
    bmi160Sensor.gyro_cfg.odr = BMI160_GYRO_ODR_800HZ;
    bmi160Sensor.gyro_cfg.range = BMI160_GYRO_RANGE_125_DPS;
    bmi160Sensor.gyro_cfg.bw = BMI160_GYRO_BW_OSR4_MODE;
    
    bmi160Sensor.gyro_cfg.power = BMI160_GYRO_NORMAL_MODE;
    
    bmi160Sensor.accel_cfg.odr = BMI160_ACCEL_ODR_1600HZ;
    bmi160Sensor.accel_cfg.range = BMI160_ACCEL_RANGE_2G;
    bmi160Sensor.accel_cfg.bw = BMI160_ACCEL_BW_OSR4_AVG1;
    bmi160Sensor.accel_cfg.power = BMI160_ACCEL_NORMAL_MODE;
    
    /*set the sensor configuration*/
    bmi160_set_sens_conf(&bmi160Sensor);
    bmi160Sensor.delay_ms(50);
    
}


void get_accData (void *arg)
{
    
    (void)arg;
    
    I2C_BMI_Start();
    bmi160Config();
    
    struct bmi160_sensor_data acc;
    float gx, gy, gz;
    
    while(1)
    {
        bmi160_get_sensor_data(BMI160_ACCEL_ONLY, &acc, NULL, &bmi160Sensor);
        
        gx= (float)acc.x/MAXACCEL;
        gy= (float)acc.y/MAXACCEL;
        gz= (float)acc.z/MAXACCEL;
        
        printf("x=%1.2f y=%1.2f z=%1.2f \r\n",gx,gy,gz);
        vTaskDelay(200);
        
    }
    
    
    
    
}
 
    


/* [] END OF FILE */
