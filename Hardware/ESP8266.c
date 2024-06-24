#include "stm32f10x.h"                  // Device header
#include "MyUSART.h"
#include "ESP8266.h"
#include "OLED.h"
#include "Delay.h"
#include "MyStringTool.h"

#include <string.h>

#define ESP8266_TryCounts 5

void ESP8266_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_Init_Structrue;
	GPIO_Init_Structrue.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init_Structrue.GPIO_Pin=GPIO_Pin_8;
	GPIO_Init_Structrue.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Init_Structrue);
	GPIO_SetBits(GPIOA,GPIO_Pin_8);
	ESP8266_RST_Hardware();								//初始化时，软件复位ESP8266
}

void ESP8266_RST_Hardware(void)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_8);
	GPIO_ResetBits(GPIOA,GPIO_Pin_8);
	Delay_ms(10);
	GPIO_SetBits(GPIOA,GPIO_Pin_8);
}

/*
	功能：发送AT指令软件复位
	输入：无
	输出：AT结果代码（０.没有接收到回复　1.接收含有期待的回复 2.接收不含期待的回复 ）
	注意：无
*/
uint8_t ESP8266_RST_Software(void)
{
	return ESP8266_W_AT("AT+RST","OK",ESP8266_TryCounts);
}

/*
	功能：写AT指令
	输入：AT指令｜期待的回复｜等待结果最多次数
	输出：AT结果代码（０.没有接收到回复　1.接收含有期待的回复 2.接收不含期待的回复 ）
*/
uint8_t ESP8266_W_AT(char* atCommand,char* response,uint16_t tryCounts)
{
	RX_P_1 = 0;//每次发送指令，将缓冲区指向头部。
	uint8_t commandLen = strlen(atCommand) ;
	for (uint8_t i = 0 ; i < commandLen ; i++)			//按字节发送AT指令
	{
		MyUSART1_W_Byte(atCommand[i]);
		OLED_ShowChar(i,3,atCommand[i],1);
	}	
	
	MyUSART1_W_Byte('\r'); 
    MyUSART1_W_Byte('\n'); 								//ESP8266的AT指令要求结尾
	return ESP8266_R_AT(tryCounts,response);
}

/*
	功能：读AT指令结果
	输入：期待的回复｜等待结果最多次数
	输出：AT结果代码（０.没有接收到回复　1.接收含有期待的回复 2.接收不含期待的回复 ）
*/
uint8_t ESP8266_R_AT(uint16_t tryCounts,char* response)
{
	
	for (uint8_t count = 0;count < tryCounts ; count++)	//尝试count次读取结果,
	{
		Delay_ms(10);
		if (MyUSART1_G_Flag()==1)
		{
			MyStringTool_Remove_Emp(RX_Data_1);			//接收字符串可能前面也带有\0　需要自定义特殊函数去除
			if (strstr(RX_Data_1,response) != NULL)		//如果结果RX_Data_1中存在responsse   
			{
				return 1;
			}
			return 2;
		}
	}
	return 0;
}

uint8_t ESP8266_Test(void)
{
	return ESP8266_W_AT("AT","OK",ESP8266_TryCounts);
}

/*
	功能：设置芯片的工作模式 
	输入：模式代号(1.Station 2.AP 3.AP+STA ４.测试)｜是否保存(0.不保存到flash !0.保存)
	输出：AT结果代码
*/
uint8_t ESP8266_S_Mode(uint8_t mode,uint8_t reserve)
{	
	char command[20]; 					//直接使用指针需要用malloc初始化，否则报错崩溃
	if (reserve==0)
	{
		strcpy(command,"AT+CWMODE_CUR=");
	}
	else
	{	
		strcpy(command,"AT+CWMODE_DEF=");
	}
	
	if 
	(mode==1)
	{
		strcat(command,"1");
	} 
	else if 
	(mode==2)
	{
		strcat(command,"2");
	}
	else if
	(mode==3)
	{
		strcat(command,"3");
	}
	else if 
	(mode==4)
	{
		strcat(command,"?");
	} 
	else
	{
		return 0;
	}
	
	return ESP8266_W_AT(command,"OK",ESP8266_TryCounts);	
}

/*
	功能：station模式下连接其他AP
	输入：SSID｜密码｜是否保存(0.不保存到flash !0.保存)
	输出：AT结果代码
	注意：连接WIFI，芯片会有一定延迟才能够连接成功，并发送OK。并且如果芯片已经连接了WIFI，会导致连接返回disconnected。
*/
uint8_t ESP8266_Station_Connect(char* id,char* password,uint8_t reserve)
{
	char command[50]; 					//直接使用指针需要用malloc初始化，否则报错崩溃
	if (reserve==0)
	{
		strcpy(command,"AT+CWJAP_CUR=");
	}
	else
	{	
		strcpy(command,"AT+CWJAP_DEF=");
	}
	strcat(command,"\"");
	strcat(command,id);
	strcat(command,"\"");
	strcat(command,",");
	strcat(command,"\"");
	strcat(command,password);
	strcat(command,"\"");
	
	ESP8266_W_AT(command,"OK",ESP8266_TryCounts);
	Delay_ms(3000);//等待芯片5s连接网络的时间后，读取串口缓冲区的消息.
	
	return ESP8266_R_AT(ESP8266_TryCounts,"OK");
}

/*
	功能：station模式下断开AP
	输入：无
	输出：AT结果代码
	注意：无
*/
uint8_t ESP8266_Station_Disconnect(void)
{
	return ESP8266_W_AT("AT+CWQAP","OK",ESP8266_TryCounts);
}

/*
	功能：查询当前网络连接情况
	输入：无
	输出：AT结果代码
	注意：无
*/
uint8_t ESP8266_TCP_Status(void)
{
	return ESP8266_W_AT("AT+CIPSATUS","OK",ESP8266_TryCounts);
}

/*
	功能：建立TCP连接
	输入：远端IP | 远端端口| TCP存活侦测时间(0.关闭 1~7200.n秒)
	输出：AT结果代码
	注意：无
*/
uint8_t ESP8266_TCP_Start(char* ip,char* port,char* keepAliveTime)
{
	char command[50] = "AT+CIPSTART=";
	strcat(command,"\"");
	strcat(command,"TCP");
	strcat(command,"\"");
	strcat(command,",");
	
	strcat(command,"\"");
	strcat(command,ip);
	strcat(command,"\"");
	strcat(command,",");
	
	strcat(command,port);
	strcat(command,",");
	
	strcat(command,keepAliveTime);
	
	ESP8266_W_AT(command,"OK",ESP8266_TryCounts);
	Delay_ms(3000);//等待芯片5s连接网络的时间后，读取串口缓冲区的消息.
	
	return ESP8266_R_AT(ESP8266_TryCounts,"OK");
}

/*
	功能：退出TCP连接
	输入：无
	输出：AT结果代码
	注意：无
*/
uint8_t ESP8266_TCP_End(void)
{
	return ESP8266_W_AT("AT+CIPCLOSE","OK",ESP8266_TryCounts);
}


/*
	功能：设置传输模式
	输入：传输模式代码(0.普通传输 !0.透传)
	输出：AT结果代码
	注意：无
*/
uint8_t ESP8266_TCP_S_SendMode(uint8_t mode)
{
	if (mode==0)
	{
		return ESP8266_W_AT("AT+CIPMODE=0","OK",ESP8266_TryCounts);
	}
	else
	{
		return ESP8266_W_AT("AT+CIPMODE=1","OK",ESP8266_TryCounts);
	}
}


/*
	功能：直接往ESP8266写字符串
	输入：字符串
	输出：无
	注意：通常在透传模式使用,大小不能溢出16位二进制
*/
void ESP8266_ContinuousMode_W_String(char* str)
{
	uint16_t len = strlen(str);
	for (uint16_t i = 0 ; i < len ; i++)
	{
		MyUSART1_W_Byte(str[i]);
	}
}

/*
	功能：退出透传模式，写入+++
	输入：无
	输出：无
	注意：无
*/
void ESP8266_ContinuousMode_Exit(void)
{
	ESP8266_ContinuousMode_W_String("+++");
}


