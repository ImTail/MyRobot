#include "stm32f10x.h"
#include "LED.h"
#include "Delay.h"
#include "OLED.h"
#include "Cartoon.h"



int main() 
{
	LED_Init();
	OLED_Init();
	OLED_ClearAll();
	Cartoon_Raw();
	while (1)
	{
		
		Cartoon_Say("你");
	}
	
}
