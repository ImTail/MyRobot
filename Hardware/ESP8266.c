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
	ESP8266_RST_Hardware();								//��ʼ��ʱ�������λESP8266
}

void ESP8266_RST_Hardware(void)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_8);
	GPIO_ResetBits(GPIOA,GPIO_Pin_8);
	Delay_ms(10);
	GPIO_SetBits(GPIOA,GPIO_Pin_8);
}

/*
	���ܣ�����ATָ�������λ
	���룺��
	�����AT������루��.û�н��յ��ظ���1.���պ����ڴ��Ļظ� 2.���ղ����ڴ��Ļظ� ��
	ע�⣺��
*/
uint8_t ESP8266_RST_Software(void)
{
	return ESP8266_W_AT("AT+RST","OK",ESP8266_TryCounts);
}

/*
	���ܣ�дATָ��
	���룺ATָ����ڴ��Ļظ����ȴ����������
	�����AT������루��.û�н��յ��ظ���1.���պ����ڴ��Ļظ� 2.���ղ����ڴ��Ļظ� ��
*/
uint8_t ESP8266_W_AT(char* atCommand,char* response,uint16_t tryCounts)
{
	RX_P_1 = 0;//ÿ�η���ָ���������ָ��ͷ����
	uint8_t commandLen = strlen(atCommand) ;
	for (uint8_t i = 0 ; i < commandLen ; i++)			//���ֽڷ���ATָ��
	{
		MyUSART1_W_Byte(atCommand[i]);
		OLED_ShowChar(i,3,atCommand[i],1);
	}	
	
	MyUSART1_W_Byte('\r'); 
    MyUSART1_W_Byte('\n'); 								//ESP8266��ATָ��Ҫ���β
	return ESP8266_R_AT(tryCounts,response);
}

/*
	���ܣ���ATָ����
	���룺�ڴ��Ļظ����ȴ����������
	�����AT������루��.û�н��յ��ظ���1.���պ����ڴ��Ļظ� 2.���ղ����ڴ��Ļظ� ��
*/
uint8_t ESP8266_R_AT(uint16_t tryCounts,char* response)
{
	
	for (uint8_t count = 0;count < tryCounts ; count++)	//����count�ζ�ȡ���,
	{
		Delay_ms(10);
		if (MyUSART1_G_Flag()==1)
		{
			MyStringTool_Remove_Emp(RX_Data_1);			//�����ַ�������ǰ��Ҳ����\0����Ҫ�Զ������⺯��ȥ��
			if (strstr(RX_Data_1,response) != NULL)		//������RX_Data_1�д���responsse   
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
	���ܣ�����оƬ�Ĺ���ģʽ 
	���룺ģʽ����(1.Station 2.AP 3.AP+STA ��.����)���Ƿ񱣴�(0.�����浽flash !0.����)
	�����AT�������
*/
uint8_t ESP8266_S_Mode(uint8_t mode,uint8_t reserve)
{	
	char command[20]; 					//ֱ��ʹ��ָ����Ҫ��malloc��ʼ�������򱨴����
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
	���ܣ�stationģʽ����������AP
	���룺SSID��������Ƿ񱣴�(0.�����浽flash !0.����)
	�����AT�������
	ע�⣺����WIFI��оƬ����һ���ӳٲ��ܹ����ӳɹ���������OK���������оƬ�Ѿ�������WIFI���ᵼ�����ӷ���disconnected��
*/
uint8_t ESP8266_Station_Connect(char* id,char* password,uint8_t reserve)
{
	char command[50]; 					//ֱ��ʹ��ָ����Ҫ��malloc��ʼ�������򱨴����
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
	Delay_ms(3000);//�ȴ�оƬ5s���������ʱ��󣬶�ȡ���ڻ���������Ϣ.
	
	return ESP8266_R_AT(ESP8266_TryCounts,"OK");
}

/*
	���ܣ�stationģʽ�¶Ͽ�AP
	���룺��
	�����AT�������
	ע�⣺��
*/
uint8_t ESP8266_Station_Disconnect(void)
{
	return ESP8266_W_AT("AT+CWQAP","OK",ESP8266_TryCounts);
}

/*
	���ܣ���ѯ��ǰ�����������
	���룺��
	�����AT�������
	ע�⣺��
*/
uint8_t ESP8266_TCP_Status(void)
{
	return ESP8266_W_AT("AT+CIPSATUS","OK",ESP8266_TryCounts);
}

/*
	���ܣ�����TCP����
	���룺Զ��IP | Զ�˶˿�| TCP������ʱ��(0.�ر� 1~7200.n��)
	�����AT�������
	ע�⣺��
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
	Delay_ms(3000);//�ȴ�оƬ5s���������ʱ��󣬶�ȡ���ڻ���������Ϣ.
	
	return ESP8266_R_AT(ESP8266_TryCounts,"OK");
}

/*
	���ܣ��˳�TCP����
	���룺��
	�����AT�������
	ע�⣺��
*/
uint8_t ESP8266_TCP_End(void)
{
	return ESP8266_W_AT("AT+CIPCLOSE","OK",ESP8266_TryCounts);
}


/*
	���ܣ����ô���ģʽ
	���룺����ģʽ����(0.��ͨ���� !0.͸��)
	�����AT�������
	ע�⣺��
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
	���ܣ�ֱ����ESP8266д�ַ���
	���룺�ַ���
	�������
	ע�⣺ͨ����͸��ģʽʹ��,��С�������16λ������
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
	���ܣ��˳�͸��ģʽ��д��+++
	���룺��
	�������
	ע�⣺��
*/
void ESP8266_ContinuousMode_Exit(void)
{
	ESP8266_ContinuousMode_W_String("+++");
}


