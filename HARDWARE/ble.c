#include "ble.h"

int num = 0;
uint8_t  startBit = 0;
uint8_t newLineReceived = 0;
uint16_t inputString[80] = {0};
uint16_t uartvalue = 0;

void BLEIntHandler(void)
{
		uint32_t flag = UARTIntStatus(UART2_BASE,1);
	
		//清除中断标志
		UARTIntClear(UART2_BASE,flag);

		uartvalue =UARTCharGet(UART2_BASE);	//读取接收到的数据
//		UARTCharPutNonBlocking(UART0_BASE,1);
		if(uartvalue == '$')
	    {
	      	startBit = 1;
					num = 0;
	    }
	    if(startBit == 1)
	    {
	       	inputString[num] = uartvalue;     
	    }  
	    if (startBit == 1 && uartvalue == '#') 
	    {		
				newLineReceived = 1; 
				startBit = 0;
	    }
			num++;
			if(num >= 80)
				{
					num = 0;
					startBit = 0;
					newLineReceived	= 0;
				}	 
}


void BLE_Init(void)
{
	//使能UART外设
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2);
	//使能GPIO外设
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	//GPIO复用模式配置
	GPIOPinConfigure(GPIO_PD6_U2RX);
	GPIOPinConfigure(GPIO_PD7_U2TX);
	GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_6 |GPIO_PIN_7);
	//uart配置 波特率：115200 8位 1位停止位 无校验
	UARTConfigSetExpClk(UART2_BASE, SysCtlClockGet(),
                    115200, (UART_CONFIG_WLEN_8 |UART_CONFIG_STOP_ONE |UART_CONFIG_PAR_NONE));
	
	//UART禁用FIFO 默认FIFO Level为4/8 寄存器满8字节后产生中断
	//禁用后接收1位就产生中断
	UARTFIFODisable(UART2_BASE);
	//使能UART4中断
	IntEnable(INT_UART2);
	//使能UART4接收中断
	UARTIntEnable(UART2_BASE,UART_INT_RX |UART_INT_RT);
	//UART中断地址注册
	UARTIntRegister(UART2_BASE,BLEIntHandler);
	
	IntPrioritySet(INT_UART2, 3);
}
