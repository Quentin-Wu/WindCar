#ifndef __PID_H
#define __PID_H

#include <stdint.h>

typedef struct
{

	float Kp;
	float Ki;
	float Kd;
	
	float Ek;
	float Ek1;
	float Ek2;
	
	uint32_t PWM_L;
	uint32_t PWM_R;
	
	uint32_t PWM_L_Middle; //舵机中值
	uint32_t PWM_R_Middle; //舵机中值
	
}PIDTypedef;

void PID_Init(void);
void Set_Kp(float kpp);
void Set_Kd(float kdd);
void Set_Ki(float kii);
void PID_realize(uint8_t x);


#endif







