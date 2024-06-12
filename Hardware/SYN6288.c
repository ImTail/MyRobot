#include "stm32f10x.h"                  // Device header
#include "MyUSART.h"
#include "SYN6288.h"
#include "Delay.h"

#include <string.h>

#define SYN6288_FrameHead 0xFD
#define SYN6288_Command_Speech 0x01
#define SYN6288_Command_SetRate 0x31
#define SYN6288_Command_Stop 0x02
#define SYN6288_Command_Pause 0x03
#define SYN6288_Command_Resume 0x04
#define SYN6288_Command_CheckStatus 0x21
#define SYN6288_Command_PowerDown 0x88

uint8_t backMusic = 0;			//模块内置了背景音乐0~15,0代表没有背景音乐
uint8_t charCode = 0;			//字节中的编码选项,0是GB2312


void SYN6288_Init(void)
{
	char initSetting[] = "[d][v8][m2][t5][y0][x1][b0][o0][n2]";//初始化设置参数
	SYN6288_Speech(initSetting);
	Delay_ms(8);
}

void SYN6288_W_Data(uint8_t command,uint8_t commandVal,char stringData[],uint16_t stringLen)
{
	uint8_t frameCheck = 0x00;//异或校验
	
	MyUSART3_W_Byte(SYN6288_FrameHead);					//帧头
	frameCheck ^= SYN6288_FrameHead;
	
	uint16_t dataZoomLen = stringLen +3;	  //加上命令　命令参数　校验共三个字节的长度,为数据区总长度
	MyUSART3_W_Byte((uint8_t)(dataZoomLen>>8));//数据区长度 高位
	MyUSART3_W_Byte((uint8_t)(dataZoomLen));	  //数据区长度 低位
	frameCheck ^= (uint8_t)(dataZoomLen>>8);
	frameCheck ^= (uint8_t)(dataZoomLen);
	
	/*数据区*/
	MyUSART3_W_Byte(command);				//命令
	MyUSART3_W_Byte(commandVal);				//命令参数
	frameCheck ^= command;
	frameCheck ^= commandVal;
	
	for (uint16_t i = 0 ; i < stringLen ; i++)
	{
		MyUSART3_W_Byte(stringData[i]);
		frameCheck ^= stringData[i];
	}
	
	MyUSART3_W_Byte(frameCheck);				//异或校验
}

uint8_t SYN6288_R_Data(void)
{
	if (MyUSART3_G_Flag()==1)
	{
		return RX_Data_3;
	} else
	{
		return 0;
	}
}

void SYN6288_Speech(char stringData[])
{
	uint8_t commandVal = 0x00;
	commandVal |= backMusic << 3;
	commandVal |= charCode;
	SYN6288_W_Data(SYN6288_Command_Speech,commandVal,stringData,strlen(stringData));
}
