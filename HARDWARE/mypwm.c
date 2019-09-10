#include "mypwm.h"

//PC4
void  M0PWM6_Init(uint32_t freq, float width)
{
	//����PWMʱ��Ϊϵͳʱ�ӵ�8��Ƶ
	SysCtlPWMClockSet(SYSCTL_PWMDIV_64);
	//ʹ��PWM0����
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
	
	//��������ΪPWM����
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	GPIOPinConfigure(GPIO_PC4_M0PWM6);
	GPIOPinTypePWM(GPIO_PORTC_BASE, GPIO_PIN_4);
	
	//���� PWM0 Generator3��������
  PWMGenConfigure(PWM0_BASE, PWM_GEN_3, PWM_GEN_MODE_UP_DOWN|PWM_GEN_MODE_NO_SYNC);
	//���� PWM0 Generator3 ����
  PWMGenPeriodSet(PWM0_BASE, PWM_GEN_3, 1250*freq - 1);
	//���� PWM0 Generator3 ռ�ձ�
  PWMPulseWidthSet(PWM0_BASE, PWM_OUT_6, 125*width/100 - 1);
	
	//ʹ�� PWM0 ���
  PWMOutputState(PWM0_BASE, PWM_OUT_6_BIT, true);
  //ʹ�� PWM0 ������ģ��
  PWMGenEnable(PWM0_BASE, PWM_GEN_3);
}

//PC5
void  M0PWM7_Init(uint32_t freq, float width)
{
	//����PWMʱ��Ϊϵͳʱ�ӵ�8��Ƶ
	SysCtlPWMClockSet(SYSCTL_PWMDIV_64);
	//ʹ��PWM0����
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
	
	//��������ΪPWM����
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	GPIOPinConfigure(GPIO_PC5_M0PWM7);
	GPIOPinTypePWM(GPIO_PORTC_BASE, GPIO_PIN_5);
	
	//���� PWM0 Generator3��������
  PWMGenConfigure(PWM0_BASE, PWM_GEN_3, PWM_GEN_MODE_UP_DOWN|PWM_GEN_MODE_NO_SYNC);
	//���� PWM0 Generator3 ����
  PWMGenPeriodSet(PWM0_BASE, PWM_GEN_3, 1250*freq - 1);
	//���� PWM0 Generator3 ռ�ձ�
  PWMPulseWidthSet(PWM0_BASE, PWM_OUT_7, 125*width/100 - 1);
	
	//ʹ�� PWM0 ���
  PWMOutputState(PWM0_BASE, PWM_OUT_7_BIT, true);
  //ʹ�� PWM0 ������ģ��
  PWMGenEnable(PWM0_BASE, PWM_GEN_3);
}


void M0PWM6_PWM_Set(uint32_t width)
{
	//���� PWM0 Generator3 ռ�ձ�
  PWMPulseWidthSet(PWM0_BASE, PWM_OUT_6, 1.25*width - 1);
}

void M0PWM7_PWM_Set(uint32_t width)
{
	//���� PWM0 Generator3 ռ�ձ�
  PWMPulseWidthSet(PWM0_BASE, PWM_OUT_7, 1.25*width - 1);
}



