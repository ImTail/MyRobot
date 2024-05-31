#include "stm32f10x.h"                  // Device header

#define LED1 GPIO_Pin_1

void LED_Init(void) 
{
	/*使能时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	/*初始化*/
	GPIO_InitTypeDef IO_Init_Structure;
	IO_Init_Structure.GPIO_Mode=GPIO_Mode_Out_PP;
	IO_Init_Structure.GPIO_Pin=LED1;
	IO_Init_Structure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&IO_Init_Structure);
	
}

void LED_ON(void)
{
	GPIO_ResetBits(GPIOA,LED1);
}

void LED_OFF(void)
{
	GPIO_SetBits(GPIOA,LED1);
}
