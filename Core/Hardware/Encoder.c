/*
 * Encoder.c
 *
 *  Created on: Apr 5, 2025
 *      Author: Alva
 */
#include "Encoder.h"

/*
 * @brief 编码器的初始化函数，一定要放在最前面！！！
 */
void Encoder_Init(void)
{
	 HAL_TIM_Encoder_Start(ENCODER_HANDLER, TIM_CHANNEL_ALL);
}
/*
 * @brief 返回编码器计数器得到的值
 */
static uint16_t Read_RawEncoder(void)
{
	return __HAL_TIM_GET_COUNTER(ENCODER_HANDLER);
}

/*
 * @brief 返回经过处理后的值
 *        限制范围从0到160
 */
uint16_t Read_ProcessedEncoder(void)
{
	uint16_t raw = Read_RawEncoder();
	uint16_t processed = raw / 2;
	if(processed > 6000){processed = 0;__HAL_TIM_SET_COUNTER(ENCODER_HANDLER,processed);}
	else if(processed >160){processed = 160;__HAL_TIM_SET_COUNTER(ENCODER_HANDLER,processed);}
	return processed;
}

