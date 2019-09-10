#include "my_sw_iic.h"
#include "delay.h"

#define DEV_ADR  ADDR_OV7670 			 /*�豸��ַ����*/

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
 * ��������SCCB_Start
 * ����  ��SCCB��ʼ�ź�
 * ����  ����
 * ���  ����
 * ע��  ���ڲ�����        
 ********************************************************************/
static int SCCB_Start(void)
{
	SDA_H;
	SCL_H;
	SCCB_delay();
	if(!SDA_read)
	return DISABLE;	/* SDA��Ϊ�͵�ƽ������æ,�˳� */
	SDA_L;
	SCCB_delay();
	if(SDA_read) 
	return DISABLE;	/* SDA��Ϊ�ߵ�ƽ�����߳���,�˳� */
	SDA_L;
	SCCB_delay();
	return ENABLE;
}



/********************************************************************
 * ��������SCCB_Stop
 * ����  ��SCCBֹͣ�ź�
 * ����  ����
 * ���  ����
 * ע��  ���ڲ�����        
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
 * ��������SCCB_Ack
 * ����  ��SCCBӦ��ʽ
 * ����  ����
 * ���  ����
 * ע��  ���ڲ�����        
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
 * ��������SCCB_NoAck
 * ����  ��SCCB ��Ӧ��ʽ
 * ����  ����
 * ���  ����
 * ע��  ���ڲ�����        
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
// * ��������SCCB_WaitAck
// * ����  ��SCCB �ȴ�Ӧ��
// * ����  ����
// * ���  ������Ϊ:=1��ACK,=0��ACK
// * ע��  ���ڲ�����        
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
 * ��������SCCB_SendByte
 * ����  �����ݴӸ�λ����λ
 * ����  ��SendByte: ���͵�����
 * ���  ����
 * ע��  ���ڲ�����        
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
	
	SCCB_SDA_IN();			//����SDAΪ����
	SCCB_delay();
	
	SCL_H;
	SCCB_delay();
	
	if(SDA_read)	res=1;  //SDA=1����ʧ�ܣ�����1
	else	res=0;
	
	SCL_L;
	SCCB_SDA_OUT();
	
	return res;
}




 /******************************************************************
 * ��������SCCB_ReceiveByte
 * ����  �����ݴӸ�λ����λ
 * ����  ����
 * ���  ��SCCB���߷��ص�����
 * ע��  ���ڲ�����        
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
 * ��������SCCB_WriteByte
 * ����  ��дһ�ֽ�����
 * ����  ��- WriteAddress: ��д���ַ 	- SendByte: ��д������	- DeviceAddress: ��������
 * ���  ������Ϊ:=1�ɹ�д��,=0ʧ��
 * ע��  ����        
 *****************************************************************************************/           
u8 SCCB_WriteByte( uint16_t WriteAddress , uint8_t SendByte )
{		
//    if(!SCCB_Start())
//	{
//	    return DISABLE;
//	}
//    SCCB_SendByte( DEV_ADR );                    /* ������ַ */
//    if( !SCCB_WaitAck() )
//	{
//		SCCB_Stop(); 
//		return DISABLE;
//	}
//    SCCB_SendByte((uint8_t)(WriteAddress & 0x00FF));   /* ���õ���ʼ��ַ */      
//    SCCB_WaitAck();	
//    SCCB_SendByte(SendByte);
//    SCCB_WaitAck();   
//    SCCB_Stop(); 
//    return ENABLE;
	
    if(!SCCB_Start())
	{
	    return DISABLE;
	}
	if(SCCB_SendByte( DEV_ADR ))	return DISABLE;		//д������ַ ʧ�ܷ���1
	SCCB_delay();
	SCCB_delay();
	
	if(SCCB_SendByte(WriteAddress)) return DISABLE;				//д�Ĵ�����ַ ʧ�ܷ���1
	SCCB_delay();
	SCCB_delay();
	
	if(SCCB_SendByte(SendByte)) return DISABLE;				//д����				ʧ�ܷ���1
	SCCB_Stop();					//ֹͣ
	
	return ENABLE;
	
}

/******************************************************************************************************************
 * ��������SCCB_ReadByte
 * ����  ����ȡһ������
 * ����  ��- pBuffer: ��Ŷ������� 	- length: ����������	- ReadAddress: ��������ַ		 - DeviceAddress: ��������
 * ���  ������Ϊ:=1�ɹ�����,=0ʧ��
 * ע��  ����        
 **********************************************************************************************************************/           
u8 SCCB_ReadByte(uint8_t* pBuffer, uint16_t length, uint8_t ReadAddress)
{	
//    if(!SCCB_Start())
//	{
//	    return DISABLE;
//	}
//    SCCB_SendByte( DEV_ADR );         /* ������ַ */

//    if( !SCCB_WaitAck() )
//	{
//		SCCB_Stop(); 
//		return DISABLE;
//	}
//    SCCB_SendByte( ReadAddress );     /* ���õ���ʼ��ַ */      
//    SCCB_WaitAck();	
//    SCCB_Stop(); 
//    if(!SCCB_Start())
//	{
//		return DISABLE;
//	}
//    SCCB_SendByte( DEV_ADR + 1 );     /* ������ַ */ 
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
	if(SCCB_SendByte( DEV_ADR ))	return DISABLE;		//д������ַ ʧ�ܷ���1
	SCCB_delay();
	SCCB_delay();
	
	if(SCCB_SendByte(ReadAddress)) return DISABLE;				//д�Ĵ�����ַ ʧ�ܷ���1
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


