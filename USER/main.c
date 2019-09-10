#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"

#include "led.h"
#include "myuart.h"
#include "delay.h"
#include "usmart.h"
#include "mypwm.h"
#include "ov7670.h"
#include "pid.h"
#include "control.h"
#include "ble.h"


extern uint8_t ov7670_vsync;
volatile float frame_count = 0;

extern PIDTypedef PID_Struct;

extern uint8_t pos;
extern int cross_state;

extern void BLE_Init(void);
extern uint16_t inputString[80] ;

int main(void)
{
	//配置系统时钟80Mhz
	SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

	LED_Init();
	My_Uart_Init();
	BLE_Init();
	usmart_dev.init(80);		 //80MHz
	
	PID_Init();
	M0PWM6_Init(20,700);  //PD0  //第一个参数周期：20ms（单位ms）  第二个参数带宽：（单位us）
	M0PWM7_Init(20,700);	 //PD1
	
	ov7670_GPIO_Config();
	ov7670_Init();
	
	PID_Struct.Kp = 0.1;
	PID_Struct.Kd = 0.5;
	PID_Struct.Ki = 0;
	
	while(inputString[1] != 49)
	{
		LED_RED(ON);
	}
	LED_RED(OFF);
	
	/* ov7670 寄存器默认配置初始化 */
	while(ov7670_Init() != SUCCESS)
	{
	}
	
	LED_GREEN_TUGGEL;
	printf("\r\nov7670摄像头初始化完成\r\n");
	ov7670_vsync = 0;

	while(1)
	{
//		LED_RED(OFF);
//		if(inputString[1] == 49)
//		{
//			LED_RED(ON);
			if( ov7670_vsync == 2 )
			{
				frame_count++;
				FIFO_PREPARE;  			/*FIFO准备*/					
				ImagGet_Proc(0,0,80,60);			/*采集并显示*/
				dispayImg();
				
				if(inputString[1]==49)
				{
					switch (cross_state)
					{
						case 0: straight_way(); break;
					}
				}
				else
				{
					printf("OFF\n");
					M0PWM6_PWM_Set(0);  
					M0PWM7_PWM_Set(0);
				}
				
				ov7670_vsync = 0;
				LED_GREEN_TUGGEL
			}
//		}
	}
}


