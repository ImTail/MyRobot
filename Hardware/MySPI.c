#include "stm32f10x.h"                  // Device header


/*计划使用模式2　SPI*/
#define MYSPI_GPIO GPIOA
#define MYSPI_SS GPIO_Pin_4
#define MYSPI_SCL GPIO_Pin_5
#define MYSPI_MISO GPIO_Pin_6
#define MYSPI_MOSI GPIO_Pin_7

void MySPI_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	
	GPIO_InitTypeDef GPIO_Init_Structrue;
	/*SS,SCL,MOSI推挽输出*/
	GPIO_Init_Structrue.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init_Structrue.GPIO_Pin=MYSPI_SS | MYSPI_SCL | MYSPI_MOSI;
	GPIO_Init_Structrue.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(MYSPI_GPIO,&GPIO_Init_Structrue);
	/*MISO上拉输入*/
	GPIO_Init_Structrue.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init_Structrue.GPIO_Pin=MYSPI_MISO;
	GPIO_Init_Structrue.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(MYSPI_GPIO,&GPIO_Init_Structrue);
	
	GPIO_SetBits(MYSPI_GPIO,MYSPI_SS);//空闲状态拉高SS
	GPIO_SetBits(MYSPI_GPIO,MYSPI_SCL);//空闲状态拉高时钟线
}

void MySPI_W_SS(uint8_t data)
{
	GPIO_WriteBit(MYSPI_GPIO,MYSPI_SS,(BitAction)data);
}

void MySPI_W_SCL(uint8_t data)
{
	GPIO_WriteBit(MYSPI_GPIO,MYSPI_SCL,(BitAction)data);
}

void MySPI_W_MOSI(uint8_t data)
{
	GPIO_WriteBit(MYSPI_GPIO,MYSPI_MOSI,(BitAction)data);
}

uint8_t MySPI_R_MISO(void)
{
	return GPIO_ReadInputDataBit(MYSPI_GPIO,MYSPI_MISO);
}

void MySPI_Start(void)
{
	MySPI_W_SS(0);					//拉低唯一的SS，作为起始信号选中从机。
}

void MySPI_End(void)
{
	MySPI_W_SS(1);
}

uint8_t MySPI_SwapByte(uint8_t ByteSend)
{
	uint8_t ByteReceive = 0x00;
	for (uint8_t i = 0 ; i < 8 ; i++)
	{
		MySPI_W_MOSI(ByteSend & (0x80 >> i));//移出数据
		MySPI_W_SCL(0);	
		ByteReceive|= (MySPI_R_MISO()<<(7-i));//从高位读到低位，从高位放到低位
		MySPI_W_SCL(1);
	}
	return ByteReceive;
}

