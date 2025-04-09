/*
 * motor.h
 *
 *  Created on: Mar 21, 2025
 *      Author: Alva
 */

#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_
#include "gpio.h"
#include "tim.h"

#define MOTOR_TIM_HANDLE &htim1                  //电机对应的时钟外设的句柄
#define MOTOR_TIM_CHANNEL TIM_CHANNEL_1          //电机对应时钟外设的通道
#define TOGGLE_PERIOD 20                          //周期的一半，这个值越小频率越高(注意频率不可太高，也不可低于1500HZ)
                                                   //本例中频率的计算：f = 72000/2/TOGGLE_PERIOD
#define MOTOR_AUTORELOAD 0xFFFF                    //重装载寄存器的填充值
#define REDUCTION_RATIO 30                           //减速比
typedef enum
{
	CW = 0,    //顺时针
	CCW        //逆时针（相对）
}MOTOR_DIR;  //顺时针逆时针旋转的枚举常量

extern __IO uint16_t pulse_count;    //电机接收的脉冲数= pulse_count/2
void motor_timer_init(void);

uint16_t Read_pulse(void);
void Change_dir(MOTOR_DIR direction);
void Rotate_Givenpulse(int16_t pulse_val);
#endif /* INC_MOTOR_H_ */
