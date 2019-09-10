#include "my_sw_iic.h"
#include "delay.h"

#define DEV_ADR  ADDR_OV7670 			 /*设备地址定义*/

void my_sw_iic_init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIO_SCL);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIO_SDA);
	
	GPIOPinTypeGPIOOutput(GPIO_PORT_BASE_SCL, iic_GPIO_PIN_SCL);
	GPIOPinTypeGPIOOutput(GPIO_PORT_BASE_SDA, iic_GPIO_PIN_SDA);
}



static void SCCB_delay(void)
{	
   delay_us(50);
}


/********************************************************************
 * 函数名：SCCB_Start
 * 描述  ：SCCB起始信号
 * 输入  ：无
 * 输出  ：无
 * 注意  ：内部调用        
 ********************************************************************/
static int SCCB_Start(void)
{
	SDA_H;
	SCL_H;
	SCCB_delay();
	if(!SDA_read)
	return DISABLE;	/* SDA线为低电平则总线忙,退出 */
	SDA_L;
	SCCB_delay();
	if(SDA_read) 
	return DISABLE;	/* SDA线为高电平则总线出错,退出 */
	SDA_L;
	SCCB_delay();
	return ENABLE;
}



/********************************************************************
 * 函数名：SCCB_Stop
 * 描述  ：SCCB停止信号
 * 输入  ：无
 * 输出  ：无
 * 注意  ：内部调用        
 ********************************************************************/
static void SCCB_Stop(void)
{
	SCL_L;
	SCCB_delay();
	SDA_L;
	SCCB_delay();
	SCL_H;
	SCCB_delay();
	SDA_H;
	SCCB_delay();
}



/********************************************************************
 * 函数名：SCCB_Ack
 * 描述  ：SCCB应答方式
 * 输入  ：无
 * 输出  ：无
 * 注意  ：内部调用        
 ********************************************************************/
static void SCCB_Ack(void)
{	
	SCL_L;
	SCCB_delay();
	SDA_L;
	SCCB_delay();
	SCL_H;
	SCCB_delay();
	SCL_L;
	SCCB_delay();
}



/********************************************************************
 * 函数名：SCCB_NoAck
 * 描述  ：SCCB 无应答方式
 * 输入  ：无
 * 输出  ：无
 * 注意  ：内部调用        
 ********************************************************************/
static void SCCB_NoAck(void)
{	
	SCL_L;
	SCCB_delay();
	SDA_H;
	SCCB_delay();
	SCL_H;
	SCCB_delay();
	SCL_L;
	SCCB_delay();
}

///********************************************************************
// * 函数名：SCCB_WaitAck
// * 描述  ：SCCB 等待应答
// * 输入  ：无
// * 输出  ：返回为:=1有ACK,=0无ACK
// * 注意  ：内部调用        
// ********************************************************************/
//static int SCCB_WaitAck(void) 	
//{
////	printf("1\n");
//	SCL_L;
//	SCCB_delay();
//	SDA_H;			
//	SCCB_delay();
//	SCL_H;
//	SCCB_SDA_IN();
//	SCCB_delay();	
//	if(SDA_read)
//	{
//      SCL_L;
//      return DISABLE;
//	}
//	SCL_L;
//	SCCB_SDA_OUT();
//	return ENABLE;
//}



 /*******************************************************************
 * 函数名：SCCB_SendByte
 * 描述  ：数据从高位到低位
 * 输入  ：SendByte: 发送的数据
 * 输出  ：无
 * 注意  ：内部调用        
 *********************************************************************/
static u8 SCCB_SendByte(uint8_t dat) 
{
//    uint8_t i=8;
//    while(i--)
//    {
//        SCL_L;
//        SCCB_delay();
//      if(SendByte&0x80)
//        SDA_H;  
//      else 
//        SDA_L;   
//        SendByte<<=1;
//        SCCB_delay();
//		SCL_H;
//        SCCB_delay();
//    }
//    SCL_L;
	
	uint8_t j,res;
	SCL_L;
	SCCB_delay();
  for(j=0;j<8;j++)
	{
		
		
		if((dat<<j)&0x80)	
			SDA_H;
		else
			SDA_L;
		SCCB_delay();
		
		SCL_H;
		SCCB_delay();
		
		SCL_L;
		SCCB_delay();
	}
	
	SCCB_SDA_IN();			//设置SDA为输入
	SCCB_delay();
	
	SCL_H;
	SCCB_delay();
	
	if(SDA_read)	res=1;  //SDA=1发送失败，返回1
	else	res=0;
	
	SCL_L;
	SCCB_SDA_OUT();
	
	return res;
}




 /******************************************************************
 * 函数名：SCCB_ReceiveByte
 * 描述  ：数据从高位到低位
 * 输入  ：无
 * 输出  ：SCCB总线返回的数据
 * 注意  ：内部调用        
 *******************************************************************/
static u8 SCCB_ReceiveByte(void)  
{ 
//    uint8_t i;
//    uint8_t ReceiveByte=0;

//    SDA_H;				
//    while(i--)
//    {
//      ReceiveByte<<=1;      
//      SCL_L;
//      SCCB_delay();
//	  SCL_H;
//      SCCB_delay();
//			SCCB_SDA_IN();
//      if(SDA_read)
//      {
//        ReceiveByte|=0x01;
//      }
//    }
//    SCL_L;
//		SCCB_SDA_OUT();
//    return ReceiveByte;
	
	u8 read,j;
	read=0x00;
	
	SCCB_SDA_IN();
	SCCB_delay();
	
	for(j=8;j>0;j--)
	{
		SCCB_delay();
		SCL_H;
		
		read=read<<1;
		if(SDA_read)	read++;
		
		SCCB_delay();
		SCL_L;
	}
	
	SCCB_SDA_OUT();
	return (read);
}


 /*****************************************************************************************
 * 函数名：SCCB_WriteByte
 * 描述  ：写一字节数据
 * 输入  ：- WriteAddress: 待写入地址 	- SendByte: 待写入数据	- DeviceAddress: 器件类型
 * 输出  ：返回为:=1成功写入,=0失败
 * 注意  ：无        
 *****************************************************************************************/           
u8 SCCB_WriteByte( uint16_t WriteAddress , uint8_t SendByte )
{		
//    if(!SCCB_Start())
//	{
//	    return DISABLE;
//	}
//    SCCB_SendByte( DEV_ADR );                    /* 器件地址 */
//    if( !SCCB_WaitAck() )
//	{
//		SCCB_Stop(); 
//		return DISABLE;
//	}
//    SCCB_SendByte((uint8_t)(WriteAddress & 0x00FF));   /* 设置低起始地址 */      
//    SCCB_WaitAck();	
//    SCCB_SendByte(SendByte);
//    SCCB_WaitAck();   
//    SCCB_Stop(); 
//    return ENABLE;
	
    if(!SCCB_Start())
	{
	    return DISABLE;
	}
	if(SCCB_SendByte( DEV_ADR ))	return DISABLE;		//写器件地址 失败返回1
	SCCB_delay();
	SCCB_delay();
	
	if(SCCB_SendByte(WriteAddress)) return DISABLE;				//写寄存器地址 失败返回1
	SCCB_delay();
	SCCB_delay();
	
	if(SCCB_SendByte(SendByte)) return DISABLE;				//写数据				失败返回1
	SCCB_Stop();					//停止
	
	return ENABLE;
	
}

/******************************************************************************************************************
 * 函数名：SCCB_ReadByte
 * 描述  ：读取一串数据
 * 输入  ：- pBuffer: 存放读出数据 	- length: 待读出长度	- ReadAddress: 待读出地址		 - DeviceAddress: 器件类型
 * 输出  ：返回为:=1成功读入,=0失败
 * 注意  ：无        
 **********************************************************************************************************************/           
u8 SCCB_ReadByte(uint8_t* pBuffer, uint16_t length, uint8_t ReadAddress)
{	
//    if(!SCCB_Start())
//	{
//	    return DISABLE;
//	}
//    SCCB_SendByte( DEV_ADR );         /* 器件地址 */

//    if( !SCCB_WaitAck() )
//	{
//		SCCB_Stop(); 
//		return DISABLE;
//	}
//    SCCB_SendByte( ReadAddress );     /* 设置低起始地址 */      
//    SCCB_WaitAck();	
//    SCCB_Stop(); 
//    if(!SCCB_Start())
//	{
//		return DISABLE;
//	}
//    SCCB_SendByte( DEV_ADR + 1 );     /* 器件地址 */ 
//	printf("2\n");
//    if(!SCCB_WaitAck())
//	{
//		SCCB_Stop(); 
//		return DISABLE;
//	}
//    while(length)
//    {
//      *pBuffer = SCCB_ReceiveByte();
//      if(length == 1)
//	  {
//		  SCCB_NoAck();
//	  }
//      else
//	  {
//		SCCB_Ack(); 
//	  }
//      pBuffer++;
//      length--;
//    }
//    SCCB_Stop();
//    return ENABLE;

    if(!SCCB_Start())
	{
	    return DISABLE;
	}
	if(SCCB_SendByte( DEV_ADR ))	return DISABLE;		//写器件地址 失败返回1
	SCCB_delay();
	SCCB_delay();
	
	if(SCCB_SendByte(ReadAddress)) return DISABLE;				//写寄存器地址 失败返回1
	SCCB_delay();
	SCCB_delay();
	
	SCCB_Stop();
	SCCB_delay();
	SCCB_delay();
	
	if(!SCCB_Start())
	{
	    return DISABLE;
	}
	if(SCCB_SendByte( DEV_ADR +1))	return DISABLE;
	SCCB_delay();
	SCCB_delay();
	
	while(length)
	{
		*pBuffer = SCCB_ReceiveByte();
		if(length == 1)
	{
		SCCB_NoAck();
	}
		else
	{
	SCCB_Ack(); 
	}
		pBuffer++;
		length--;
	}
	SCCB_Stop();
	return ENABLE;
}


