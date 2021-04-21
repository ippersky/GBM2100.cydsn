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

#ifndef SAMPLEFILTER_H_
#define SAMPLEFILTER_H_

/*

FIR filter designed with
 http://t-filter.appspot.com

sampling frequency: 200 Hz

* 0 Hz - 2 Hz
  gain = 1
  desired ripple = 1 dB
  actual ripple = 0.7423241933985565 dB

* 2.5 Hz - 100 Hz
  gain = 0
  desired attenuation = -40 dB
  actual attenuation = -40.11271142937664 dB

*/

#define SAMPLEFILTER_TAP_NUM 619

typedef struct {
  float history[SAMPLEFILTER_TAP_NUM];
  unsigned int last_index;
} SampleFilter;

void SampleFilter_init(SampleFilter* f);
void SampleFilter_put(SampleFilter* f, float input);
float SampleFilter_get(SampleFilter* f);

#endif

/* [] END OF FILE */
