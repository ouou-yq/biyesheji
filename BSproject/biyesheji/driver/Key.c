#include "stm32f10x.h"                  // Device header
#include "Delay.h"

#define L_key_port 		GPIOB
#define L_key_Pin 		GPIO_Pin_0
#define L_key_number	1			//左1右2

#define R_key_port 		GPIOB
#define R_key_Pin 		GPIO_Pin_1
#define R_key_number	2

uint8_t Key_Num;

void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);		//按键初始化
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

uint8_t Key_GetNum(void)
{
	uint8_t Temp;
	if (Key_Num)
	{
		Temp = Key_Num;
		Key_Num = 0;
		return Temp;
	}
	return 0;
}

uint8_t Key_GetState(void)
{
	
	if(GPIO_ReadInputDataBit(L_key_port,L_key_Pin) == 0) 
	{
		return L_key_number;
	}
	if(GPIO_ReadInputDataBit(R_key_port,R_key_Pin) == 0) 
	{
		return R_key_number;
	}
	return 0;
}

void Key_Tick(void)
{
	static uint8_t Count;
	static uint8_t CurrState, PrevState;
	
	Count ++;
	if (Count >= 2)
	{
		Count = 0;
		
		PrevState = CurrState;
		CurrState = Key_GetState();
		
		if (CurrState == 0 && PrevState != 0)
		{
			Key_Num = PrevState;
		}
	}
}


