/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "motor.h"
#include "usart_with_zdt.h"
#include "TOF050C.h"
#include "key.h"
#include "state_machine.h"
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DEBUG_INFO  0           //调试�??????????�??????????
uint8_t send_buffer[128]={0};
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	//串口空闲中断
//	if((huart == PC_USART))  //�???测是否为正确通道与是否为串口空闲中断
//	{
//		HAL_GPIO_TogglePin(GREEN_GPIO_Port, GREEN_Pin);
////		global_state = RUNNING;
////		pc_rxFIFO.rxcount = DMA_BUFFER_SIZE - __HAL_DMA_GET_COUNTER(PC_DMA);   //获取数据长度
////		PtrWrite_Change(&pc_rxFIFO);                                              //修改写指针，即改变下�???次接收时缓冲区存储的起始位置
////
//////		memcpy(rxCmd,pc_rxFIFO.buffer[pc_rxFIFO.ptrRead],pc_rxFIFO.rxcount);  //将缓冲区的数据用读指针拷贝到接收数组中，长度为rxCount
////
////		Send_Array_Length(&pc_rxFIFO.buffer[pc_rxFIFO.ptrRead],PC_USART,pc_rxFIFO.rxcount);
////		PtrRead_Change(&pc_rxFIFO);                                      //改变读指针，即改变下�???次读取时拷贝的起始位�???
//////		memset(usart_bluetooth.receive_buffer,0,data_length);
////		pc_rxFIFO.rxcount = 0;                                            //全局变量设置�???0，不影响下一次的读取与接�???
////		rxFrameFlag = true;
////		Send_sprintf(PC_USART, "from pc:%d\n",send_buffer[0]);
//		HAL_UARTEx_ReceiveToIdle_DMA(PC_USART,& pc_rxFIFO.buffer[pc_rxFIFO.ptrWrite], DMA_BUFFER_SIZE);  //重新�???始接�???
////		HAL_UARTEx_ReceiveToIdle_DMA(PC_USART,  send_buffer, DMA_BUFFER_SIZE);  //重新�???始接�???
//	   	__HAL_DMA_DISABLE_IT( PC_DMA,DMA_IT_HT);//关闭传输过半中断
////
//	}
	//串口空闲中断
	if((huart == PC_USART))  //�???测是否为正确通道与是否为串口空闲中断
	{
		HAL_GPIO_TogglePin(GREEN_GPIO_Port, GREEN_Pin);
		global_state = RUNNING;
		rxFIFO.rxcount = DMA_BUFFER_SIZE - __HAL_DMA_GET_COUNTER(PC_DMA);   //获取数据长度

		memcpy(&rxFIFO.buffer[rxFIFO.ptrWrite],send_buffer,rxFIFO.rxcount);  //将缓冲区的数据用读指针拷贝到接收数组中，长度为rxCount
		PtrWrite_Change(&rxFIFO);                                              //修改写指针，即改变下�???次接收时缓冲区存储的起始位置
		memset(send_buffer,0x00,128);
		HAL_UARTEx_ReceiveToIdle_DMA(PC_USART, send_buffer, DMA_BUFFER_SIZE);  //重新�???始接�???
	   	__HAL_DMA_DISABLE_IT( PC_DMA,DMA_IT_HT);//关闭传输过半中断

	}
}
void TOF_start(void)
{
	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
	  HAL_Delay(1000);

	  while(!VL6180X_Init())
	  {
		  Send_sprintf(PC_USART,"test!\n");
		  HAL_Delay(100);
	  }
	  Send_sprintf(PC_USART,"initial successful!\n");
	   VL6180X_ConfigureDefault();
	   Send_sprintf(PC_USART,"test2!\n");
	   HAL_Delay(2);
	    VL6180X_SetScaling(2);
	    Send_sprintf(PC_USART,"test begin!\n");
}

//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//	//在定时器中轮询按�??????????
//	if(htim == (&htim2))
//	{
//		  my_key_scan();
//	}
//
//}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
//	int i = 0;
//	uint16_t range = 0;
//	uint16_t real_range = 0;


  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM1_Init();
  MX_I2C1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_ADC1_Init();
  MX_I2C2_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
   HAL_TIM_Base_Start_IT(&htim2);
   Encoder_Init();
   my_key_init();
   motor_timer_init();

   HAL_UARTEx_ReceiveToIdle_DMA(PC_USART,&send_buffer[0] , DMA_BUFFER_SIZE);
   	__HAL_DMA_DISABLE_IT(PC_DMA,DMA_IT_HT);//关闭传输过半中断

//    HAL_UARTEx_ReceiveToIdle_DMA(&huart1,&send_buffer[0] , DMA_BUFFER_SIZE);
//    	__HAL_DMA_DISABLE_IT( &hdma_usart1_rx,DMA_IT_HT);//关闭传输过半中断
//   receive_usart_DMA(PC_USART,PC_DMA,&pc_rxFIFO);
//   receive_usart_DMA( MOTOR_USART, MOTOR_DMA,&rxFIFO);
//   receive_usart_DMA(PC_USART,PC_DMA,send_buffer);
//   receive_usart_DMA( MOTOR_USART, MOTOR_DMA,send_buffer);

  state_machine_init();  //打开状�?�机
//


  TOF_start();     //打开TOF



//
//    VL6180X_Offset_Calibration();
//    HAL_Delay(2000);
//    VL6180X_Offset_Calibration();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

//	  position = Read_ProcessedEncoder();
//	  Send_sprintf("%d\n",position);
//	  range = VL6180X_ReadRangeSingleMillimeters();
//	  Send_sprintf("%d\n",range);
//	  Rotate_Givenpulse(3200);
//	  HAL_Delay(1000);
//	  Rotate_Givenpulse(-3200);
//	  HAL_Delay(2000);
//	  Send_sprintf("%d\n",global_state);
//	  global_state = RUNNING;


	 my_state_machine[global_state].my_func();

	         //�?????????100ms读取�?????????次TOF050C的�??
	  	   //�?????????100ms进行�?????????次pid调试
//	  		uint32_t now_time = 0;
//	  		static uint32_t last_time = 0;
//	  		now_time = HAL_GetTick();   //得到现在的时�?????????
////	  		Send_sprintf("%d,%d\n",now_time,last_time);
//	  		if((now_time - last_time)>= 100)
//	  		{
//	  			range = VL6180X_ReadRangeSingleMillimeters();
//
//	  			pid_control_motor(range,60);
//	  			Send_sprintf("%d,%d\n",range,my_pid.error);
//	  			last_time = HAL_GetTick();
//	  		}


#if(DEBUG_INFO == 1)
	  Key_CallbackFunc();
	  range = VL6180X_ReadRangeSingleMillimeters();
      real_range = filter(range);
      Send_sprintf("%d, %d\n",real_range,range);
      HAL_Delay(100);
#endif
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
