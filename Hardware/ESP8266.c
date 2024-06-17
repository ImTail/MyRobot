#include "stm32f10x.h"                  // Device header
#include "MyUSART.h"
#include "ESP8266.h"
#include "OLED.h"
#include "Delay.h"
#include "MyStringTool.h"


#include <string.h>

void ESP8266_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_Init_Structrue;
	GPIO_Init_Structrue.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init_Structrue.GPIO_Pin=GPIO_Pin_8;
	GPIO_Init_Structrue.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Init_Structrue);
	GPIO_SetBits(GPIOA,GPIO_Pin_8);
	//ESP8266_RST();
	
}

void ESP8266_RST(void)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_8);
	GPIO_ResetBits(GPIOA,GPIO_Pin_8);
	Delay_ms(10);
	GPIO_SetBits(GPIOA,GPIO_Pin_8);
}

uint8_t ESP8266_W_AT(char* atCommand,char* response,uint16_t tryCounts)
{
	RX_P_1 = 0;//每次发送指令，将缓冲区指向头部。
	uint8_t commandLen = strlen(atCommand) ;
	for (uint8_t i = 0 ; i < commandLen ; i++)
	{
		MyUSART1_W_Byte(atCommand[i]);
		OLED_ShowChar(i,3,atCommand[i],1);
	}	
	
	MyUSART1_W_Byte('\r'); // 使用回车符作为结束符
    MyUSART1_W_Byte('\n'); // 也可以使用'\r\n'，具体取决于ESP8266的AT指令要求
	for (uint8_t count = 0;count < tryCounts ; count++)
	{
		Delay_ms(10);
		if (MyUSART1_G_Flag()==1)
		{
			MyStringTool_Remove_Emp(RX_Data_1);//接收字符串可能前面也带有\0　需要自定义特殊函数去除
			if (strcmp(RX_Data_1,response)==0)
			{
				return 1;
			}
			return 2;
		}
	}
	return 0;
}
