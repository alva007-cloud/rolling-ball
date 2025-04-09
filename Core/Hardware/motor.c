/*
 * motor.c
 *
 *  Created on: Mar 21, 2025
 *      Author: Alva
 */
#include "motor.h"



__IO uint16_t pulse_count = 0;          //全局变量，每一次输出比较中断都加一

/*
 * =======================电机驱动函数============================================
 */

/*
 * @brief 电机的初始化函数，一定一定要放在最前面！！！
 */
void motor_timer_init(void)
{
	HAL_TIM_Base_Start(MOTOR_TIM_HANDLE);         //开启时钟
	HAL_TIM_OC_Start_IT(MOTOR_TIM_HANDLE,MOTOR_TIM_CHANNEL);//启动比较输出使能中断
	HAL_GPIO_WritePin(ENA_GPIO_Port, ENA_Pin, GPIO_PIN_RESET);    //电机失能（ENA引脚低电平）
}

/*
 * @brief 输出比较中断回调函数
 */

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
	__IO uint16_t count = __HAL_TIM_GET_COUNTER(MOTOR_TIM_HANDLE);
	uint16_t temp = MOTOR_AUTORELOAD & (count + TOGGLE_PERIOD);   //重新读取counter值并填入比较寄存器中，这样可以消除因为系统
	                                                              //中断响应而额外花费的时间
	__HAL_TIM_SET_COMPARE(MOTOR_TIM_HANDLE,MOTOR_TIM_CHANNEL,temp);
	pulse_count++;    //计算脉冲数的两倍
}


/*
 * ==================================电机转动特定角度====================================
 */
uint16_t Read_pulse(void)
{
	return pulse_count/2;   //返回脉冲数
}


void Change_dir(MOTOR_DIR direction)
{
	HAL_GPIO_WritePin(DIR_GPIO_Port, DIR_Pin, direction);//控制方向
}

/*
 * @brief 让电机旋转特定脉冲数  在滚球控制中使用增量式pid
 * @para pulse_val  希望给到电机的脉冲数（有符号）
 */
void  Rotate_Givenpulse(int16_t pulse_val)
{
	if (pulse_val < 0)
	{
		//逆时针旋转，可以按情况修改
		Change_dir(CCW);
		pulse_val = -1*pulse_val;
	}
	else if(pulse_val > 0)
	{
		//顺时针旋转
		Change_dir(CW);
	}
	pulse_count = 0;
    //电机使能,脉冲数清零
	HAL_GPIO_WritePin(ENA_GPIO_Port, ENA_Pin, GPIO_PIN_SET);

	//开始计数
	while(Read_pulse() < pulse_val){};       //如果未输出完则堵塞在这里不继续往下运行
    //电机失能,脉冲数清零
	HAL_GPIO_WritePin(ENA_GPIO_Port, ENA_Pin, GPIO_PIN_RESET);
	pulse_count = 0;
}


