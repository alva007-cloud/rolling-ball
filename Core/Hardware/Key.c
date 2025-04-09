/*
 * Key.c
 *
 *  Created on: Mar 29, 2025
 *      Author: Alva
 */

#include "Key.h"

#define OFFSET_CALIBRATION_FLAG 0
#define CRSTLK_CALIBRATION_FLAG 1
#define FROZEN 2                  //按键被冻结，防止误触



bool read_key1(void)  // 读按键1管脚电平
{
	bool is_press = true;
	is_press = (HAL_GPIO_ReadPin(SWITCH_GPIO_Port, SWITCH_Pin)? true : false);  // 此处添加自己的读管脚函数
	return is_press;
}

bool read_key2(void)  // 读按键1管脚电平
{
	return HAL_GPIO_ReadPin(POSITION_GPIO_Port, POSITION_Pin)? true : false;  // 此处添加自己的读管脚函数
}

 my_key_param_t key_param[KEY_NUMBER] = {0};  // 按键参数结构体数组

void key0_single_click_callback(void)
{
	//回调函数，一些步骤
	static uint8_t i = 0;    //标志位
	switch(i)
	{
	  case OFFSET_CALIBRATION_FLAG:
		  global_state = OFFSET_CALIBRATION;
	  break;

	  case CRSTLK_CALIBRATION_FLAG:
		  global_state = CRSTLK_CALIBRATION;
	  break;


	  default:
	  break;

	}
	if(i >= 1)i = 2;
	else i = (i+1)%2;



}

void key0_double_click_callback(void)
{
  //回调函数，一些步骤

}

void key0_long_press_callback(void)
{
	//长按
	static uint8_t i = 0;
	switch(i)
	{
	  case 0:
			global_state = MOTOR_RESET_POS;
			//一些电机复位代码
	  break;

	  case 1:
		  global_state = SPEED_PID_SET;
		  //速度环pid参数选择
	  break;

	  case 2:
		  global_state = POS_PID_SET;
		  //位置环Pid参数选择
	  break;

	  case 3:
		  global_state = RUNNING;
	  break;

	  default:
	  break;

	}

	i = (i+1)%4;


}

void key0_long_release_callback(void)
{
	return 0;
}

void key0_short_long_press_callback(void)
{
	return 0;
}

void key0_short_long_release_callback(void)
{
	return 0;
}

void key0_long_long_press_callback(void)
{
	return 0;
}

void key0_long_long_release_callback(void)
{
	return 0;
}

void key1_single_click_callback(void)
{
    //在这里读取目标值，单击代表可以进入运行状态
	global_state = RUNNING;
	my_pid.target = Read_ProcessedEncoder();   //将编码器选取到的目标值存储在my_pid.target变量中
}

void key1_double_click_callback(void)
{

}

void key1_long_press_callback(void)
{

}

void key1_long_release_callback(void)
{
	printf("key1_long_release_callback\r\n");
}

void key1_short_long_press_callback(void)
{
	printf("key1_short_long_press_callback\r\n");
}

void key1_short_long_release_callback(void)
{
	printf("key1_short_long_release_callback\r\n");
}

void key1_long_long_press_callback(void)
{
	printf("key1_long_long_press_callback\r\n");
}

void key1_long_long_release_callback(void)
{
	printf("key1_long_long_release_callback\r\n");
}

void my_key_init(void)  // 按键初始化
{
	key_param[0].read_key = read_key1;
	key_param[0].key_is_press = 0;
	key_param[0].count = 0;
	key_param[0].state = KEY_STATE_IDLE;
	key_param[0].event = KEY_EVENT_IDLE;
	key_param[0].single_click_callback = key0_single_click_callback;
	key_param[0].double_click_callback = key0_double_click_callback;
	key_param[0].long_press_callback = key0_long_press_callback;
//	key_param[0].long_release_callback = key0_long_release_callback;
//	key_param[0].short_long_press_callback = key0_short_long_press_callback;
//	key_param[0].short_long_release_callback = key0_short_long_release_callback;
//	key_param[0].long_long_press_callback = key0_long_long_press_callback;
//	key_param[0].long_long_release_callback = key0_long_long_release_callback;      //注释掉不需要的，减少资源占用

	key_param[1].read_key = read_key2;
	key_param[1].key_is_press = 0;
	key_param[1].count = 0;
	key_param[1].state = KEY_STATE_IDLE;
	key_param[1].event = KEY_EVENT_IDLE;
	key_param[1].single_click_callback = key1_single_click_callback;
	key_param[1].double_click_callback = key1_double_click_callback;
	key_param[1].long_press_callback = key1_long_press_callback;
	key_param[1].long_release_callback = key1_long_release_callback;
//	key_param[1].short_long_press_callback = key1_short_long_press_callback;
//	key_param[1].short_long_release_callback = key1_short_long_release_callback;
//	key_param[1].long_long_press_callback = key1_long_long_press_callback;
//	key_param[1].long_long_release_callback = key1_long_long_release_callback;
}

/*
 * @brief 创建一个10ms的定时器，将这个函数放在定时器更新中断中使用，在中断中进行判断并改变事件标志
 */
void my_key_scan(void)  // 按键扫描
{
	for (uint8_t i = 0; i < KEY_NUMBER; i++)  // 轮询按键
	{
		if (key_param[i].read_key == NULL) continue;

		key_param[i].key_is_press = !key_param[i].read_key();  // 获取按键状态

		// 扫描按键状态
		switch (key_param[i].state)  // 进入状态机流程
		{

			case KEY_STATE_IDLE:  // 按键空闲状态
				if (key_param[i].key_is_press)  // 若按键被按下
				{
					key_param[i].count = 0;  // 计数清零
					key_param[i].state = KEY_STATE_FIRST_PRESS;  // 跳转到按键第一次按下状态
				}
				else
				{
					key_param[i].count = 0;  // 计数清零
					key_param[i].state = KEY_STATE_IDLE;  // 跳转到按键空闲状态
					key_param[i].event = KEY_EVENT_IDLE;  // 按键空闲事件
				}
				break;

			case KEY_STATE_FIRST_PRESS:  // 按键第一次按下状态
				if (key_param[i].key_is_press)  // 若按键被按下
				{
					if (++key_param[i].count >= KEY_SHORT_PRESS_COUNT_NUMBER)  // 若第一次按下的时间计数超过KEY_SHORT_PRESS_COUNT_NUMBER
					{
						key_param[i].count = 0;  // 计数清零
						key_param[i].state = KEY_STATE_FIRST_PRESS_VALID;  // 跳转到按键第一次按下有效状态
					}
				}
				else
				{
					// 按下时间不够，不处理，按键消抖
					key_param[i].count = 0;  // 计数清零
					key_param[i].state = KEY_STATE_IDLE;  // 跳转到按键空闲状态
					key_param[i].event = KEY_EVENT_IDLE;  // 按键空闲事件
				}
				break;

			case KEY_STATE_FIRST_PRESS_VALID:	// 按键第一次按下有效状态
				if (key_param[i].key_is_press)  // 若按键被按下
				{
					if (++key_param[i].count >= KEY_LONG_PRESS_COUNT_NUMBER - KEY_SHORT_PRESS_COUNT_NUMBER)  // 若第一次按下的时间计数超过KEY_LONG_PRESS_COUNT_NUMBER
					{
						key_param[i].count = 0;  // 计数清零
						key_param[i].state = KEY_STATE_LONG_PRESS;  // 跳转到按键长按状态
						key_param[i].event = KEY_EVENT_LONG_PRESS;  // 按键长按事件
					}
				}
				else
				{
					key_param[i].count = 0;  // 计数清零
					key_param[i].state = KEY_STATE_FIRST_RELEASE;  // 跳转到按键第一次释放状态
				}
				break;

			case KEY_STATE_FIRST_RELEASE:  // 按键第一次释放状态
				if (!key_param[i].key_is_press)  // 若按键释放
				{
					if (++key_param[i].count >= KEY_DOUBLE_CLICK_INTERVAL_COUNT_NUMBER)  // 若超过KEY_DOUBLE_CLICK_INTERVAL_COUNT_NUMBER计数时间，按键没有第二次被按下
					{
						key_param[i].count = 0;  // 计数清零
						key_param[i].state = KEY_STATE_IDLE;  // 跳转到按键空闲状态
						key_param[i].event = KEY_EVENT_SINGLE_CLICK;  // 按键单击事件
					}
				}
				else if (key_param[i].count >= KEY_SHORT_PRESS_COUNT_NUMBER && key_param[i].count < KEY_DOUBLE_CLICK_INTERVAL_COUNT_NUMBER)  // 若在间隔时间内，获取了第二次按键按下
				{
					key_param[i].count = 0;  // 计数清零
					key_param[i].state = KEY_STATE_SECOND_PRESS;  // 跳转到按键第二次按下状态
				}
				else if (key_param[i].count < KEY_SHORT_PRESS_COUNT_NUMBER) // 若在KEY_SHORT_PRESS_COUNT_NUMBER计数时间内，获取了第二次按键按下，那么此次动作忽略，做消抖处理
				{
					key_param[i].count = 0;  // 计数清零
					key_param[i].state = KEY_STATE_IDLE;  // 跳转到按键空闲状态
					key_param[i].event = KEY_EVENT_SINGLE_CLICK;  // 按键单击事件
				}
				break;

			case KEY_STATE_SECOND_PRESS:  // 按键第二次按下状态
				if (key_param[i].key_is_press)  // 若按键被按下
				{
					if (++key_param[i].count >= KEY_SHORT_PRESS_COUNT_NUMBER)  // 若第一次按下的时间计数超过KEY_SHORT_PRESS_COUNT_NUMBER
					{
						key_param[i].count = 0;  // 计数清零
						key_param[i].state = KEY_STATE_SECOND_PRESS_VALID;  // 跳转到按键第二次按下有效状态
					}
				}
				else
				{
					// 按下时间不够，不处理，按键消抖
					key_param[i].count = 0;  // 计数清零
					key_param[i].state = KEY_STATE_IDLE;  // 跳转到按键空闲状态
					key_param[i].event = KEY_EVENT_SINGLE_CLICK;  // 按键单击事件
				}
				break;

			case KEY_STATE_SECOND_PRESS_VALID:  // 按键第二次按下有效状态
				if (key_param[i].key_is_press)  // 若按键被按下
				{
					if (++key_param[i].count >= KEY_LONG_PRESS_COUNT_NUMBER - KEY_SHORT_PRESS_COUNT_NUMBER)  // 若第二次按下的时间计数超过KEY_LONG_PRESS_COUNT_NUMBER
					{
						key_param[i].count = 0;  // 计数清零
						key_param[i].state = KEY_STATE_SHORT_LONG_PRESS;  // 跳转到按键短长按状态
						key_param[i].event = KEY_EVENT_SHORT_LONG_PRESS;  // 按键短长按事件
					}
				}
				else
				{
					key_param[i].count = 0;  // 计数清零
					key_param[i].state = KEY_STATE_IDLE;  // 跳转到按键空闲状态
					key_param[i].event = KEY_EVENT_DOUBLE_CLICK;  // 按键双击事件
				}
				break;

			case KEY_STATE_SECOND_RELEASE:  // 按键第二次释放状态
				// Do nothing!
				break;

			case KEY_STATE_LONG_PRESS:
				if (key_param[i].key_is_press)  // 若按键被按下
				{
					if (++key_param[i].count >= KEY_LONG_LONG_PRESS_COUNT_NUMBER - KEY_LONG_PRESS_COUNT_NUMBER)  // 若长按下的时间计数超过KEY_LONG_LONG_PRESS_COUNT_NUMBER
					{
						key_param[i].count = 0;  // 计数清零
						key_param[i].state = KEY_STATE_LONG_LONG_PRESS;  // 跳转到按键超长按状态
						key_param[i].event = KEY_EVENT_LONG_LONG_PRESS;  // 按键超长按事件
					}
				}
				else
				{
					key_param[i].count = 0;  // 计数清零
					key_param[i].state = KEY_STATE_LONG_RELEASE;  // 跳转到按键长按释放状态
					key_param[i].event = KEY_EVENT_LONG_RELEASE;  // 按键长按释放事件
				}
				break;

			case KEY_STATE_LONG_RELEASE:
				key_param[i].count = 0;  // 计数清零
				key_param[i].state = KEY_STATE_IDLE;  // 跳转到按键空闲状态
				key_param[i].event = KEY_EVENT_IDLE;  // 按键空闲事件
				break;

			case KEY_STATE_SHORT_LONG_PRESS:
				if (key_param[i].key_is_press)  // 若按键被按下
				{
					// 短长按后，不再做处理，等待按键释放
					// Do nothing!
				}
				else
				{
					key_param[i].count = 0;  // 计数清零
					key_param[i].state = KEY_STATE_SHORT_LONG_RELEASE;  // 跳转到按键短长按释放状态
					key_param[i].event = KEY_EVENT_SHORT_LONG_RELEASE;  // 按键短长按释放事件
				}
				break;

			case KEY_STATE_SHORT_LONG_RELEASE:
				key_param[i].count = 0;  // 计数清零
				key_param[i].state = KEY_STATE_IDLE;  // 跳转到按键空闲状态
				key_param[i].event = KEY_EVENT_IDLE;  // 按键空闲事件
				break;

			case KEY_STATE_LONG_LONG_PRESS:
				if (key_param[i].key_is_press)  // 若按键被按下
				{
					// 超长按后，不再做处理，等待按键释放
					// Do nothing!
				}
				else
				{
					key_param[i].count = 0;  // 计数清零
					key_param[i].state = KEY_STATE_LONG_LONG_RELEASE;  // 跳转到按键超长按释放状态
					key_param[i].event = KEY_EVENT_LONG_LONG_RELEASE;  // 按键超长按释放事件
				}
				break;

			case KEY_STATE_LONG_LONG_RELEASE:
				key_param[i].count = 0;  // 计数清零
				key_param[i].state = KEY_STATE_IDLE;  // 跳转到按键空闲状态
				key_param[i].event = KEY_EVENT_IDLE;  // 按键空闲事件
				break;
		}

		// 扫描按键事件
		switch (key_param[i].event)  // 进入状态机流程
		{
			case KEY_EVENT_IDLE:
				// Do nothing!
				break;

			case KEY_EVENT_SINGLE_CLICK:
				if (key_param[i].single_click_callback != NULL) key_param[i].single_click_callback();
				break;

			case KEY_EVENT_DOUBLE_CLICK:
				if (key_param[i].double_click_callback != NULL) key_param[i].double_click_callback();
				break;

			case KEY_EVENT_LONG_PRESS:
				if (key_param[i].long_press_callback != NULL) key_param[i].long_press_callback();
				break;

			case KEY_EVENT_LONG_RELEASE:
				if (key_param[i].long_release_callback != NULL) key_param[i].long_release_callback();
				break;

			case KEY_EVENT_SHORT_LONG_PRESS:
				if (key_param[0].short_long_press_callback != NULL) key_param[0].short_long_press_callback();
				break;

			case KEY_EVENT_SHORT_LONG_RELEASE:
				if (key_param[0].short_long_release_callback != NULL) key_param[0].short_long_release_callback();
				break;

			case KEY_EVENT_LONG_LONG_PRESS:
				if (key_param[0].long_long_press_callback != NULL) key_param[0].long_long_press_callback();
				break;

			case KEY_EVENT_LONG_LONG_RELEASE:
				if (key_param[0].long_long_release_callback != NULL) key_param[0].long_long_release_callback();
				break;
		}

		key_param[i].event = KEY_EVENT_IDLE;  // 清除按键事件
	}
}



void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	//在定时器中轮询按�??
	if(htim == (&htim2))
	{

		  my_key_scan();
	}

}
