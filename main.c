#include "stm32f10x.h"
#include "LED.h"
#include "Delay.h"
#include "OLED.h"
#include "Cartoon.h"
#include "LD3320.h"
#include "SYN6288.h"
#include "MyUSART.h"


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
	
	
	OLED_ClearAll();						//����
	//Cartoon_Raw();							//չʾ��ʼͼ��
	
	LD3320_RunOnce();						//��һ�����г�������һ��ʶ��
	
	while (1)
	{
		
		
		if (LD3320_G_HaveResFlag()==1)		
		{
			//resCount = LD3320_G_ResCount();
			res = LD3320_G_Res();
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
				default:;
			}
			LD3320_RunOnce();
		}
		Delay_ms(20);
	}
	
}
