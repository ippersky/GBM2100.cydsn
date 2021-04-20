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
#include "variables.h"

uint32_t red[BUFFER_LENGTH];
uint32_t ir[BUFFER_LENGTH];
uint32_t filteredRED[BUFFER_LENGTH];
uint32_t filteredIR[BUFFER_LENGTH];
uint16_t indexBuffer = 0;
float32_t BPM;
float32_t SPO2;

TaskHandle_t xSample;
TaskHandle_t xFiltering;
TaskHandle_t xResults;
uint8_t flagMOUVEMENT;

/* [] END OF FILE */

