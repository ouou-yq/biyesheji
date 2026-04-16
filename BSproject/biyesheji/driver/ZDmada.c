#include "stm32f10x.h"                  // Device header

#define Mada_port 		GPIOA
#define Mada_Pin 		GPIO_Pin_4

void ZDmada_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);		//按键初始化
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;
	GPIO_InitStructure.GPIO_Pin = Mada_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_Init(Mada_port,&GPIO_InitStructure);
	
	GPIO_ResetBits(Mada_port, Mada_Pin);
}

void mada_on(void)
{
	GPIO_SetBits(Mada_port, Mada_Pin);
}

void mada_off(void)
{
	GPIO_ResetBits(Mada_port, Mada_Pin);
}

