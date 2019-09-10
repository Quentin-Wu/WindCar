#ifndef		__MYUART_H
#define		__MYUART_H

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"

#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/uart.h"
#include "driverlib/gpio.h"


#define USART_BLE_LEN  			200  	//定义最大接收字节数 200
#define EN_USART2_RX 			1		//使能（1）/禁止（0）串口1接收
	  				
//extern uint8_t  USART_BLE_RX_BUF[USART_BLE_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
//extern uint16_t USART_BLE_RX_STA;         		//接收状态标记	

void BLE_Init(void);


#endif

