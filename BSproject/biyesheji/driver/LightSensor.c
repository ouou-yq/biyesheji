#include "stm32f10x.h"                  // Device header

#define LightSensor_IO_Port	GPIOA
#define LightSensor_IO_Pin	GPIO_Pin_8

uint8_t LightSensor_Flag = 0;

void LightSensor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = LightSensor_IO_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 上拉输入
    GPIO_Init(LightSensor_IO_Port, &GPIO_InitStructure);
}

void LightSensor_GetFlag(void)
{
	if(GPIO_ReadInputDataBit(LightSensor_IO_Port,LightSensor_IO_Pin) == 1)
	{
		LightSensor_Flag = 1;
	}
	else
	{
		LightSensor_Flag = 0;
	}	
}
