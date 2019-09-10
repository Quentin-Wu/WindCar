#include "ov7670.h"
#include "my_sw_iic.h"
#include "ov7670config.h"
#include "myuart.h"
#include "led.h"
#include "mypwm.h"


void ov7670_VSYNC_EXTI_INT_FUNCTION ( void );

uint8_t ov7670_REG_NUM = sizeof(Sensor_Config)/sizeof(Sensor_Config[0]);	  /*结构体数组成员数目*/

volatile uint8_t ov7670_vsync = 10 ;	 /* 帧同步信号标志，在中断函数和main函数里面使用 */

uint8_t imgdata[60][80]={0};	
int cross_state;
uint8_t pos;

static void FIFO_GPIO_Config(void)
{

	/*(FIFO_OE--FIFO输出使能)*/
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIO_OE);
	GPIOPinTypeGPIOOutput(GPIO_PORT_BASE_OE, ov7670_GPIO_PIN_OE);

	/*(FIFO_WRST--FIFO写复位)*/
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIO_WRST);
	GPIOPinTypeGPIOOutput(GPIO_PORT_BASE_WRST, ov7670_GPIO_PIN_WRST);

	/*(FIFO_RRST--FIFO读复位) */
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIO_RRST);
	GPIOPinTypeGPIOOutput(GPIO_PORT_BASE_RRST, ov7670_GPIO_PIN_RRST);

	/*(FIFO_RCLK-FIFO读时钟)*/
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIO_RCLK);	
	GPIOPinTypeGPIOOutput(GPIO_PORT_BASE_RCLK, ov7670_GPIO_PIN_RCLK);

	/*(FIFO_WE--FIFO写使能)*/
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIO_WE);
	GPIOPinTypeGPIOOutput(GPIO_PORT_BASE_WE, ov7670_GPIO_PIN_WE);

	
	/*(FIFO_DATA--FIFO输出数据)*/
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIO_DATA);
	GPIOPinTypeGPIOInput(GPIO_PORT_BASE_DATA, ov7670_DATA_0_GPIO_PIN | 
																						ov7670_DATA_2_GPIO_PIN | ov7670_DATA_3_GPIO_PIN |
																						ov7670_DATA_4_GPIO_PIN | ov7670_DATA_5_GPIO_PIN | 
																						ov7670_DATA_6_GPIO_PIN | ov7670_DATA_7_GPIO_PIN);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIO_DATA_1);
	GPIOPinTypeGPIOInput(GPIO_PORT_BASE_DATA_1, ov7670_DATA_1_GPIO_PIN);

	FIFO_OE_L();	  					/*拉低使FIFO输出使能*/
  FIFO_WE_H();   						/*拉高使FIFO写允许*/

}

/************************************************
 * 函数名：VSYNC_GPIO_Config
 * 描述  ：ov7670 VSYNC中断相关配置
 * 输入  ：无
 * 输出  ：无
 * 注意  ：无
 ************************************************/
static void VSYNC_GPIO_Config(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIO_VSYNC);
	
	GPIOPinTypeGPIOInput(GPIO_PORT_BASE_VSYNC, ov7670_GPIO_PIN_VSYNC );
	GPIOPadConfigSet(GPIO_PORT_BASE_VSYNC, ov7670_GPIO_PIN_VSYNC , GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
	
	//中断 下降沿触发
	GPIOIntTypeSet(GPIO_PORT_BASE_VSYNC, ov7670_GPIO_PIN_VSYNC , GPIO_FALLING_EDGE );
	//使能中断
	GPIOIntEnable(GPIO_PORT_BASE_VSYNC, ov7670_GPIO_PIN_VSYNC );
	//注册中断处理句柄
	GPIOIntRegister(GPIO_PORT_BASE_VSYNC, ov7670_VSYNC_EXTI_INT_FUNCTION);
	//清除中断标志位
	GPIOIntClear(GPIO_PORT_BASE_VSYNC, ov7670_GPIO_PIN_VSYNC );
}

/* ov7670 场中断 服务程序 */
void ov7670_VSYNC_EXTI_INT_FUNCTION ( void )
{
		uint32_t IntStatus_raw;
		IntStatus_raw = GPIOIntStatus(GPIO_PORT_BASE_VSYNC, true);	//读取GPIO值
    if ( (IntStatus_raw & ov7670_GPIO_PIN_VSYNC)==ov7670_GPIO_PIN_VSYNC ) 	//检查EXTI_Line0线路上的中断请求是否发送到了NVIC 
    {
        if( ov7670_vsync == 0 )
        {
            FIFO_WRST_L(); 	                      //拉低使FIFO写(数据from摄像头)指针复位
            FIFO_WE_H();	                        //拉高使FIFO写允许
            
            ov7670_vsync = 1;	   	
            FIFO_WE_H();                          //使FIFO写允许
            FIFO_WRST_H();                        //允许使FIFO写(数据from摄像头)指针运动
        }
        else if( ov7670_vsync == 1 )
        {
            FIFO_WE_L(); 
						ov7670_vsync = 2;					//拉低使FIFO写暂停
        }
        //清除中断标志位
			GPIOIntClear(GPIO_PORT_BASE_VSYNC, ov7670_GPIO_PIN_VSYNC );       
    }    
}

/************************************************
 * 函数名：ov7670_GPIO_Config
 * 描述  ：初始化控制摄像头相接的IO
 * 输入  ：无
 * 输出  ：无
 * 注意  ：无
 ************************************************/
void ov7670_GPIO_Config(void)
{
	my_sw_iic_init();
	FIFO_GPIO_Config();
	VSYNC_GPIO_Config();	
}

/************************************************
 * 函数名：Sensor_Init
 * 描述  ：Sensor初始化
 * 输入  ：无
 * 输出  ：返回1成功，返回0失败
 * 注意  ：无
 ************************************************/
ErrorStatus ov7670_Init(void)
{
	uint16_t i = 0;
	uint8_t Sensor_IDCode = 0;	
	
	printf("ov7670 Register Config Start......\n");
	
	if( 0 == SCCB_WriteByte ( 0x12, 0x80 ) ) /*复位sensor */
	{
		printf("sccb write data error\n");		
		return ERROR ;
	}	

	if( 0 == SCCB_ReadByte( &Sensor_IDCode, 1, 0x0b ) )	 /* 读取sensor ID号*/
	{
		printf("read id faild\n");		
		return ERROR;
	}
	printf("Sensor ID is 0x%x\n", Sensor_IDCode);	
	
	if(Sensor_IDCode == ov7670_ID)
	{
		for( i = 0 ; i < ov7670_REG_NUM ; i++ )
		{
			if( 0 == SCCB_WriteByte(Sensor_Config[i][0], Sensor_Config[i][1]) )
			{                
				printf("write reg faild%d\n", Sensor_Config[i][0]);
				return ERROR;
			}
		}
	}
	else
	{
		return ERROR;
	}
	printf("ov7670 Register Config Success");
	
	return SUCCESS;
}


void ImagSend(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height)
{
	uint16_t i, j;
	uint16_t Camera_Data;
	uint8_t img[1]={0};
	
	#define CMD_WARE												1 //3表示虚拟示波器，1摄像头，2CCD
	uint8_t cmdf[2] = {CMD_WARE, ~CMD_WARE};
	uint8_t cmdr[2] = {~CMD_WARE, CMD_WARE};
	
	usart_putbuff(cmdf, sizeof(cmdf));	
	for(i = 0; i < height; i++)
	{
		for(j = 0; j < width; j++)
		{
			READ_FIFO_PIXEL(Camera_Data);		/* 从FIFO读出一个rgb565像素到Camera_Data变量 */			
			img[0]=(Camera_Data & 0x0000ff00) | (Camera_Data >>6);
//			img[0]=(Camera_Data & 0xff);
//			img[1]=(Camera_Data & 0xff00)>>8;
			usart_putbuff(img, sizeof(img));
		}
	}
	usart_putbuff(cmdr, sizeof(cmdr));

}



void ImagGet(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height)
{
	uint16_t i, j;
	uint16_t Camera_Data;
	uint8_t data;

		
	for(i = 0; i < height; i++)
	{
		for(j = 0; j < width; j++)
		{
			READ_FIFO_PIXEL(Camera_Data);		/* 从FIFO读出一个rgb565像素到Camera_Data变量 */
//			imgdata[i][j] = (Camera_Data & 0x0000ff00) | (Camera_Data >>6);
			data=(Camera_Data & 0x0000ff00) | (Camera_Data >>6);			
			if(data>206)
				imgdata[i][j]=0;
			else imgdata[i][j]=1;
//			printf("%d",imgdata[i][j]);
		}
//		printf("\n");
	}
	
//	for(i = 40; i < 70; i++)
//	{
//		for(j = 0; j < 60; j++)
//		{
//			printf("%d",imgdata[j][i]);
//		}
//		printf("\n");
//	}
	
//	#define CMD_WARE												1 //3表示虚拟示波器，1摄像头，2CCD
//	uint8_t cmdf[2] = {CMD_WARE, ~CMD_WARE};
//	uint8_t cmdr[2] = {~CMD_WARE, CMD_WARE};
//	
//	usart_putbuff(cmdf, sizeof(cmdf));	
//	for(i = 0; i < width; i++)
//	{
//		for(j = 0; j < height; j++)
//		{
//			img[0]=imgdata[j][i];
//			usart_putbuff(img, sizeof(img));
//		}
//	}
//	usart_putbuff(cmdr, sizeof(cmdr));
}

void ImagGet_Proc(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height)
{
	uint16_t i, j;
	uint16_t Camera_Data;
	uint8_t data;

	for(i = 0; i < height; i++)
	{
		for(j = 0; j < width; j++)
		{
			READ_FIFO_PIXEL(Camera_Data);		/* 从FIFO读出一个rgb565像素到Camera_Data变量 */
			data=(Camera_Data & 0x0000ff00) | (Camera_Data >>6);			
			imgdata[i][j] = (data>206? 1:0);
		}
	}
	
	
	int state = -1, black_left = 0, cur_width = 0;
	float cur_pos = 0;
	
		j=75; imgdata[59][75]=1;
		for(i = 0; i < 60; i++)
		{
			if (state == imgdata[i][j])
			{
				continue;
			}
			if (imgdata[i][j] == 0)//黑线左端
			{
				state = 0;
				black_left = i;
			}
			else//黑线右端
			{
				state = 1;
				if (i - black_left>cur_width)
				{
					cur_width = i - black_left;
					cur_pos = (i - 1 + black_left) / 2.0;
				}
			}
		}
		
		pos = cur_pos;
	if (cur_width>20 && cur_width<45)
	{
		//右直角弯
		if (cur_pos>40)
		{
			cross_state = -1;
		}
		else//左直角弯
		{
			cross_state = 1;
		}
	}
	else if (cur_width >= 45)
	{
		cross_state = 2;//十字
	}
	else if(cur_width<4)
	{
		cross_state = -2;
	}
	else
	{
		cross_state = 0;//正常
	}
	
	switch (cross_state)
	{
		case -1: printf("右直角 中线：%d\n",pos); break;
		case  1: printf("左直角 中线：%d\n",pos); break;
		case  2: printf("十字路 中线：%d\n",pos); break;
		case -2: printf("没有   中线：%d\n",pos); break;
		case  0: printf("正常   中线：%d\n",pos); break;
	}
		
}

void dispayImg(void)
{
	uint16_t i, j;
		i=75;
		for(j = 0; j < 60; j++)
		{
			printf("%d",imgdata[j][i]);
		}
		printf("\n");
}













