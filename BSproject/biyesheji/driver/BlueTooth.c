#include "stm32f10x.h"                  // Device header
#include "BlueTooth.h"
#include <stdio.h>
#include <string.h>

#define BlueTooth_TX		GPIO_Pin_9			//蓝牙串口重定义
#define BlueTooth_RX		GPIO_Pin_10
#define BlueTooth_Port 		GPIOA
#define BlueTooth_Usart 	USART1


void USART1_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = BlueTooth_TX;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(BlueTooth_Port,&GPIO_InitStructure);		//TX引脚配置
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = BlueTooth_RX;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(BlueTooth_Port,&GPIO_InitStructure);		//RX引脚配置
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;			//该成员配置USART通信波特率
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//是否开启硬件流控模式
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;					//指定是否启用或禁用接收或发送模式。
	USART_InitStructure.USART_Parity = USART_Parity_No;				//选择奇偶校验模式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;				//指定传输的停止位数
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;			//指定帧中发送或接收的数据位数
	USART_Init(BlueTooth_Usart,&USART_InitStructure);
	
//	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//	NVIC_InitTypeDef NVIC_InitStructure;
//	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
//	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(BlueTooth_Usart,ENABLE);
}

void USART1_SendByte(uint8_t Byte)		//发送一个字节数据或者发送一个字符
{
	USART_SendData(BlueTooth_Usart,Byte);
	while(USART_GetFlagStatus(BlueTooth_Usart,USART_FLAG_TXE) == RESET);		//不需要手动清零，因为下一次再SendData时，这个标志位会自动清零
}


//发送字符串
void USART1_SendString(char *String)
{
	uint8_t i;
	for(i = 0; String[i] != '\0'; i++)
	{
		USART1_SendByte(String[i]);
	}
}

void USART_SendAlert_Tick(void)
{
	 static uint16_t BT_Count;
	
	BT_Count ++;
	if(BT_Count >= 50)
	{
		USART1_SendString("ALERT!");
		BT_Count = 0;
	}	
}


