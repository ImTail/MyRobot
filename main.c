#include "stm32f10x.h"
#include "LED.h"
#include "Delay.h"
#include "OLED.h"
#include "Cartoon.h"
#include "LD3320.h"
#include "SYN6288.h"
#include "MyUSART.h"
#include "ESP8266.h"

#include "MyStringTool.h"

#include <string.h>

int main() 
{
	uint8_t res = 0;
	uint8_t ledFlag = 1;					//��¼�ƿ���,Ĭ�Ͽ�
	uint8_t speechFlag = 0;					//��¼�Ƿ�������������
	
	LED_Init();								//LDE��ʼ��
	/*IIC����*/
	OLED_Init();							//OLED��ʼ��
	/*SPI����*/
	LD3320_Init();							//��ʼ��LD3320����
	/*����ģ���ô�������*/
	MyUSART_Init();	
	SYN6288_Init();							//SYN6288��ʼ��
	ESP8266_Init();
	
	OLED_ClearAll();						//����
	//Cartoon_Raw();							//չʾ��ʼͼ��
	
	LD3320_RunOnce();						//��һ�����г�������һ��ʶ��
	uint8_t x = 0;
	
	
	
	while (1)
	{
		OLED_ShowChar(0,0,'0'+x++,1);
		if (x>=9)
		{
			x=0;
		}
		if (LD3320_G_HaveResFlag()==1)		
		{
			//resCount = LD3320_G_ResCount();
			res = LD3320_G_Res();
			OLED_ShowChar(0,1,'0'+res,1);
			Delay_ms(20);
			switch (res)
			{
				case 1:						//������䣬�ɼ���һ�λش�
					SYN6288_Speech("��");
					speechFlag = 1;
				break;
				case 2:
					SYN6288_Speech("�õ�");			
					speechFlag = 0;
				break;
				case 3:
					if (ledFlag == 0 && speechFlag == 1) 
					{
						speechFlag = 0;
						ledFlag =1;
						LED_ON();
						SYN6288_Speech("��[3]�Ѵ�"); 
					}						
				break;
				case 4:
					if (ledFlag == 1&& speechFlag == 1) 
					{
						speechFlag = 0;
						ledFlag =0;
						LED_OFF();
						SYN6288_Speech("��[3]�ѹر�"); 
					}
				break;
				case 5:
					if (speechFlag==1)
					{
						speechFlag = 0;
						SYN6288_Speech("[2]����[3]stm[n1]32[2]����[2]����[2]����[n2]"); 
					}
					
				break;
					case 6:
					if (speechFlag==1)
					{
						speechFlag = 0;
						uint8_t espRes = 0x01;
						espRes &= ESP8266_S_Mode(1,1);								//����ģʽΪstation,���洢
						espRes &= ESP8266_Station_Connect("NPC","123456789",0);		//����ָ��AP,���洢
						espRes &= ESP8266_TCP_Start("192.168.1.3","8080","0");
						OLED_ShowString(0,2,RX_Data_1,12,1);
						if (espRes==1)
						{
							SYN6288_Speech("�ɹ�"); 
						} else if (espRes==0)
						{
							SYN6288_Speech("ʧ��"); 
						} else 
						{
							SYN6288_Speech("����"); 
						}
						
					}
					
				break;
				default:;
			}
			LD3320_RunOnce();
		}
		Delay_ms(100);
	}
	
}
