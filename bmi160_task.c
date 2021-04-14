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
#include <stdio.h>
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "bmi160_task.h"
#include "bmi160.h"
#include "bmi160_defs.h"//
#include <stdlib.h>

static struct bmi160_dev bmi160Sensor;

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
    int8_t resultat = BMI160_OK;
    
    
    
    bmi160Sensor.read       = (bmi160_com_fptr_t)bmi160Read;
    bmi160Sensor.write      = (bmi160_com_fptr_t)bmi160Write;
    bmi160Sensor.delay_ms   = (bmi160_delay_fptr_t)vTaskDelay;
    
    bmi160Sensor.id         = BMI160_I2C_ADDR;
    
    //bmi160Sensor.interface  = BMI160_I2C_INTF;

    
    resultat += bmi160_init(&bmi160Sensor);
    
    if(resultat == BMI160_OK)
    {
    //Acceléromètre
    bmi160Sensor.accel_cfg.odr = BMI160_ACCEL_ODR_200HZ;
    bmi160Sensor.accel_cfg.range = BMI160_ACCEL_RANGE_2G; // ??
    bmi160Sensor.accel_cfg.bw   = BMI160_ACCEL_BW_OSR4_AVG1; //??
    bmi160Sensor.accel_cfg.power = BMI160_ACCEL_NORMAL_MODE;
    
    //gyroscope
    bmi160Sensor.gyro_cfg.odr   = BMI160_GYRO_ODR_800HZ;
    bmi160Sensor.gyro_cfg.range = BMI160_GYRO_RANGE_125_DPS;
    bmi160Sensor.gyro_cfg.bw    = BMI160_GYRO_BW_OSR4_MODE;
    bmi160Sensor.gyro_cfg.power = BMI160_GYRO_NORMAL_MODE;
        
        
    bmi160_set_sens_conf(&bmi160Sensor);
    bmi160Sensor.delay_ms(50);
    }
        else
    {
        UART_1_PutString("Echec de la connection avec I2C");
    }
}


void get_accData ()
{
    
    
    
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
        char acc_string[40];
    
        sprintf(acc_string,"x=%1.2f y=%1.2f z=%1.2f \r\n",gx,gy,gz);
        UART_1_PutString(acc_string);
        vTaskDelay(pdMS_TO_TICKS(50));
    }
    
    
    
    
}
/*
static void orient_isr()
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    //Clear Pending iterrupts
    Cy_GPIO_ClearInterrupt(Pin_Orient_INT_PORT, Pin_Orient_INT_NUM);
    NVIC_ClearPendingIRQ(SysInt_OrientINT_cfg.intrSrc);
    
    //Allume led verte
    Cy_GPIO_Write(greenLED_PORT,redLED_NUM,1);
    
    // Resume Task_Motion
    xHigherPriorityTaskWoken = xTaskResumeFromISR(xTaskHandleMotion);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);


}
*/
/*static int8_t configOrientISR()
{
    
    
    
}
*/
/* [] END OF FILE */
