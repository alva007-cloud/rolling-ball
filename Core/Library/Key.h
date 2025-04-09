/*
 * Key.h
 *
 *  Created on: Mar 29, 2025
 *      Author: Alva
 */
#include <stdbool.h>
#include "gpio.h"
#include "usart.h"
#include "state_machine.h"
// 在10ms的定时器中进行计数
#define KEY_SHORT_PRESS_COUNT_NUMBER				5	 // 短按时间  50ms
#define KEY_DOUBLE_CLICK_INTERVAL_COUNT_NUMBER		15  // 双击间隔   150ms
#define KEY_LONG_PRESS_COUNT_NUMBER					100  // 长按时间  1000ms
#define KEY_LONG_LONG_PRESS_COUNT_NUMBER			800  // 超长按时间  8000ms

typedef bool (* my_read_key_t)(void);               //函数指针
typedef void (* my_key_callback_t)(void);           //函数指针

#define KEY_NUMBER									2  // 按键个数

// 按键状态
typedef enum
{
	KEY_STATE_IDLE,					// 按键空闲状态
	KEY_STATE_FIRST_PRESS,			// 按键第一次按下状态
	KEY_STATE_FIRST_PRESS_VALID,	// 按键第一次按下有效状态
	KEY_STATE_FIRST_RELEASE,		// 按键第一次释放状态
	KEY_STATE_SECOND_PRESS,			// 按键第二次按下状态
	KEY_STATE_SECOND_PRESS_VALID,	// 按键第二次按下有效状态
	KEY_STATE_SECOND_RELEASE,		// 按键第二次释放状态
	KEY_STATE_LONG_PRESS,			// 按键长按状态
	KEY_STATE_LONG_RELEASE,			// 按键长按释放状态
	KEY_STATE_SHORT_LONG_PRESS,		// 按键短长按状态
	KEY_STATE_SHORT_LONG_RELEASE,	// 按键短长按释放状态
	KEY_STATE_LONG_LONG_PRESS,		// 按键超长按状态
	KEY_STATE_LONG_LONG_RELEASE,	// 按键超长按释放状态
} my_key_state_t;

// 按键事件
typedef enum
{
	KEY_EVENT_IDLE,					// 按键空闲事件
	KEY_EVENT_SINGLE_CLICK,			// 按键单击事件
	KEY_EVENT_DOUBLE_CLICK,			// 按键双击事件
	KEY_EVENT_LONG_PRESS,			// 按键长按事件
	KEY_EVENT_LONG_RELEASE,			// 按键长按释放事件
	KEY_EVENT_SHORT_LONG_PRESS,		// 按键短长按事件
	KEY_EVENT_SHORT_LONG_RELEASE,	// 按键短长按释放事件
	KEY_EVENT_LONG_LONG_PRESS,		// 按键超长按事件
	KEY_EVENT_LONG_LONG_RELEASE,	// 按键超长按释放事件
} my_key_event_t;

// 按键参数
typedef struct
{
	my_read_key_t 		read_key;  // 读按键管脚状态
	bool				key_is_press;  // 按键是否被按下
	volatile uint32_t 			count;  // 按键持续时间定时器计数值
	my_key_state_t 		state;  // 按键状态
	my_key_event_t 		event;  // 按键事件
	my_key_callback_t 	single_click_callback;  // 按键单击回调函数
	my_key_callback_t 	double_click_callback;  // 按键双击回调函数
	my_key_callback_t 	long_press_callback;  // 按键长按回调函数
	my_key_callback_t 	long_release_callback;  // 按键长按释放回调函数
	my_key_callback_t 	short_long_press_callback;  // 按键短长按回调函数
	my_key_callback_t 	short_long_release_callback;  // 按键短长按释放回调函数
	my_key_callback_t 	long_long_press_callback;  // 按键超长按回调函数
	my_key_callback_t 	long_long_release_callback;  // 按键超长按释放回调函数
} my_key_param_t;

extern my_key_param_t key_param[KEY_NUMBER];  // 按键参数结构体数组
extern __IO uint32_t tickcount;

void my_key_init(void);  // 按键初始化，必须在定时器初始化之前！！！
void my_key_scan(void);  // 按键扫描
void Key_CallbackFunc(void);


