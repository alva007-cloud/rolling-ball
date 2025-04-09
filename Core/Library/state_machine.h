/*
 * state_machine.h
 *
 *  Created on: Apr 1, 2025
 *      Author: Alva
 */

#ifndef LIBRARY_STATE_MACHINE_H_
#define LIBRARY_STATE_MACHINE_H_

#include "pid.h"
#include "Encoder.h"
#include "usart_with_zdt.h"
#include "ADC_pid.h"
#define TOTAL_EVENT_NUM 5             //状态机总的事件数量
#define PC_MODE 0
typedef enum
{
	EVENT_IDLE = 0,         //空闲事件          0
	OFFSET_CALIBRATION ,   //偏移校准           1
	CRSTLK_CALIBRATION,     //串扰校准          2
	MOTOR_RESET_POS,     //电机归零，选择新的位置 3
	SPEED_PID_SET,         //pid速度选择
	POS_PID_SET,          //pid位置选择
	RUNNING,                  //运作            4

}STATE;   //状态相关的枚举量

typedef void (*callback_f)(void);  //回调函数的函数指针
extern volatile STATE global_state;   //状态值全局变量

struct state_machine
{
	callback_f my_func;
	STATE my_state;
};  //状态机结构体，包含其对应的状态和对应的回调函数

extern struct state_machine my_state_machine[TOTAL_EVENT_NUM];

void state_machine_init(void);
#endif /* LIBRARY_STATE_MACHINE_H_ */
