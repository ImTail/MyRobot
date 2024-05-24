#include "stm32f10x.h"                  // Device header

void LED_Init() 
{
	/*使能时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	/*初始化*/
	GPIO_InitTypeDef IO_Init_Structure;
	IO_Init_Structure.GPIO_Mode=GPIO_Mode_Out_PP;
	IO_Init_Structure.GPIO_Pin=GPIO_Pin_0;
	IO_Init_Structure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&IO_Init_Structure);
	
}
