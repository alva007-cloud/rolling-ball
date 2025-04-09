/*
 * usart_with_zdt.h
 *
 *  Created on: Mar 22, 2025
 *      Author: Alva
 */

#ifndef INC_USART_WITH_ZDT_H_
#define INC_USART_WITH_ZDT_H_

#include "usart.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

#define FIFO_SIZE  2000             //缓冲区大小
//#define MOTOR_USART &huart3
#define PC_USART &huart1
//#define MOTOR_DMA  &hdma_usart3_rx
#define PC_DMA &hdma_usart1_rx

#define TIMEOUT 8000
#define DMA_BUFFER_SIZE 1000




typedef struct {
	uint8_t buffer[FIFO_SIZE];
	__IO uint8_t ptrWrite;
	__IO uint8_t ptrRead;
//	__IO uint8_t buffer_rest;
	__IO uint8_t rxcount;
}FIFO_t;                             //FIFO缓冲区结构体

extern  FIFO_t rxFIFO;
extern __IO bool rxFrameFlag;
extern FIFO_t pc_rxFIFO ;    //FIFO缓冲区结构体
extern  char rxCmd[FIFO_SIZE];   //接收数组

void Send_Byte(const uint8_t Byte,UART_HandleTypeDef *huart);
void Send_Array(const uint8_t *usart_array,UART_HandleTypeDef *huart);
void Send_Array_Length(const uint8_t *usart_array,UART_HandleTypeDef *huart,uint8_t size);
void Send_String(const uint8_t *string,UART_HandleTypeDef *huart);
void Send_sprintf(UART_HandleTypeDef *huart,char *format,...);
void receive_usart_DMA(UART_HandleTypeDef *huart,DMA_HandleTypeDef *hdma,FIFO_t *read_buffer);
void PtrWrite_Change(__IO FIFO_t *read_buffer);
void PtrRead_Change(__IO FIFO_t *read_buffer);
void Buffer_Clean(FIFO_t *read_buffer);
#endif /* INC_USART_WITH_ZDT_H_ */
