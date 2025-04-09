/*_
 *  state_machine.c
 *
 *  Created on: Apr 1, 2025
 *      Author: Alva
 */
#include "state_machine.h"


struct state_machine my_state_machine[TOTAL_EVENT_NUM];
volatile STATE global_state = EVENT_IDLE;        //可能会在中断中修改

/*
 * =============================================各种任务的时间切片==========================================
 */
/**
  *@功能： 获取2个时间点之间是否达到期望的延时
  *@参数1：现在的实时时间
  *@参数2：开始计时的时间
  *@参数3：要延时的时间间隔
  *@返回值：0-延时时间未到，1-延时时间已到
  */
static uint8_t Get_Time_Interval(uint32_t Current_Time, uint32_t Past_Time, uint32_t Delay_Time)
{
    if(Current_Time < Past_Time)//实时时间小于开始时间，时间计数发生回滚，防止溢出
    {
        if(0xFFFFFFFF - Past_Time + Current_Time >= Delay_Time)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else        //实时时间大于开始时间
    {
        if(Current_Time - Past_Time >= Delay_Time)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
}


/*
 * @ TOF050C读取以及pid调参的时间切片，周期为100ms
 */

static void TOF_pid_proc(void)
{
	uint8_t range = 0;
	float proccess_range = 0;
   //每100ms读取一次TOF050C的值
   //每100ms进行一次pid调试
	static uint32_t Static_Time = 0;

	if(Get_Time_Interval(HAL_GetTick(),Static_Time,100)==1)
	{
	    Static_Time = HAL_GetTick();
	    range = VL6180X_ReadRangeSingleMillimeters();   //实际上容易有高频噪音，且当距离大幅度变化时输出波形会比较生硬
	                                                    //实操发现对range进行低通滤波会平滑一点，可以视到时候的情况来决定是否滤波
	    proccess_range = filter(range);
		pid_control_motor(proccess_range);
		Send_sprintf(PC_USART,"%d,%d\n",range,my_pid.error);


	}

}





/*
 * ==========================状态机的各个回调函数==========================================
 */

void callback_func_0(void)
{
	//空闲状态下的函数指针
	return;    //空闲状态下什么都不用干
}

void callback_func_1(void)
{
	if(global_state == OFFSET_CALIBRATION)
	{//偏移校准
		VL6180X_Offset_Calibration();
		global_state = EVENT_IDLE;
	}
}

void callback_func_2(void)
{
	//串扰校准
	if(global_state == CRSTLK_CALIBRATION)
	{
		VL6180X_CrossTalk_Callibration();
		global_state = EVENT_IDLE;      //校准完后空闲状态，还要确定小球位置，因此不会马上开始运行
	}
}

void callback_func_3(void)
{
	static uint16_t encoder = 0;
	if(global_state == MOTOR_RESET_POS)
	{
		//选择新的位置并确定
		//一些电机归零的代码
		while(global_state != RUNNING)
		{
			encoder = Read_ProcessedEncoder();
			Send_sprintf(PC_USART,"encoder:%d!\n",encoder);
		}    //这里在按键那里进行状态改变，确定按键没按下时堵塞在这里，
											 //当确定按键按下之后就把标志位设为RUNNING，跳出死循环
											//在这里记下设定好的位置
		Send_sprintf(PC_USART,"target:%d!\n",encoder);
		my_pid.target = encoder;
		Send_sprintf(PC_USART,"start!\n");
	}
}

void callback_func_4(void)
{
	//开始运行，这里可以运用时间切片
	if(global_state == RUNNING)
	{

#if (PC_MODE == 1)
		Send_sprintf(PC_USART,"pid\n");
//		pid_control_motor(rxCmd[4]);
		memcpy(rxCmd,&rxFIFO.buffer[rxFIFO.ptrWrite],rxFIFO.rxcount);
		PtrRead_Change(&rxFIFO);               //改变读指针，即改变下�?次读取时拷贝的起始位�?

		pid_control_motor(rxCmd[0]);

		global_state = EVENT_IDLE;
		memset(rxCmd,0x00,FIFO_SIZE);
#else if (PC_MODE == 0)

		TOF_pid_proc();
#endif
	}

}

void callback_func_5(void)
{
	if(global_state == SPEED_PID_SET)
	{
		while(global_state == SPEED_PID_SET)
		{
			Send_sprintf(PC_USART,"speed,kp:%.1f,ki:%.1f,kd:%.1f\n",ADC_buffer[0],ADC_buffer[1],ADC_buffer[2]);
		}
		change_pid(&my_speed_pid,ADC_speed_buffer);
		Send_sprintf(PC_USART,"speed,kp:%.1f,ki:%.1f,kd:%.1f\n",ADC_buffer[0],ADC_buffer[1],ADC_buffer[2]);
	}
}

void callback_func_6(void)
{
	if(global_state == POS_PID_SET)
	{
		while(global_state == POS_PID_SET)
		{
			Send_sprintf(PC_USART,"position,kp:%.1f,ki:%.1f,kd:%.1f\n",ADC_buffer[0],ADC_buffer[1],ADC_buffer[2]);
		}
		change_pid(&my_position_pid,ADC_pos_buffer);
		Send_sprintf(PC_USART,"speed,kp:%.1f,ki:%.1f,kd:%.1f\n",ADC_buffer[0],ADC_buffer[1],ADC_buffer[2]);
	}
}

/*
 * @brief 初始化函数，放在主函数的循环之前初始化
 */
void state_machine_init(void)
{

		my_state_machine[0].my_func = callback_func_0;
		my_state_machine[0].my_state = EVENT_IDLE;

		my_state_machine[1].my_func = callback_func_1;
		my_state_machine[1].my_state = OFFSET_CALIBRATION;

		my_state_machine[2].my_func = callback_func_2;
		my_state_machine[2].my_state = CRSTLK_CALIBRATION;

		my_state_machine[3].my_func = callback_func_3;
		my_state_machine[3].my_state = MOTOR_RESET_POS;

		my_state_machine[4].my_func = callback_func_4;
		my_state_machine[4].my_state = RUNNING;

		my_state_machine[5].my_func = callback_func_5;
		my_state_machine[5].my_state = SPEED_PID_SET;

		my_state_machine[6].my_func = callback_func_6;
		my_state_machine[6].my_state = POS_PID_SET;
}




