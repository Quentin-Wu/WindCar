#ifndef __OV7670_H
#define __OV7670_H

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/pin_map.h"

#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

// FIFO ���ʹ�ܣ���ģ���е�OE
#define      SYSCTL_PERIPH_GPIO_OE       	            SYSCTL_PERIPH_GPIOD
#define      GPIO_PORT_BASE_OE                      	GPIO_PORTD_BASE
#define      ov7670_GPIO_PIN_OE                       GPIO_PIN_7

// FIFO д��λ
#define      SYSCTL_PERIPH_GPIO_WRST       	          SYSCTL_PERIPH_GPIOE
#define      GPIO_PORT_BASE_WRST                      GPIO_PORTE_BASE
#define      ov7670_GPIO_PIN_WRST                     GPIO_PIN_2

// FIFO ����λ
#define      SYSCTL_PERIPH_GPIO_RRST       	          SYSCTL_PERIPH_GPIOE
#define      GPIO_PORT_BASE_RRST                      GPIO_PORTE_BASE
#define      ov7670_GPIO_PIN_RRST                     GPIO_PIN_1

// FIFO ��ʱ��
#define      SYSCTL_PERIPH_GPIO_RCLK       	          SYSCTL_PERIPH_GPIOE
#define      GPIO_PORT_BASE_RCLK                      GPIO_PORTE_BASE
#define      ov7670_GPIO_PIN_RCLK                     GPIO_PIN_3

// FIFO дʹ��
#define      SYSCTL_PERIPH_GPIO_WE       	            SYSCTL_PERIPH_GPIOF
#define      GPIO_PORT_BASE_WE                      	GPIO_PORTF_BASE
#define      ov7670_GPIO_PIN_WE                       GPIO_PIN_4

// 8λ���ݿ�
#define      SYSCTL_PERIPH_GPIO_DATA                  SYSCTL_PERIPH_GPIOB
#define      GPIO_PORT_BASE_DATA                   		GPIO_PORTB_BASE
#define      ov7670_DATA_0_GPIO_PIN                   GPIO_PIN_0
//#define      ov7670_DATA_1_GPIO_PIN                   GPIO_PIN_1
#define      ov7670_DATA_2_GPIO_PIN                   GPIO_PIN_2
#define      ov7670_DATA_3_GPIO_PIN                   GPIO_PIN_3
#define      ov7670_DATA_4_GPIO_PIN                   GPIO_PIN_4
#define      ov7670_DATA_5_GPIO_PIN                   GPIO_PIN_5
#define      ov7670_DATA_6_GPIO_PIN                   GPIO_PIN_6
#define      ov7670_DATA_7_GPIO_PIN                   GPIO_PIN_7

//data1
#define      SYSCTL_PERIPH_GPIO_DATA_1                SYSCTL_PERIPH_GPIOD
#define      GPIO_PORT_BASE_DATA_1                   	GPIO_PORTD_BASE
#define      ov7670_DATA_1_GPIO_PIN                   GPIO_PIN_2


#define GPIOB_DATA  *(uint32_t *)0x400053f4
#define GPIOD_DATAp *(uint32_t *)0x40007010

// ov7670���ж�
#define      SYSCTL_PERIPH_GPIO_VSYNC       	        SYSCTL_PERIPH_GPIOE
#define      GPIO_PORT_BASE_VSYNC                     GPIO_PORTE_BASE
#define      ov7670_GPIO_PIN_VSYNC                    GPIO_PIN_4


#define FIFO_OE_H()     GPIOPinWrite(GPIO_PORT_BASE_OE, ov7670_GPIO_PIN_OE , ov7670_GPIO_PIN_OE)	  
#define FIFO_OE_L()     GPIOPinWrite(GPIO_PORT_BASE_OE, ov7670_GPIO_PIN_OE , 0)	  /*����ʹFIFO���ʹ��*/

#define FIFO_WRST_H()   GPIOPinWrite(GPIO_PORT_BASE_WRST, ov7670_GPIO_PIN_WRST , ov7670_GPIO_PIN_WRST)	  /*��������FIFOд(����from����ͷ)ָ���˶� */
#define FIFO_WRST_L()   GPIOPinWrite(GPIO_PORT_BASE_WRST, ov7670_GPIO_PIN_WRST , 0)	  /*����ʹFIFOд(����from����ͷ)ָ�븴λ*/

#define FIFO_RRST_H()   GPIOPinWrite(GPIO_PORT_BASE_RRST, ov7670_GPIO_PIN_RRST , ov7670_GPIO_PIN_RRST)	  /*��������FIFO��(���ݴ�FIFO���)ָ���˶� */
#define FIFO_RRST_L()   GPIOPinWrite(GPIO_PORT_BASE_RRST, ov7670_GPIO_PIN_RRST , 0)	  /*����ʹFIFO��(���ݴ�FIFO���)ָ�븴λ */

#define FIFO_RCLK_H()   GPIOPinWrite(GPIO_PORT_BASE_RCLK, ov7670_GPIO_PIN_RCLK , ov7670_GPIO_PIN_RCLK)	  
#define FIFO_RCLK_L()   GPIOPinWrite(GPIO_PORT_BASE_RCLK, ov7670_GPIO_PIN_RCLK , 0)	  /*FIFO�������ʱ��*/

#define FIFO_WE_H()     GPIOPinWrite(GPIO_PORT_BASE_WE, ov7670_GPIO_PIN_WE , ov7670_GPIO_PIN_WE)	  /*����ʹFIFOд����*/
#define FIFO_WE_L()     GPIOPinWrite(GPIO_PORT_BASE_WE, ov7670_GPIO_PIN_WE , 0)

#define READ_FIFO_PIXEL(RGB565)   	do{\
	                                  RGB565=0;\
	                                  FIFO_RCLK_L();\
	                                  RGB565 = ((GPIOB_DATA|(GPIOD_DATAp>>1))<<8) & 0xff00;\
	                                  FIFO_RCLK_H();\
                                    FIFO_RCLK_L();\
	                                  RGB565 |= (GPIOB_DATA|(GPIOD_DATAp>>1)) & 0x00ff;\
	                                  FIFO_RCLK_H();\
                                    }while(0)

#define READ_FIFO_SKIP()						do{\
	                                  FIFO_RCLK_L();\
	                                  FIFO_RCLK_H();\
                                    FIFO_RCLK_L();\
	                                  FIFO_RCLK_H();\
                                    }while(0)

#define FIFO_PREPARE                do{\
	                                  FIFO_RRST_L();\
	                                  FIFO_RCLK_L();\
	                                  FIFO_RCLK_H();\
	                                  FIFO_RRST_H();\
	                                  FIFO_RCLK_L();\
	                                  FIFO_RCLK_H();\
                                    }while(0)

#define ov7670_ID       0x73
																		
typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;
void ov7670_GPIO_Config(void);
ErrorStatus ov7670_Init(void);
void ImagSend(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height);
void ImagGet(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height);
void ImagGet_Proc(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height);
void dispayImg(void);


#endif


