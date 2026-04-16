#include "stm32f10x.h"                  // Device header

#define L_led_Port     GPIOA
#define L_led_Pin      GPIO_Pin_0

#define R_led_Port     GPIOA
#define R_led_Pin      GPIO_Pin_1

uint8_t LED1_Mode;
uint8_t LED2_Mode;

uint16_t LED1_Count;
uint16_t LED2_Count;

/* 初始化 */
void LRlight_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = L_led_Pin | R_led_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_ResetBits(GPIOA, L_led_Pin | R_led_Pin);		//上电初始化时先熄灭灯
}

void LED1_SetMode(uint8_t Mode)
{
	if (Mode != LED1_Mode)
	{
		LED1_Mode = Mode;
		LED1_Count = 0;
	}
}

void LED2_SetMode(uint8_t Mode)
{
	if (Mode != LED2_Mode)
	{
		LED2_Mode = Mode;
		LED2_Count = 0;
	}
}

void LED1_ON(void)
{
	GPIO_SetBits(L_led_Port, L_led_Pin);
}

void LED1_OFF(void)
{
	GPIO_ResetBits(L_led_Port, L_led_Pin);
}

void LED2_ON(void)
{
	GPIO_SetBits(R_led_Port, R_led_Pin);
}

void LED2_OFF(void)
{
	GPIO_ResetBits(R_led_Port, R_led_Pin);
}

void LED_Tick(void)
{
	//左灯
	if(LED1_Mode == 0)
	{
		LED1_OFF();
	}
	else if (LED1_Mode == 1)
	{
		LED1_Count ++;
		LED1_Count %= 100;
		
		if (LED1_Count < 50)
		{
			LED1_ON();
		}
		else
		{
			LED1_OFF();
		}
	}
	
	//右灯
	if(LED2_Mode == 0)
	{
		LED2_OFF();
	}
	else if (LED2_Mode == 1)
	{
		LED2_Count ++;
		LED2_Count %= 100;
		
		if (LED2_Count < 50)
		{
			LED2_ON();
		}
		else
		{
			LED2_OFF();
		}
	}
}
