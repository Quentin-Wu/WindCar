#include "pid.h"
#include "myuart.h"
#include "mypwm.h"
#include "math.h"

PIDTypedef PID_Struct;
extern int cross_state;
extern uint32_t PWM_R_max,PWM_R_min,PWM_L_max,PWM_L_min;

void PID_Init(void)
{
	PID_Struct.Kp = 0;
	PID_Struct.Kd = 0;
	PID_Struct.Ki = 0;
	
	PID_Struct.Ek	= 0;
	PID_Struct.Ek1 = 0;
	PID_Struct.Ek2 = 0;
	
	PID_Struct.PWM_L = 0;
	PID_Struct.PWM_R = 0;
}

void Set_Kp(float kpp)
{
	PID_Struct.Kp = kpp;
	printf("Kp = %f\n", PID_Struct.Kp);
}

void Set_Kd(float kdd)
{
	PID_Struct.Kd = kdd;
	printf("Kd = %f\n", PID_Struct.Kd);
}

void Set_Ki(float kii)
{
	PID_Struct.Ki = kii;
	printf("Ki = %f\n", PID_Struct.Ki);
}


void PID_realize(uint8_t x)
{	  
		int plu;
	
	if(cross_state==-2 || x==0)
	{
		PID_Struct.PWM_R=0;
		PID_Struct.PWM_L=0;
	}
	else if( fabs(x-30) < 4)
	{
		PID_Struct.PWM_R=1130;
		PID_Struct.PWM_L=1130;
	}
	else
	{
		PID_Struct.Ek = x - 30;	

		plu = PID_Struct.Kp*(PID_Struct.Ek-PID_Struct.Ek1)
			+PID_Struct.Ki*PID_Struct.Ek
			+PID_Struct.Kd*(PID_Struct.Ek-2*PID_Struct.Ek1+PID_Struct.Ek2);
		
		PID_Struct.PWM_R = PID_Struct.PWM_R + plu*0.7;
		PID_Struct.PWM_L = PID_Struct.PWM_L - plu;
	
		PID_Struct.Ek1 = PID_Struct.Ek;
		
		if(PID_Struct.PWM_R>PWM_R_max)		PID_Struct.PWM_R=PWM_R_max;
		if(PID_Struct.PWM_R<PWM_R_min)			PID_Struct.PWM_R=PWM_R_min;
		
		if(PID_Struct.PWM_L>PWM_L_max)		PID_Struct.PWM_L=PWM_L_max;
		if(PID_Struct.PWM_L<PWM_L_min)			PID_Struct.PWM_L=PWM_L_min;
	}
	
		printf("%d\n",x);
		printf("左侧风机：%d\n",PID_Struct.PWM_L);
		printf("右侧风机：%d\n",PID_Struct.PWM_R);
	
		M0PWM6_PWM_Set(PID_Struct.PWM_L);  
		M0PWM7_PWM_Set(PID_Struct.PWM_R);
}



















