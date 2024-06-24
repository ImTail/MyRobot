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
	uint8_t ledFlag = 1;					//记录灯开关,默认开
	uint8_t speechFlag = 0;					//记录是否唤醒了语音助手
	
	LED_Init();								//LDE初始化
	/*IIC驱动*/
	OLED_Init();							//OLED初始化
	/*SPI驱动*/
	LD3320_Init();							//初始化LD3320引脚
	/*以下模块用串口驱动*/
	MyUSART_Init();	
	SYN6288_Init();							//SYN6288初始化
	ESP8266_Init();
	
	OLED_ClearAll();						//清屏
	//Cartoon_Raw();							//展示初始图像
	
	LD3320_RunOnce();						//第一次运行程序，启动一次识别
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
				case 1:						//唤醒语句，可激活一次回答
					SYN6288_Speech("在");
					speechFlag = 1;
				break;
				case 2:
					SYN6288_Speech("好的");			
					speechFlag = 0;
				break;
				case 3:
					if (ledFlag == 0 && speechFlag == 1) 
					{
						speechFlag = 0;
						ledFlag =1;
						LED_ON();
						SYN6288_Speech("灯[3]已打开"); 
					}						
				break;
				case 4:
					if (ledFlag == 1&& speechFlag == 1) 
					{
						speechFlag = 0;
						ledFlag =0;
						LED_OFF();
						SYN6288_Speech("灯[3]已关闭"); 
					}
				break;
				case 5:
					if (speechFlag==1)
					{
						speechFlag = 0;
						SYN6288_Speech("[2]我是[3]stm[n1]32[2]主控[2]语音[2]助手[n2]"); 
					}
					
				break;
					case 6:
					if (speechFlag==1)
					{
						speechFlag = 0;
						uint8_t espRes = 0x01;
						espRes &= ESP8266_S_Mode(1,1);								//设置模式为station,并存储
						espRes &= ESP8266_Station_Connect("NPC","123456789",0);		//连接指定AP,不存储
						espRes &= ESP8266_TCP_Start("192.168.1.3","8080","0");
						OLED_ShowString(0,2,RX_Data_1,12,1);
						if (espRes==1)
						{
							SYN6288_Speech("成功"); 
						} else if (espRes==0)
						{
							SYN6288_Speech("失败"); 
						} else 
						{
							SYN6288_Speech("错误"); 
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
