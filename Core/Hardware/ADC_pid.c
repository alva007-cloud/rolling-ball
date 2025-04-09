/*
 * ADC_pid.c
 *
 *  Created on: Apr 5, 2025
 *      Author: Alva
 */
#include "ADC_pid.h"

volatile uint16_t ADC_speed_buffer[3]={0};  //ADC的缓冲区
volatile uint16_t ADC_pos_buffer[3]={0};  //ADC的缓冲区
/*
 * 硬件层代码，用于从电位器中得到数据
 */
void ADC_COM(void)
{
	HAL_ADCEx_Calibration_Start(MY_ADC_HANDLER);//ADC自动校准
	HAL_Delay(200);//延时200ms
	HAL_ADC_Start_DMA(MY_ADC_HANDLER,(uint32_t*)ADC_buffer,3); //启动ADC多通道和DMA
}


