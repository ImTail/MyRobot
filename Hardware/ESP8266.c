#include "stm32f10x.h"                  // Device header
#include "MyUSART.h"
#include "ESP8266.h"
#include "OLED.h"
#include <string.h>

void ESP8266_Init()
{
	
}

void ESP8266_W_AT(char atCommand[])
{
	uint8_t commandLen = strlen(atCommand) ;
	for (uint8_t i = 0 ; i < commandLen ; i++)
	{
		MyUSART1_W_Byte(atCommand[i]);
	}	
}
