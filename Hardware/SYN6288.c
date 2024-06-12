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

uint8_t backMusic = 0;			//ģ�������˱�������0~15,0����û�б�������
uint8_t charCode = 0;			//�ֽ��еı���ѡ��,0��GB2312


void SYN6288_Init(void)
{
	char initSetting[] = "[d][v8][m2][t5][y0][x1][b0][o0][n2]";//��ʼ�����ò���
	SYN6288_Speech(initSetting);
	Delay_ms(8);
}

void SYN6288_W_Data(uint8_t command,uint8_t commandVal,char stringData[],uint16_t stringLen)
{
	uint8_t frameCheck = 0x00;//���У��
	
	MyUSART3_W_Byte(SYN6288_FrameHead);					//֡ͷ
	frameCheck ^= SYN6288_FrameHead;
	
	uint16_t dataZoomLen = stringLen +3;	  //����������������У�鹲�����ֽڵĳ���,Ϊ�������ܳ���
	MyUSART3_W_Byte((uint8_t)(dataZoomLen>>8));//���������� ��λ
	MyUSART3_W_Byte((uint8_t)(dataZoomLen));	  //���������� ��λ
	frameCheck ^= (uint8_t)(dataZoomLen>>8);
	frameCheck ^= (uint8_t)(dataZoomLen);
	
	/*������*/
	MyUSART3_W_Byte(command);				//����
	MyUSART3_W_Byte(commandVal);				//�������
	frameCheck ^= command;
	frameCheck ^= commandVal;
	
	for (uint16_t i = 0 ; i < stringLen ; i++)
	{
		MyUSART3_W_Byte(stringData[i]);
		frameCheck ^= stringData[i];
	}
	
	MyUSART3_W_Byte(frameCheck);				//���У��
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
