#ifndef __ESP8266_H
#define __ESP8266_H


void ESP8266_Init(void);

void ESP8266_RST_Hardware(void);
/*
	功能：发送AT指令软件复位
	输入：无
	输出：AT结果代码（０.没有接收到回复　1.接收含有期待的回复 2.接收不含期待的回复 ）
	注意：无
*/
uint8_t ESP8266_RST_Software(void);
/*
	功能：写AT指令
	输入：AT指令｜期待的回复｜等待结果最多次数
	输出：AT结果代码（０.没有接收到回复　1.接收含有期待的回复 2.接收不含期待的回复 ）
*/
uint8_t ESP8266_W_AT(char* atCommand,char* response,uint16_t tryCounts);
/*
	功能：读AT指令结果
	输入：期待的回复｜等待结果最多次数
	输出：AT结果代码（０.没有接收到回复　1.接收含有期待的回复 2.接收不含期待的回复 ）
*/
uint8_t ESP8266_R_AT(uint16_t tryCounts,char* response);

uint8_t ESP8266_Test(void);
/*
	功能：设置芯片的工作模式 
	输入：模式代号(1.Station 2.AP 3.AP+STA ４.测试)｜是否保存(0.不保存到flash !0.保存)
	输出：AT结果代码
*/
uint8_t ESP8266_S_Mode(uint8_t mode,uint8_t reserve);

/*
	功能：station模式下连接其他AP
	输入：SSID｜密码｜是否保存(0.不保存到flash !0.保存)
	输出：AT结果代码
	注意：连接WIFI，芯片会有一定延迟才能够连接成功，并发送OK。并且如果芯片已经连接了WIFI，会导致连接返回disconnected。
*/
uint8_t ESP8266_Station_Connect(char* id,char* password,uint8_t reserve);

/*
	功能：station模式下断开AP
	输入：无
	输出：AT结果代码
	注意：无
*/
uint8_t ESP8266_Station_Disconnect(void);

/*
	功能：查询当前网络连接情况
	输入：无
	输出：AT结果代码
	注意：无
*/
uint8_t ESP8266_TCP_Status(void);
/*
	功能：建立TCP连接
	输入：远端IP | TCP存活侦测时间(0.关闭 1~7200.n秒)
	输出：AT结果代码
	注意：无
*/
uint8_t ESP8266_TCP_Start(char* ip,char* port,char* keepAliveTime);
/*
	功能：退出TCP连接
	输入：无
	输出：AT结果代码
	注意：无
*/
uint8_t ESP8266_TCP_End(void);

/*
	功能：设置传输模式
	输入：传输模式代码(0.普通传输 !0.透传)
	输出：AT结果代码
	注意：无
*/
uint8_t ESP8266_TCP_S_SendMode(uint8_t mode);

/*
	功能：直接往ESP8266写字符串
	输入：字符串
	输出：无
	注意：通常在透传模式使用,大小不能溢出16位二进制
*/
void ESP8266_ContinuousMode_W_String(char* str);

/*
	功能：退出透传模式，写入+++
	输入：无
	输出：无
	注意：无
*/
void ESP8266_ContinuousMode_Exit(void);
#endif
