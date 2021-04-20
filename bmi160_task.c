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


/*!
 * @ Résumé Cette fonction permet de lire la valeur contenue
 * dans un registre du BMI160.
 *
 * @param[in] dev_adresse  : adresse fixe du capteur
 * @param[in] reg_adresse  : adresse du registre dans lequel il faut écrire
 * @param[in] data         : tableau où les données contenant les valeurs à écrire
 * @param[in] length       : nombre de bits à écrire
 * 
 *
 * @return 0
 * 
 */

static int8_t bmi160Write(uint8_t dev_adresse, uint8_t reg_adresse, uint8_t *data, uint16_t length){
    
    Cy_SCB_I2C_MasterSendStart(I2C_BMI_HW,dev_adresse,CY_SCB_I2C_WRITE_XFER,0,&I2C_BMI_context);
    Cy_SCB_I2C_MasterWriteByte(I2C_BMI_HW,reg_adresse,0,&I2C_BMI_context);
    for(int i = 0; i<length; i++)
    {
        Cy_SCB_I2C_MasterWriteByte(I2C_BMI_HW,data[i],0,&I2C_BMI_context);   
    }
    
    Cy_SCB_I2C_MasterSendStop(I2C_BMI_HW,0,&I2C_BMI_context);
    return 0;
} 

/*!
 * @ Résumé Cette fonction permet de lire la valeur contenue
 * dans un registre du BMI160.
 *
 * @param[in] dev_adresse  : adresse fixe du capteur.
 * @param[in] reg_adresse  : adresse du registre à lire.
 * @param[in] data         : tableau où les données lues sont stockées
 * @param[in] length       : nombre de bits à lire
 * 
 *
 * @return 0
 * 
 */

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


/*!
 * @ Résumé Cette fonction permet de configurer la capteur BMI160
 * 
 *
 * @param[in] None
 * 
 *
 * @return None
 * 
 */
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
        bmi160_get_int_status(BMI160_INT_STATUS_0,&bmi160Status,&bmi160Sensor);
        
        gx= (float)acc.x/MAXACCEL;
        gy= (float)acc.y/MAXACCEL;
        gz= (float)acc.z/MAXACCEL;
         if(bmi160Status.bit.anym==1)
        {
            printf("Interruption!\r\n");
            bmi160Status.bit.anym=0;
            vTaskDelay(pdMS_TO_TICKS(5000));
            
        }
        else
        {
        printf("x=%1.2f y=%1.2f z=%1.2f \r\n",gx,gy,gz);
        vTaskDelay(200);
        }
        
    }
    
    
    
    
}

/*!
 * @ Résumé Cette fonction permet d'initialiser l'interruption
 *
 * @param[in] None
 * 
 *
 * @return None
 * 
 */

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
    
   
    
    
}

 
    
void Task_Motion(void* pvParameters)
{
    (void)pvParameters;
    

    I2C_BMI_Start();
    bmi160Config();
    anyMotionInt_set();
    get_accData();

    
}


/* [] END OF FILE */
