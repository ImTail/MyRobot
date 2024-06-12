#include "stm32f10x.h"                  // Device header


uint8_t RX_Data_1[20];						
uint8_t RX_Flag_1;	

uint8_t RX_Data_3;						//���յ�����һ���ֽڵ�����
uint8_t RX_Flag_3;						//��־λ�������жϽ��յ�������


					
void MyUSART_Init(void)
{

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	//USART1��PA9��PB10��,PA9 TX    PA10 RX
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);	//����USART1ʱ��(APB2)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	//USART����PB10��PB11��,PB10 TX    PB11 RX
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);	//����USART3ʱ��(APB1)
	
	/*USART1����*/
	GPIO_InitTypeDef GPIO_Init_Structure;
	GPIO_Init_Structure.GPIO_Mode=GPIO_Mode_AF_PP;			//TX�����������
	GPIO_Init_Structure.GPIO_Pin=GPIO_Pin_9;
	GPIO_Init_Structure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Init_Structure);
	GPIO_Init_Structure.GPIO_Mode=GPIO_Mode_IPU;			//RX ��������
	GPIO_Init_Structure.GPIO_Pin=GPIO_Pin_10;
	GPIO_Init_Structure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Init_Structure);
	/*USART3����*/
	GPIO_Init_Structure.GPIO_Mode=GPIO_Mode_AF_PP;			//TX�����������
	GPIO_Init_Structure.GPIO_Pin=GPIO_Pin_10;
	GPIO_Init_Structure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_Init_Structure);
	GPIO_Init_Structure.GPIO_Mode=GPIO_Mode_IPU;			//RX ��������
	GPIO_Init_Structure.GPIO_Pin=GPIO_Pin_11;
	GPIO_Init_Structure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_Init_Structure);
	
	
	/*USART1��������*/
	USART_InitTypeDef USART_Init_Structrue;
	USART_Init_Structrue.USART_BaudRate=9600;										//������
	USART_Init_Structrue.USART_HardwareFlowControl=USART_HardwareFlowControl_None;	//����������
	USART_Init_Structrue.USART_Mode=USART_Mode_Rx | USART_Mode_Tx;					//�շ�ģʽ
	USART_Init_Structrue.USART_Parity=USART_Parity_No;								//��У��
	USART_Init_Structrue.USART_StopBits=USART_StopBits_1;							//ֹͣλ����Ϊ��
	USART_Init_Structrue.USART_WordLength=USART_WordLength_8b;						//�ֳ���λ
	USART_Init(USART1,&USART_Init_Structrue);
	/*USART3��������*/
	USART_Init_Structrue.USART_BaudRate=9600;										//������
	USART_Init_Structrue.USART_HardwareFlowControl=USART_HardwareFlowControl_None;	//����������
	USART_Init_Structrue.USART_Mode=USART_Mode_Rx | USART_Mode_Tx;					//�շ�ģʽ
	USART_Init_Structrue.USART_Parity=USART_Parity_No;								//��У��
	USART_Init_Structrue.USART_StopBits=USART_StopBits_1;							//ֹͣλ����Ϊ��
	USART_Init_Structrue.USART_WordLength=USART_WordLength_8b;						//�ֳ���λ
	USART_Init(USART3,&USART_Init_Structrue);
	
	/*�������ݽ��ռĴ����ǿ�ʱ�ж�*/
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);									//��λ���ȼ���������ʾ��Ӧ���ȼ��������ȼ���
	
	/*����USART1�ж����ȼ�*/
	NVIC_InitTypeDef NVIC_Init_Structrue;
	NVIC_Init_Structrue.NVIC_IRQChannel=USART1_IRQn;
	NVIC_Init_Structrue.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init_Structrue.NVIC_IRQChannelPreemptionPriority=0;						//û��������ռ���ȼ�������ֵ
	NVIC_Init_Structrue.NVIC_IRQChannelSubPriority=2;								//��Ӧ���ȼ�Ϊ2
	NVIC_Init(&NVIC_Init_Structrue);
	/*����USART3�ж����ȼ�*/
	NVIC_Init_Structrue.NVIC_IRQChannel=USART3_IRQn;
	NVIC_Init_Structrue.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init_Structrue.NVIC_IRQChannelPreemptionPriority=0;						//û��������ռ���ȼ�������ֵ
	NVIC_Init_Structrue.NVIC_IRQChannelSubPriority=1;								//��Ӧ���ȼ�Ϊ��
	NVIC_Init(&NVIC_Init_Structrue);
	
	/*ʹ�ܴ���*/
	USART_Cmd(USART3,ENABLE);
	USART_Cmd(USART3,ENABLE);
}

void MyUSART1_W_Byte(uint8_t data)
{
	USART_SendData(USART1,data);
	while (USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);//�����Ϳձ�־λΪ��������û�д�����ɣ��ȴ���ɡ�
}

void MyUSART3_W_Byte(uint8_t data)
{
	USART_SendData(USART3,data);
	while (USART_GetFlagStatus(USART3,USART_FLAG_TXE)==RESET);//�����Ϳձ�־λΪ��������û�д�����ɣ��ȴ���ɡ�
}

//��ȡRX_Flag_1,������λ
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

//��ȡRX_Flag_3,������λ
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

//�����ж�
void USART1_IRQHandler(void)
{
	if (USART_GetITStatus(USART1,USART_IT_RXNE)==SET)		//�ж��Ƿ���RXNE�������ж�
	{
		for (uint8_t i = 0;i<20;i++)
		{
			RX_Data_1[i] = USART_ReceiveData(USART1);
		}
		
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);		//����жϹ���λ
		
		RX_Flag_1 = 1;
	}
}


void USART3_IRQHandler(void)
{
	if (USART_GetITStatus(USART3,USART_IT_RXNE)==SET)		//�ж��Ƿ���RXNE�������ж�
	{
		RX_Data_3 = USART_ReceiveData(USART3);
		RX_Flag_3 = 1;
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);		//����жϹ���λ
	}
}


