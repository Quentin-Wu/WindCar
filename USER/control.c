#include "control.h"

#include "pid.h"
#include "myuart.h"
#include "mypwm.h"

extern int cross_state;
extern uint8_t pos;

uint32_t PWM_R_max,PWM_R_min,PWM_L_max,PWM_L_min;

void straight_way(void)
{
	PWM_R_max = 1130;
	PWM_R_min = 1100;
	PWM_L_max = 1140;
	PWM_L_min = 1100;
	PID_realize(pos);
}


