#ifndef __MY_HW_IIC_H
#define __MY_HW_IIC_H

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/pin_map.h"

#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/i2c.h"

#define ADDR_OV7670   (0x42>>1)

void hw_iic_init(void);
void hw_iic_writebyte(uint8_t slave_addr,uint8_t reg_addr,uint8_t data);
uint8_t hw_iic_readbyte(uint8_t slave_addr,uint8_t reg_addr);





#endif



