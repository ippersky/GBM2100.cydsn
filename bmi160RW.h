/* ========================================
 *
 * Contient les informations des adresses et
 * des regsitres du module I2C et du capteur
 * MAX30102
 *
 * ========================================
*/

#include "project.h"

#define ADRESSE_BMI 0x68

static struct bmi160_dev bmi160Dev;