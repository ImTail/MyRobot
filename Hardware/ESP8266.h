#ifndef __ESP8266_H
#define __ESP8266_H


void ESP8266_Init(void);

void ESP8266_RST_Hardware(void);
/*
	���ܣ�����ATָ�������λ
	���룺��
	�����AT������루��.û�н��յ��ظ���1.���պ����ڴ��Ļظ� 2.���ղ����ڴ��Ļظ� ��
	ע�⣺��
*/
uint8_t ESP8266_RST_Software(void);
/*
	���ܣ�дATָ��
	���룺ATָ����ڴ��Ļظ����ȴ����������
	�����AT������루��.û�н��յ��ظ���1.���պ����ڴ��Ļظ� 2.���ղ����ڴ��Ļظ� ��
*/
uint8_t ESP8266_W_AT(char* atCommand,char* response,uint16_t tryCounts);
/*
	���ܣ���ATָ����
	���룺�ڴ��Ļظ����ȴ����������
	�����AT������루��.û�н��յ��ظ���1.���պ����ڴ��Ļظ� 2.���ղ����ڴ��Ļظ� ��
*/
uint8_t ESP8266_R_AT(uint16_t tryCounts,char* response);

uint8_t ESP8266_Test(void);
/*
	���ܣ�����оƬ�Ĺ���ģʽ 
	���룺ģʽ����(1.Station 2.AP 3.AP+STA ��.����)���Ƿ񱣴�(0.�����浽flash !0.����)
	�����AT�������
*/
uint8_t ESP8266_S_Mode(uint8_t mode,uint8_t reserve);

/*
	���ܣ�stationģʽ����������AP
	���룺SSID��������Ƿ񱣴�(0.�����浽flash !0.����)
	�����AT�������
	ע�⣺����WIFI��оƬ����һ���ӳٲ��ܹ����ӳɹ���������OK���������оƬ�Ѿ�������WIFI���ᵼ�����ӷ���disconnected��
*/
uint8_t ESP8266_Station_Connect(char* id,char* password,uint8_t reserve);

/*
	���ܣ�stationģʽ�¶Ͽ�AP
	���룺��
	�����AT�������
	ע�⣺��
*/
uint8_t ESP8266_Station_Disconnect(void);

/*
	���ܣ���ѯ��ǰ�����������
	���룺��
	�����AT�������
	ע�⣺��
*/
uint8_t ESP8266_TCP_Status(void);
/*
	���ܣ�����TCP����
	���룺Զ��IP | TCP������ʱ��(0.�ر� 1~7200.n��)
	�����AT�������
	ע�⣺��
*/
uint8_t ESP8266_TCP_Start(char* ip,char* port,char* keepAliveTime);
/*
	���ܣ��˳�TCP����
	���룺��
	�����AT�������
	ע�⣺��
*/
uint8_t ESP8266_TCP_End(void);

/*
	���ܣ����ô���ģʽ
	���룺����ģʽ����(0.��ͨ���� !0.͸��)
	�����AT�������
	ע�⣺��
*/
uint8_t ESP8266_TCP_S_SendMode(uint8_t mode);

/*
	���ܣ�ֱ����ESP8266д�ַ���
	���룺�ַ���
	�������
	ע�⣺ͨ����͸��ģʽʹ��,��С�������16λ������
*/
void ESP8266_ContinuousMode_W_String(char* str);

/*
	���ܣ��˳�͸��ģʽ��д��+++
	���룺��
	�������
	ע�⣺��
*/
void ESP8266_ContinuousMode_Exit(void);
#endif
