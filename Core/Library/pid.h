/*
 * pid.h
 *
 *  Created on: Apr 3, 2025
 *      Author: Alva
 */

#ifndef LIBRARY_PID_H_
#define LIBRARY_PID_H_

#include "motor.h"
#include "TOF050C.h"
#include "ADC_pid.h"

typedef struct pid_struct pid;
extern pid my_pid;
extern pid my_position_pid;

void change_pidindex(pid * my_pid,float kp,float ki,float kd);
void pid_pulse_cal(pid * my_pid,uint8_t range);
void pid_control_motor(pid * my_pid,uint8_t range);
void change_pid(pid * my_pid,volatile uint16_t* buffer);
#endif /* LIBRARY_PID_H_ */
