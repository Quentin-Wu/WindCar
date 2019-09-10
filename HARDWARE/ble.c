#include "ble.h"

int num = 0;
uint8_t  startBit = 0;
uint8_t newLineReceived = 0;
uint16_t inputString[80] = {0};
uint16_t uartvalue = 0;

void BLEIntHandler(void)
{
		uint32_t flag = UARTIntStatus(UART2_BASE,1);
	
		//����жϱ�־
		UARTIntClear(UART2_BASE,flag);

		uartvalue =UARTCharGet(UART2_BASE);	//��ȡ���յ�������
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
	//ʹ��UART����
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2);
	//ʹ��GPIO����
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	//GPIO����ģʽ����
	GPIOPinConfigure(GPIO_PD6_U2RX);
	GPIOPinConfigure(GPIO_PD7_U2TX);
	GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_6 |GPIO_PIN_7);
	//uart���� �����ʣ�115200 8λ 1λֹͣλ ��У��
	UARTConfigSetExpClk(UART2_BASE, SysCtlClockGet(),
                    115200, (UART_CONFIG_WLEN_8 |UART_CONFIG_STOP_ONE |UART_CONFIG_PAR_NONE));
	
	//UART����FIFO Ĭ��FIFO LevelΪ4/8 �Ĵ�����8�ֽں�����ж�
	//���ú����1λ�Ͳ����ж�
	UARTFIFODisable(UART2_BASE);
	//ʹ��UART4�ж�
	IntEnable(INT_UART2);
	//ʹ��UART4�����ж�
	UARTIntEnable(UART2_BASE,UART_INT_RX |UART_INT_RT);
	//UART�жϵ�ַע��
	UARTIntRegister(UART2_BASE,BLEIntHandler);
	
	IntPrioritySet(INT_UART2, 3);
}
