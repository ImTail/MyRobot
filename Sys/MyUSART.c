#include "stm32f10x.h"                  // Device header


uint8_t RX_Data_1[20];						
uint8_t RX_Flag_1;	

uint8_t RX_Data_3;						//接收的最新一个字节的数据
uint8_t RX_Flag_3;						//标志位，串口中断接收到了数据


					
void MyUSART_Init(void)
{

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	//USART1在PA9和PB10上,PA9 TX    PA10 RX
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);	//开启USART1时钟(APB2)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	//USART３在PB10和PB11上,PB10 TX    PB11 RX
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);	//开启USART3时钟(APB1)
	
	/*USART1引脚*/
	GPIO_InitTypeDef GPIO_Init_Structure;
	GPIO_Init_Structure.GPIO_Mode=GPIO_Mode_AF_PP;			//TX复用推挽输出
	GPIO_Init_Structure.GPIO_Pin=GPIO_Pin_9;
	GPIO_Init_Structure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Init_Structure);
	GPIO_Init_Structure.GPIO_Mode=GPIO_Mode_IPU;			//RX 上拉输入
	GPIO_Init_Structure.GPIO_Pin=GPIO_Pin_10;
	GPIO_Init_Structure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Init_Structure);
	/*USART3引脚*/
	GPIO_Init_Structure.GPIO_Mode=GPIO_Mode_AF_PP;			//TX复用推挽输出
	GPIO_Init_Structure.GPIO_Pin=GPIO_Pin_10;
	GPIO_Init_Structure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_Init_Structure);
	GPIO_Init_Structure.GPIO_Mode=GPIO_Mode_IPU;			//RX 上拉输入
	GPIO_Init_Structure.GPIO_Pin=GPIO_Pin_11;
	GPIO_Init_Structure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_Init_Structure);
	
	
	/*USART1串口设置*/
	USART_InitTypeDef USART_Init_Structrue;
	USART_Init_Structrue.USART_BaudRate=9600;										//波特率
	USART_Init_Structrue.USART_HardwareFlowControl=USART_HardwareFlowControl_None;	//不进行流控
	USART_Init_Structrue.USART_Mode=USART_Mode_Rx | USART_Mode_Tx;					//收发模式
	USART_Init_Structrue.USART_Parity=USART_Parity_No;								//无校验
	USART_Init_Structrue.USART_StopBits=USART_StopBits_1;							//停止位长度为１
	USART_Init_Structrue.USART_WordLength=USART_WordLength_8b;						//字长８位
	USART_Init(USART1,&USART_Init_Structrue);
	/*USART3串口设置*/
	USART_Init_Structrue.USART_BaudRate=9600;										//波特率
	USART_Init_Structrue.USART_HardwareFlowControl=USART_HardwareFlowControl_None;	//不进行流控
	USART_Init_Structrue.USART_Mode=USART_Mode_Rx | USART_Mode_Tx;					//收发模式
	USART_Init_Structrue.USART_Parity=USART_Parity_No;								//无校验
	USART_Init_Structrue.USART_StopBits=USART_StopBits_1;							//停止位长度为１
	USART_Init_Structrue.USART_WordLength=USART_WordLength_8b;						//字长８位
	USART_Init(USART3,&USART_Init_Structrue);
	
	/*配置数据接收寄存器非空时中断*/
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);									//４位优先级都用来表示响应优先级（从优先级）
	
	/*配置USART1中断优先级*/
	NVIC_InitTypeDef NVIC_Init_Structrue;
	NVIC_Init_Structrue.NVIC_IRQChannel=USART1_IRQn;
	NVIC_Init_Structrue.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init_Structrue.NVIC_IRQChannelPreemptionPriority=0;						//没有设置抢占优先级，任意值
	NVIC_Init_Structrue.NVIC_IRQChannelSubPriority=2;								//响应优先级为2
	NVIC_Init(&NVIC_Init_Structrue);
	/*配置USART3中断优先级*/
	NVIC_Init_Structrue.NVIC_IRQChannel=USART3_IRQn;
	NVIC_Init_Structrue.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init_Structrue.NVIC_IRQChannelPreemptionPriority=0;						//没有设置抢占优先级，任意值
	NVIC_Init_Structrue.NVIC_IRQChannelSubPriority=1;								//响应优先级为１
	NVIC_Init(&NVIC_Init_Structrue);
	
	/*使能串口*/
	USART_Cmd(USART3,ENABLE);
	USART_Cmd(USART3,ENABLE);
}

void MyUSART1_W_Byte(uint8_t data)
{
	USART_SendData(USART1,data);
	while (USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);//当发送空标志位为０，代表还没有传输完成，等待完成。
}

void MyUSART3_W_Byte(uint8_t data)
{
	USART_SendData(USART3,data);
	while (USART_GetFlagStatus(USART3,USART_FLAG_TXE)==RESET);//当发送空标志位为０，代表还没有传输完成，等待完成。
}

//获取RX_Flag_1,读后置位
uint8_t MyUSART1_G_Flag(void)
{
	if (RX_Flag_1==1)
	{
		RX_Flag_1=0;
		return 1;
	} else 
	{
		return 0;
	}
}

//获取RX_Flag_3,读后置位
uint8_t MyUSART3_G_Flag(void)
{
	if (RX_Flag_3==1)
	{
		RX_Flag_3=0;
		return 1;
	} else 
	{
		return 0;
	}
}

//串口中断
void USART1_IRQHandler(void)
{
	if (USART_GetITStatus(USART1,USART_IT_RXNE)==SET)		//判断是否是RXNE触发的中断
	{
		for (uint8_t i = 0;i<20;i++)
		{
			RX_Data_1[i] = USART_ReceiveData(USART1);
		}
		
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);		//清除中断挂起位
		
		RX_Flag_1 = 1;
	}
}


void USART3_IRQHandler(void)
{
	if (USART_GetITStatus(USART3,USART_IT_RXNE)==SET)		//判断是否是RXNE触发的中断
	{
		RX_Data_3 = USART_ReceiveData(USART3);
		RX_Flag_3 = 1;
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);		//清除中断挂起位
	}
}


