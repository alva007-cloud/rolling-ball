/*
 * usart_with_zdt.c
 *
 *  Created on: Mar 22, 2025
 *      Author: Alva
 */

#include "usart_with_zdt.h"

FIFO_t rxFIFO =
{


		.ptrRead = 0,
		.ptrWrite = 0,
		.rxcount = 0,
};
__IO bool rxFrameFlag = false;
 FIFO_t pc_rxFIFO =
{


		.ptrRead = 0,
		.ptrWrite =0,
		.rxcount = 0,
};
 char rxCmd[FIFO_SIZE] = {0};   //接收数组
/*=====================================数据串口发送底层驱动代码=========================================================*/
/*
 * 发送一个字节
 */
void Send_Byte(const uint8_t Byte,UART_HandleTypeDef *huart)
{

	HAL_StatusTypeDef usart_result = HAL_UART_Transmit(huart, &Byte, 1, 10000);   //timeout单位是毫秒
    if (usart_result!= HAL_OK)  //这里抄下来是不等于，有点奇怪
    {
	  printf("error:%d\n",usart_result);
      Error_Handler();
    }
}

void Send_Array(const uint8_t *usart_array,UART_HandleTypeDef *huart)
{
	__IO uint8_t i = 0;
	uint8_t Size = sizeof(usart_array)/ sizeof(usart_array[0]);
	for(;i<Size;i++)
	{
		Send_Byte(usart_array[i],huart);
	}

}

void Send_Array_Length(const uint8_t *usart_array,UART_HandleTypeDef *huart,uint8_t size)
{
	__IO uint8_t i = 0;

	for(;i<size;i++)
	{
		Send_Byte(usart_array[i],huart);
	}

}
/*
 * 发送一串字符串
 */
void Send_String(const uint8_t *string,UART_HandleTypeDef *huart)
{
	uint32_t i = 0;
	if(string == NULL)
	{
		return;               //指针判空
	}
	while(string[i]!= '\0')
	{
		Send_Byte(string[i],huart);
		i++;
	}
}
/*
 * @brief printf重定向，这样可以通过printf来将信息打印到串口助手上
 * 不过这一个是定串口号的，不通用
 */
int fputc(int ch, FILE *f)
{
	Send_Byte(ch,PC_USART);
	return ch;
}

/*
 * @brief 可以通过串口打印任意格式的字符,和上面重定向后的printf的最终效果相同
 */
void Send_sprintf(UART_HandleTypeDef *huart,char *format,...)
{
	uint8_t string[256];
	va_list arg;
	va_start(arg,format);
	vsprintf(string,format,arg);
	va_end(arg);
	Send_String(string,huart);

}
/*========================================================串口接收数据========================================================*/

void receive_usart_DMA(UART_HandleTypeDef *huart,DMA_HandleTypeDef *hdma,FIFO_t *read_buffer)
{
	__HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);
	HAL_UARTEx_ReceiveToIdle_DMA(huart, &(read_buffer->buffer[read_buffer->ptrWrite]), DMA_BUFFER_SIZE);
	__HAL_DMA_DISABLE_IT(hdma,DMA_IT_HT);//关闭传输过半中断
}

 void PtrWrite_Change(__IO FIFO_t *read_buffer)
{
	 Send_sprintf(PC_USART, "raw write:%d\n",read_buffer->ptrWrite);
	read_buffer->ptrWrite += read_buffer->rxcount;
	read_buffer->ptrWrite %= FIFO_SIZE;                   //不断改变写指针的位置，取余避免越界
}
void PtrRead_Change(__IO FIFO_t *read_buffer)
{
	read_buffer->ptrRead += read_buffer->rxcount;
	read_buffer->ptrRead %= FIFO_SIZE;                   //不断改变写指针的位置，取余避免越界
}

/*
 * @brief:清空缓存区
 */
void Buffer_Clean(FIFO_t *read_buffer)
{
	memset(read_buffer->buffer,0x00,FIFO_SIZE);
	read_buffer->ptrWrite = 0;
	read_buffer->ptrRead = 0;
}


