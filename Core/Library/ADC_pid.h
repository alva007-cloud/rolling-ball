/*
 * ADC_pid.h
 *
 *  Created on: Apr 5, 2025
 *      Author: Alva
 */

#ifndef LIBRARY_ADC_PID_H_
#define LIBRARY_ADC_PID_H_
#include "adc.h"

#define MY_ADC_HANDLER &hadc1
#define ADC_NUM 3
extern volatile uint16_t ADC_buffer[3];
extern volatile uint16_t ADC_pos_buffer[3];  //ADC的缓冲区
#endif /* LIBRARY_ADC_PID_H_ */
