#include "my_hw_iic.h"

//I2C1
//SCL PA6
//SDA	PA7
void hw_iic_init(void)
{
	//使能I2C1外设
	SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1);
	//使能对应的管脚
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	
	//配置引脚为I2C功能
	GPIOPinConfigure(GPIO_PA6_I2C1SCL);
	GPIOPinConfigure(GPIO_PA7_I2C1SDA);
//	GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_6 | GPIO_PIN_7);
	GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_6);
	GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_7);
	
	//设置输出IO口的驱动能力,8mA，带弱上拉输出
	GPIOPadConfigSet(GPIO_PORTA_BASE,GPIO_PIN_6,GPIO_STRENGTH_8MA,GPIO_PIN_TYPE_STD_WPU); 
	//开漏
	GPIOPadConfigSet(GPIO_PORTA_BASE,GPIO_PIN_7,GPIO_STRENGTH_8MA,GPIO_PIN_TYPE_OD); 
	
	I2CMasterInitExpClk(I2C1_BASE, SysCtlClockGet(), false);//false:100kbps true:400kbps
	
	I2CMasterEnable(I2C1_BASE);
}

//void hw_iic_init(void)
//{
//SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
//SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

//GPIOPinConfigure(GPIO_PB2_I2C0SCL);
//GPIOPinConfigure(GPIO_PB3_I2C0SDA);

//GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_2 | GPIO_PIN_3);

//GPIOPadConfigSet(GPIO_PORTB_BASE,GPIO_PIN_2,GPIO_STRENGTH_8MA,GPIO_PIN_TYPE_STD_WPU);
//GPIOPadConfigSet(GPIO_PORTB_BASE,GPIO_PIN_3,GPIO_STRENGTH_8MA,GPIO_PIN_TYPE_OD); 

//I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), false); //false:100kbps true:400kbps

//I2CMasterEnable(I2C0_BASE);
//}

void hw_iic_writebyte(uint8_t slave_addr,uint8_t reg_addr,uint8_t data)
{	
	
	//false：主机向ov7670写数据
	//true:：主机向ov7670读数据
	I2CMasterSlaveAddrSet(I2C1_BASE, slave_addr, false); 
	//写地址
	I2CMasterDataPut(I2C1_BASE, reg_addr);
	//控制主机状态为发送
	I2CMasterControl(I2C1_BASE,  I2C_MASTER_CMD_BURST_SEND_START);
	while(I2CMasterBusy(I2C1_BASE));
	
	//写数据
	I2CMasterDataPut(I2C1_BASE, data);
	I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);
	while(I2CMasterBusy(I2C1_BASE));
	
	I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH); 
	while(I2CMasterBusy(I2C1_BASE));
}

uint8_t hw_iic_readbyte(uint8_t slave_addr,uint8_t reg_addr)
{
	uint8_t data;
	
	//false：主机向ov7670写数据
	I2CMasterSlaveAddrSet(I2C1_BASE, slave_addr, false);
	//写地址
	I2CMasterDataPut(I2C1_BASE, reg_addr);
	I2CMasterControl(I2C1_BASE,  I2C_MASTER_CMD_BURST_SEND_START);
	while(I2CMasterBusy(I2C1_BASE));
	
	I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH); 
	while(I2CMasterBusy(I2C1_BASE));
	
	I2CMasterSlaveAddrSet(I2C1_BASE, slave_addr, true);
	I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START);
	while(I2CMasterBusy(I2C1_BASE));
	
	data = I2CMasterDataGet(I2C1_BASE);
	I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_RECEIVE_CONT);
	while(I2CMasterBusy(I2C1_BASE));
	
	I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH); 
	while(I2CMasterBusy(I2C1_BASE));
	return data;
}

int i2c_write(unsigned char slave_addr,
                     unsigned char reg_addr,
                     unsigned char length,
                     unsigned char const *data)
{
    //设置I2C1主模块将要放在总线上的从模块地址
    //false代表主模块发送，从模块接收
	unsigned char i;
    if(0 == length)
    {
    	return -1;
    }
	I2CMasterSlaveAddrSet(I2C1_BASE, slave_addr, false);
	I2CMasterDataPut(I2C1_BASE, reg_addr);
	I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_START);
	while(I2CMasterBusy(I2C1_BASE))
	{
	}
	// 主模块开始发送寄存器地址
	for(i=0; i<length; i++)
    {
		I2CMasterDataPut(I2C1_BASE, data[i]);
		// 主模块开始发送数据
		I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);
		while(I2CMasterBusy(I2C1_BASE))
		{
		}
    }
	I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_STOP);
	while(I2CMasterBusy(I2C1_BASE))
	{
	}
	return 0;
}


