#include "usmart.h"
#include "usmart_str.h"

#include "led.h"
#include "delay.h"
#include "mypwm.h"

//�������б��ʼ��(�û��Լ����)
//�û�ֱ������������Ҫִ�еĺ�����������Ҵ�
struct _m_usmart_nametab usmart_nametab[]=
{
#if USMART_USE_WRFUNS==1 	//���ʹ���˶�д����
	(void*)read_addr,"u32 read_addr(u32 addr)",
	(void*)write_addr,"void write_addr(u32 addr,u32 val)",	 
#endif		   
	(void*)delay_ms,"void delay_ms(u16 xms)",
 	(void*)delay_us,"void delay_us(u32 xus)",	
	(void*)LED_RED_DEBUG, "void LED_RED_DEBUG(bool a)",
	(void*)LED_BLUE_DEBUG, "void LED_BLUE_DEBUG(bool a)",
	(void*)LED_GREEN_DEBUG, "void LED_GREEN_DEBUG(bool a)",
	(void*)M0PWM6_PWM_Set, "void M0PWM6_PWM_Set(uint32_t width)",
	(void*)M0PWM7_PWM_Set, "void M0PWM7_PWM_Set(uint32_t width)",
};	




//�������ƹ�������ʼ��
//�õ������ܿغ���������
//�õ�����������
struct _m_usmart_dev usmart_dev=
{
	usmart_nametab,
	usmart_init,
	usmart_cmd_rec,
	usmart_exe,
	usmart_scan,
	sizeof(usmart_nametab)/sizeof(struct _m_usmart_nametab),//��������
	0,	  	//��������
	0,	 	//����ID
	1,		//������ʾ����,0,10����;1,16����
	0,		//��������.bitx:,0,����;1,�ַ���	    
	0,	  	//ÿ�������ĳ����ݴ��,��ҪMAX_PARM��0��ʼ��
	0,		//�����Ĳ���,��ҪPARM_LEN��0��ʼ��
};


