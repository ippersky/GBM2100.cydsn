/* ========================================
 *
 * Contient les informations des adresses et
 * des regsitres du module I2C et du capteur
 * MAX30102
 *
 * ========================================
*/

#include "project.h"
#include "bmi160.h"
#include "bmi160_defs.h"

#define MAXACCEL (32769/2)


//TaskHandle_t xTaskHandleMotion;

void get_accData ();
static void bmi160Config ();
static int8_t bmi160Read(uint8_t reg_adresse, uint8_t *data, uint16_t length);
static int8_t bmi160Write(uint8_t reg_adresse, uint8_t *data, uint16_t length);

