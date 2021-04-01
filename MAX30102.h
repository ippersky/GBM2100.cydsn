/* ========================================
 *
 * Contient les informations des adresses et
 * des regsitres du module I2C et du capteur
 * MAX30102
 *
 * ========================================
*/

#include "project.h"


// Adresse fixe du capteur MAX30102
#define ADRESSE_I2C 0x57
#define ADRESSE_I2C_WR 0x57
#define ADRESSE_I2C_RD 0x57

// Registres des différentes composantes du capteur MAX30102

#define REG_INTR_STAT_1 0x00
#define REG_INTR_STAT_3 0x01
#define REG_INTR_ENABLE_1 0x02
#define REG_INTR_ENABLE_2 0x03
#define REG_FIFO_WR 0x04
#define REG_OVFLOW_COUNTER 0x05
#define REG_FIFO_RD 0x06
#define REG_FIFO_DATA 0x07
#define REG_FIFO_CONFIG 0x08
#define REG_MODE_COFIG 0x09
#define REG_SPO2_CONFIG 0x0A
#define REG_LED_AMP_1 0x0C
#define REG_LED_AMP_2 0x0D
#define REG_MULTILED_CNTRL_1 0x11
#define REG_MULTILED_CNTRL_2 0x12
#define REG_DIE_TEMP_INT 0x1F
#define REG_DIE_TEMP_FRAC 0x20
#define REG_DIE_TEMP_CONFIG 0x21
#define REG_REVISION_ID 0xFE
#define REG_PART_ID 0xFF
#define I2C_TIMEOUT (100UL)





/* [] END OF FILE */
