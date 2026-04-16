#include "stm32f10x.h"                  // Device header

#define Buzzer_IO_Port  GPIOA
#define Buzzer_IO_Pin   GPIO_Pin_12

void Buzzer_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = Buzzer_IO_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(Buzzer_IO_Port, &GPIO_InitStructure);
	
	//因为用了有源蜂鸣器 低电平触发 初始化时先高电平
    GPIO_SetBits(Buzzer_IO_Port, Buzzer_IO_Pin);			
	
}

void Buzzer_on(void)
{
	GPIO_ResetBits(Buzzer_IO_Port,Buzzer_IO_Pin);
}

void Buzzer_off(void)
{
	GPIO_SetBits(Buzzer_IO_Port,Buzzer_IO_Pin);
}
