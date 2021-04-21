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

/*!
 * @Résumé Cette fonction permet de configurer les
 * différentes registres du capteur MAX30102.
 *
 * @param[in] None
 *
 * @return None
 */
void MAX30102_config()
{
CyDelay(100);    
    
writeRegistre(REG_INTR_ENABLE_1, 0x40);         //New FIFO Data Ready : ON //40
        
writeRegistre(REG_INTR_ENABLE_2, 0x00); 

writeRegistre(REG_FIFO_WR,0x00);

writeRegistre(REG_OVFLOW_COUNTER, 0x00);

writeRegistre(REG_FIFO_RD,0x00);
        
writeRegistre(REG_FIFO_CONFIG, 0x00);          //No sample averaging ; FIFO_ROLLOVER : Enable ; 15 empty data in FIFO when interrupt is issued //1f
        
writeRegistre(REG_MODE_CONFIG, 0x03);          //SPO2 Mode (Red and IR)
       
writeRegistre(REG_SPO2_CONFIG, 0x6B);          // LSB SIZE = 62.5 pA and FULL SCALE = 16384 nA ; Sample Rate = 200Hz ; Pulse width = 411 micosec and ADC resolution = 18 bits

writeRegistre(REG_LED_AMP_1, 0xAF);            //35 mA

writeRegistre(REG_LED_AMP_2, 0xAF);            //35mA


}    

/*!
 * @ Résumé Cette fonction permet de lire la valeur contenue
 * dans un registre du MAX30102.
 *
 * @param[in] adresse  : adresse du registre à lire.
 * 
 *
 * @return regValue : la valeur contenue dans le registre
 * 
 */
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

/*!
 * @ Résumé Cette fonction permet d'écrire une valeur dans
 * un registre en particulier du capteur MAX30102
 *
 * @param[in] adresse  : adresse du registre dans lequel on écrit
 * @param[in] data     : valeur à écrire dans le registre
 *
 * @return None
 * 
 */
void writeRegistre(uint8_t adresse, uint8_t data){ 
    
    cy_en_scb_i2c_status_t status;
    
    status = I2C_MAX_MasterSendStart(ADRESSE_MAX,CY_SCB_I2C_WRITE_XFER,I2C_TIMEOUT);
    if(status==CY_SCB_I2C_SUCCESS){
        I2C_MAX_MasterWriteByte(adresse,I2C_TIMEOUT); 
    }
    
    I2C_MAX_MasterWriteByte(data,I2C_TIMEOUT);
    I2C_MAX_MasterSendStop(I2C_TIMEOUT);
    
} 


/*!
 * @ Résumé Cette fonction permet de lire plusieurs bits 
 * dans un même registre
 *
 * @param[in] baseAddress : Adresse du registre dans lequel les valeurs sont lues
 * @param[in] buffer      : Tableau dans lequel les valeurs lues sont stockées
 * @param[in] length      : Nombre de bits que l'on veut lire
 *
 * @return None
 * 
 */
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


/*!
 * @ Résumé Cette fonction permet de lire les données
 * d'absorption de la LED rouge et infrarouge contenues dans
 * le registre FIFO_DATA. Elle stock les 18 premiers bits lus
 * dans le buffer rouge et les 18 prochains dans le buffer infrarouge
 *
 * @param[in] red_LEd  : Tableau dans lequel les valeurs d'absorption de rouge sont stockées
 * @param[in] ir_LED   : Tableau dans lequel les valeurs d'absorption d'infrarouge sont stockées
 * @param[in] compteur : Valeur qui permet d'incrémenter les cases des deux buffers.
 *
 * @return None
 * 
 */
void readFIFO(uint32_t *red_LED, uint32_t *ir_LED, uint16_t compteur){
    //ou remplacer par tableau
    uint8_t i2c_data [6];
        
    readMultipleBytes(REG_FIFO_DATA,i2c_data,6);                                        // Lecture de 6 bytes à la fois. 3 bytes pour le rouge et 3 byte pour l'infrarouge.
    red_LED[compteur]=((0b00000011&i2c_data[0])<<16)+(i2c_data[1]<<8)+(i2c_data[2]);    // Prise des 18 premiers bits pour le RED channel
    ir_LED[compteur]=((0b00000011&i2c_data[3])<<16)+(i2c_data[4]<<8)+(i2c_data[5]);     // Prise des 18 prochains bits pour le IR channel
    
    
    // vérification bonnes données
    char cRed[6];
    itoa(red_LED[compteur], cRed, 10);
    UART_1_PutString(cRed);
    UART_1_PutString("\n\r");
    
    
    
}


/*!
 * @ Résumé Cette fonction permet de changer la valeur 
 * d'intensité de la LED rouge
 *
 * @param[in] ledAmp_red  : Valeur d'intensité de le LED rouge
 * 
 * @return None
 * 
 */
void changeLED_red (short int ledAmp_red)
{
    
    writeRegistre(REG_LED_AMP_1, ledAmp_red); 
  
    
}
    

/*!
 * @ Résumé Cette fonction permet de changer la valeur 
 * d'intensité de la LED infrarouge
 *
 * @param[in] ledAmp_IR  : Valeur d'intensité de le LED infrarouge
 * 
 * @return None
 * 
 */
void changeLED_IR (short int ledAmp_IR)
{
    
    
    writeRegistre(REG_LED_AMP_2, ledAmp_IR); 
    
}
    


/* [] END OF FILE */
