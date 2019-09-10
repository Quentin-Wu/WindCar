#ifndef __MY_SW_IIC_H
#define __MY_SW_IIC_H

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/pin_map.h"

#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

/* exact-width unsigned integer types */
typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;

// SCL
#define      SYSCTL_PERIPH_GPIO_SCL       	          SYSCTL_PERIPH_GPIOA
#define      GPIO_PORT_BASE_SCL                      	GPIO_PORTA_BASE
#define      iic_GPIO_PIN_SCL                         GPIO_PIN_6

// SDA
#define      SYSCTL_PERIPH_GPIO_SDA       	          SYSCTL_PERIPH_GPIOA
#define      GPIO_PORT_BASE_SDA                       GPIO_PORTA_BASE
#define      iic_GPIO_PIN_SDA                         GPIO_PIN_7

#define SCL_H         GPIOPinWrite(GPIO_PORT_BASE_SCL, iic_GPIO_PIN_SCL , iic_GPIO_PIN_SCL) 
#define SCL_L         GPIOPinWrite(GPIO_PORT_BASE_SCL, iic_GPIO_PIN_SCL , 0)  
   
#define SDA_H         GPIOPinWrite(GPIO_PORT_BASE_SDA, iic_GPIO_PIN_SDA , iic_GPIO_PIN_SDA)  
#define SDA_L         GPIOPinWrite(GPIO_PORT_BASE_SDA, iic_GPIO_PIN_SDA , 0)   

#define SDA_read      GPIOPinRead(GPIO_PORT_BASE_SDA, iic_GPIO_PIN_SDA) 

#define	SCCB_SDA_IN()		GPIOPinTypeGPIOInput(GPIO_PORT_BASE_SDA, iic_GPIO_PIN_SDA)
#define	SCCB_SDA_OUT()	GPIOPinTypeGPIOOutput(GPIO_PORT_BASE_SDA, iic_GPIO_PIN_SDA)

#define ADDR_OV7670   0x42

void my_sw_iic_init(void);
u8 SCCB_WriteByte( uint16_t WriteAddress , uint8_t SendByte );
u8 SCCB_ReadByte(u8* pBuffer,   u16 length,   u8 ReadAddress);

#endif


