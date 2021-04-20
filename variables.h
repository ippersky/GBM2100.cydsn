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
#include "semphr.h"
#include "task.h"

#define BUFFER_LENGTH 1000
#define ON      0u
#define OFF     1u

extern uint32_t red[BUFFER_LENGTH];
extern uint32_t ir[BUFFER_LENGTH];
extern uint32_t filteredRED[BUFFER_LENGTH];
extern uint32_t filteredIR[BUFFER_LENGTH];
extern uint16_t indexBuffer;
extern float32_t BPM;
extern float32_t SPO2;

extern TaskHandle_t xSample;
extern TaskHandle_t xFiltering;
extern TaskHandle_t xResults;
extern uint8_t flagMOUVEMENT;

/* [] END OF FILE */
