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
#include "semphr.h"




static int8_t bmi160Write(uint8_t dev_adresse, uint8_t reg_adresse, uint8_t *data, uint16_t length){
    
    Cy_SCB_I2C_MasterSendStart(I2C_BMI_HW,dev_adresse,CY_SCB_I2C_WRITE_XFER,0,&I2C_BMI_context);
    Cy_SCB_I2C_MasterWriteByte(I2C_BMI_HW,reg_adresse,0,&I2C_BMI_context);
    for(int i = 0; i<length; i++)
    {
        Cy_SCB_I2C_MasterWriteByte(I2C_BMI_HW,data[i],0,&I2C_BMI_context);   
    }
    
    Cy_SCB_I2C_MasterSendStop(I2C_BMI_HW,0,&I2C_BMI_context);
    return 0;
} // pk pas void

static int8_t bmi160Read(uint8_t dev_adresse, uint8_t reg_adresse, uint8_t *data, uint16_t length){
    
    Cy_SCB_I2C_MasterSendStart(I2C_BMI_HW,dev_adresse,CY_SCB_I2C_WRITE_XFER,0,&I2C_BMI_context);
    Cy_SCB_I2C_MasterWriteByte(I2C_BMI_HW,reg_adresse,0,&I2C_BMI_context);
    Cy_SCB_I2C_MasterSendReStart(I2C_BMI_HW,dev_adresse,CY_SCB_I2C_READ_XFER,0,&I2C_BMI_context);
    for(int i =0; i<length-1; i++)
    {
        Cy_SCB_I2C_MasterReadByte(I2C_BMI_HW,CY_SCB_I2C_ACK,&data[i],0,&I2C_BMI_context);
    }
    Cy_SCB_I2C_MasterReadByte(I2C_BMI_HW,CY_SCB_I2C_NAK,&data[length-1],0,&I2C_BMI_context);
    
    Cy_SCB_I2C_MasterSendStop(I2C_BMI_HW,0,&I2C_BMI_context);
    
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


void get_accData ()
{
    
    //(void)arg;
    
    //I2C_BMI_Start();
    //bmi160Config();
    
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

void anyMotionInt_set()
{
 
    /*Structure qui va contenir la configuration de l'interruption*/
    struct bmi160_int_settg int_config;
    /*Variable qui contient le retour de BMI160 APIs*/
    uint8_t resultat = 0;
    
    int_config.int_channel = BMI160_INT_CHANNEL_1;
    int_config.int_type = BMI160_ACC_ANY_MOTION_INT;
    
    /*Interrupt pin configuration*/
    
    int_config.int_pin_settg.output_en=BMI160_ENABLE;
    int_config.int_pin_settg.output_mode=BMI160_DISABLE;
    int_config.int_pin_settg.output_type=BMI160_ENABLE;
    int_config.int_pin_settg.edge_ctrl=BMI160_ENABLE;
    int_config.int_pin_settg.input_en = BMI160_DISABLE;
    int_config.int_pin_settg.latch_dur=BMI160_LATCH_DUR_2_56_SEC;
    
    /*Interrupt Type configuration*/

    int_config.int_type_cfg.acc_any_motion_int.anymotion_en= BMI160_ENABLE;
    int_config.int_type_cfg.acc_any_motion_int.anymotion_x=BMI160_ENABLE;
    int_config.int_type_cfg.acc_any_motion_int.anymotion_y=BMI160_ENABLE;
    int_config.int_type_cfg.acc_any_motion_int.anymotion_z=BMI160_ENABLE;
    int_config.int_type_cfg.acc_any_motion_int.anymotion_dur=3;
    
    int_config.int_type_cfg.acc_any_motion_int.anymotion_thr=10;
    
    resultat=bmi160_set_int_config(&int_config,&bmi160Sensor);
    
    Cy_SysInt_Init(&SysInt_AccINT_cfg, anyMotion_Interrupt);
    NVIC_EnableIRQ(SysInt_AccINT_cfg.intrSrc);
    
    
}

void anyMotion_Interrupt ()
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    
    /* Clear any pending interrupts */
    Cy_GPIO_ClearInterrupt(Pin_Acc_INT_PORT,Pin_Acc_INT_NUM);
    NVIC_ClearPendingIRQ(SysInt_AccINT_cfg.intrSrc);
    
    printf("Ca bouge trop!");
    
    /* Resume Task_Motion*/
    xHigherPriorityTaskWoken = xTaskResumeFromISR(xTaskHandleMotion);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

 
    
void Task_Motion(void* pvParameters)
{
    (void)pvParameters;
    
    SemaphoreHandle_t xSemaphoreI2C= xSemaphoreCreateBinary();
    I2C_BMI_Start();
    bmi160Config();
    anyMotionInt_set();
    get_accData();

    
}


/* [] END OF FILE */
