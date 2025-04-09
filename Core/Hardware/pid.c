/*
 * pid.c
 *
 *  Created on: Apr 3, 2025
 *      Author: Alva
 */
#include "pid.h"

/**********************************************************
 *                 结构体定义
 ***********************************************************/
struct pid_struct
{
	float kp;
	float ki;
	float kd;
	int16_t error;   //这一次的误差
	int16_t error_last;   //上一次的误差
	int16_t error_last2;   //上上次的误差
	int16_t output;         //output，pid调控输出
	uint8_t target;        //目标值，从编码器得到

};


/**********************************************************
 *                 全局变量定义
 ***********************************************************/

pid my_speed_pid =
{
		.error = 0,
		.error_last = 0,
		.error_last2 = 0,
		.kd = 0,
		.ki = 0,
		.kp = 0,
		.output= 0,
		.target = 0,
};    //全局Pid控制器（如果之后用串级pid的话可以用一个数组）

pid my_position_pid =
{
		.error = 0,
		.error_last = 0,
		.error_last2 = 0,
		.kd = 0,
		.ki = 0,
		.kp = 0,
		.output= 0,
		.target = 0,
};    //全局Pid控制器（如果之后用串级pid的话可以用一个数组）


/**********************************************************
 *                 pid算法函数
 ***********************************************************/


/*
 * @brief 用于pid参数的改变，可以手动在程序中改变，也可以用电位器+ADC随时调控
 */
void change_pidindex(pid * my_pid,float kp,float ki,float kd)
{
	my_pid->kd = kd;
	my_pid->ki = ki;
	my_pid->kp = kp;
}

/*
 * @brief 用于pid算法控制，计算需要反馈给电机的脉冲数
 * @note 在这之前一定要调用change_pidindex()函数或者包含它的函数，否则pid参数不会改变
 *       这只是初级的一阶pid，后面可以视情况进行串级Pid调控或者进行微分先行、输出限制、积分限幅等的操作
 */
void pid_pulse_cal(pid * my_pid,uint8_t range)
{
#if (PC_MODE == 0)

	my_pid->error = my_pid.target - range;  //计算误差

	my_pid->output = my_pid.kp * (my_pid.error - my_pid.error_last) + my_pid.ki*my_pid.error + my_pid.kd * (my_pid.error - 2*my_pid.error_last + my_pid.error_last2);  //和脉冲数相关
	my_pid->output *= REDUCTION_RATIO ;//乘上减速比，保证实际输出的脉冲数足够多

	my_pid->error_last2 = my_pid.error_last;
	my_pid->error_last = my_pid.error;

#else
	my_pid->error =	error;  //计算误差

	my_pid->output = my_pid.kp * (my_pid.error - my_pid.error_last) + my_pid.ki*my_pid.error + my_pid.kd * (my_pid.error - 2*my_pid.error_last + my_pid.error_last2);  //和脉冲数相关
	my_pid->output *= REDUCTION_RATIO ;//乘上减速比，保证实际输出的脉冲数足够多

	my_pid->error_last2 = my_pid.error_last;
	my_pid->error_last = my_pid.error;
#endif
}

/*
 * @brief 正式的pid执行函数，得到了pid的调控量之后输入给电机
 */
void pid_control_motor(pid * my_pid,uint8_t range)
{
	pid_pulse_cal(range);
     Rotate_Givenpulse(my_pid->output);
}

/*****************************************************************************
 * ==============================和ADC部分进行交流==================================
 ****************************************************************************/

/*
 * @brief 定时采样电位器的电压数转换成adc
 */
void change_pid(pid * my_pid,volatile uint16_t *buffer)
{

	  float kp = buffer[0]/4096;
	  float ki = buffer[1]/4096;
	  float kd = buffer[2]/4096;
	  change_pidindex(my_pid,kp,ki,kd);
}





