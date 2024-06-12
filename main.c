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
	
	
	OLED_ClearAll();						//清屏
	//Cartoon_Raw();							//展示初始图像
	
	LD3320_RunOnce();						//第一次运行程序，启动一次识别
	
	while (1)
	{
		
		
		if (LD3320_G_HaveResFlag()==1)		
		{
			//resCount = LD3320_G_ResCount();
			res = LD3320_G_Res();
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
				default:;
			}
			LD3320_RunOnce();
		}
		Delay_ms(20);
	}
	
}
