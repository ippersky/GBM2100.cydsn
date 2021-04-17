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
#include <stdlib.h>
#include <core_cm4.h>
#include <arm_math.h>
#include <params.h>

float32_t HeartRate(uint32_t *Signal, uint32_t temps1, uint32_t temps2);
float32_t AC(uint32_t *Signal, uint32_t temps1, uint32_t temps2);
float32_t DC(uint32_t *Signal, uint32_t temps1, uint32_t temps2);
float32_t calculSpO2(uint32_t *InputRed, uint32_t *InputIR, uint32_t temps1, uint32_t temps2);

/* [] END OF FILE */
