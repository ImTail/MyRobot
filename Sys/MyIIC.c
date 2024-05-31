#include "stm32f10x.h"                  // Device header
#include "Delay.h"

#define MYIIC_CLK GPIO_Pin_8			
#define MYIIC_SDA GPIO_Pin_9

void MyIIC_Init(void)
{
	//使能GPIOB时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	//配置SCK SDA　为开漏输出
	GPIO_InitTypeDef GPIO_Init_Structure;
	GPIO_Init_Structure.GPIO_Mode=GPIO_Mode_Out_OD;
	GPIO_Init_Structure.GPIO_Pin=MYIIC_CLK | MYIIC_SDA;
	GPIO_Init_Structure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_Init_Structure);
	
	//无数据状态下，释放SCK 和 SDA
	GPIO_SetBits(GPIOB,MYIIC_CLK | MYIIC_SDA);
}

//写时钟线
void MyIIC_W_SCL(uint8_t value)
{
	GPIO_WriteBit(GPIOB,MYIIC_CLK,(BitAction)value);
	//Delay_us(10);
}

//写数据线
void MyIIC_W_SDA(uint8_t value)
{
	GPIO_WriteBit(GPIOB,MYIIC_SDA,(BitAction)value);
	//Delay_us(10);
}

uint8_t MyIIC_R_SDA(void)
{
	uint8_t bitRes;
	bitRes = GPIO_ReadInputDataBit(GPIOB,MYIIC_SDA);
	//Delay_us(10);
	return bitRes;
}

//开始时序
void MyIIC_Start(void) 
{	
	//释放两根线，以便兼容重复起始条件。
	MyIIC_W_SDA(0);
	MyIIC_W_SCL(0);
	
	MyIIC_W_SDA(0);
	MyIIC_W_SCL(0);
}

void MyIIC_End(void)
{
	MyIIC_W_SDA(0);				//提前释放SDA，以便进行结束时序
	
	MyIIC_W_SCL(1);
	MyIIC_W_SDA(1);
}

void MyIIC_W_Byte(uint8_t data)
{
	
	for (uint8_t i = 0 ; i < 8 ; i++) 
	{
		if ((data & (0x80>>i))) 
		{
			MyIIC_W_SDA(1);
		}
		else 
		{
			MyIIC_W_SDA(0);
		}
			MyIIC_W_SCL(1);
			Delay_us(10);	//将时释放10us等待从机读取数据
			MyIIC_W_SCL(0);
	}
}

uint8_t MyIIC_R_ACK(void)
{
	uint8_t ack;
	MyIIC_W_SDA(1);			//释放SDA给从机操纵
	MyIIC_W_SCL(1);			//释放SCL,让主机读取数据
	ack = MyIIC_R_SDA();	//将读取的ack接收
	MyIIC_W_SCL(0);			//重新拉低时钟
	return ack;
}
