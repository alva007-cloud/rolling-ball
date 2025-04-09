/*
 * Encoder.h
 *
 *  Created on: Apr 5, 2025
 *      Author: Alva
 *      note: 这个文件用于编码器的驱动和读数。在本项目 中通过编码器进行目标距离的设定
 */

#ifndef LIBRARY_ENCODER_H_
#define LIBRARY_ENCODER_H_
#include "tim.h"

#define ENCODER_HANDLER &htim3

void Encoder_Init(void);
uint16_t Read_ProcessedEncoder(void);

#endif /* LIBRARY_ENCODER_H_ */
