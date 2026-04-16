#include "stm32f10x.h"                  // Device header

#define JQ8900_IO1		GPIO_Pin_11
#define JQ8900_IO2		GPIO_Pin_12
#define JQ8900_Port		GPIOA

void JQ8900_IO1_Control_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin = JQ8900_IO1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // 推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(JQ8900_Port, &GPIO_InitStructure);

    // 默认拉高（不播放）
    GPIO_SetBits(JQ8900_Port, JQ8900_IO1);
}

void JQ8900_Play_IO1(void)
{
    // 拉低 → 相当于接地
    GPIO_ResetBits(JQ8900_Port, JQ8900_IO1);
}

void JQ8900_No_Play_IO1(void)
{
     // 释放（拉高）
    GPIO_SetBits(JQ8900_Port, JQ8900_IO1);
}
