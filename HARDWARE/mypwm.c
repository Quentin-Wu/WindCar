#include "mypwm.h"

//PC4
void  M0PWM6_Init(uint32_t freq, float width)
{
	//设置PWM时钟为系统时钟的8分频
	SysCtlPWMClockSet(SYSCTL_PWMDIV_64);
	//使能PWM0外设
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
	
	//配置引脚为PWM功能
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	GPIOPinConfigure(GPIO_PC4_M0PWM6);
	GPIOPinTypePWM(GPIO_PORTC_BASE, GPIO_PIN_4);
	
	//配置 PWM0 Generator3·发生器
  PWMGenConfigure(PWM0_BASE, PWM_GEN_3, PWM_GEN_MODE_UP_DOWN|PWM_GEN_MODE_NO_SYNC);
	//配置 PWM0 Generator3 周期
  PWMGenPeriodSet(PWM0_BASE, PWM_GEN_3, 1250*freq - 1);
	//配置 PWM0 Generator3 占空比
  PWMPulseWidthSet(PWM0_BASE, PWM_OUT_6, 125*width/100 - 1);
	
	//使能 PWM0 输出
  PWMOutputState(PWM0_BASE, PWM_OUT_6_BIT, true);
  //使能 PWM0 发生器模块
  PWMGenEnable(PWM0_BASE, PWM_GEN_3);
}

//PC5
void  M0PWM7_Init(uint32_t freq, float width)
{
	//设置PWM时钟为系统时钟的8分频
	SysCtlPWMClockSet(SYSCTL_PWMDIV_64);
	//使能PWM0外设
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
	
	//配置引脚为PWM功能
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	GPIOPinConfigure(GPIO_PC5_M0PWM7);
	GPIOPinTypePWM(GPIO_PORTC_BASE, GPIO_PIN_5);
	
	//配置 PWM0 Generator3·发生器
  PWMGenConfigure(PWM0_BASE, PWM_GEN_3, PWM_GEN_MODE_UP_DOWN|PWM_GEN_MODE_NO_SYNC);
	//配置 PWM0 Generator3 周期
  PWMGenPeriodSet(PWM0_BASE, PWM_GEN_3, 1250*freq - 1);
	//配置 PWM0 Generator3 占空比
  PWMPulseWidthSet(PWM0_BASE, PWM_OUT_7, 125*width/100 - 1);
	
	//使能 PWM0 输出
  PWMOutputState(PWM0_BASE, PWM_OUT_7_BIT, true);
  //使能 PWM0 发生器模块
  PWMGenEnable(PWM0_BASE, PWM_GEN_3);
}


void M0PWM6_PWM_Set(uint32_t width)
{
	//配置 PWM0 Generator3 占空比
  PWMPulseWidthSet(PWM0_BASE, PWM_OUT_6, 1.25*width - 1);
}

void M0PWM7_PWM_Set(uint32_t width)
{
	//配置 PWM0 Generator3 占空比
  PWMPulseWidthSet(PWM0_BASE, PWM_OUT_7, 1.25*width - 1);
}



