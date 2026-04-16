#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Delay.h"
#include "LRlight.h"
#include "Key.h"
#include "ZDmada.h"
#include "Buzzer.h"
#include "Timer2.h"
#include "BlueTooth.h"
#include "LightSensor.h"
#include "RainSensor.h"
#include "WeiDeng.h"
#include "MPU6050.h"
#include "JQ8900.h"


#define RAIN_THRESHOLD    3000

uint8_t KeyNum;
uint8_t LED1Mode;
uint8_t LED2Mode;

float raindata; 
int16_t MPU_diff;

int main(void)
{
	OLED_Init();
	LRlight_Init();
	Key_Init();
	ZDmada_Init();
	Buzzer_Init();
	Timer2_Init();
//	USART1_Init();
	LightSensor_Init();
	RainSen_AO_Adc_Init();
	RainSen_DO_Init();
	MAX7219_Init();
	MPU6050_Init();
//	JQ8900_IO1_Control_Init();
	


	MAX7219_Center6x6();
	MAX7219_SetBrightness(6);

	OLED_ShowString(1, 1, "LED1Mode:");
	OLED_ShowString(2, 1, "LED2Mode:");
	OLED_ShowString(3,1,"rain%:");
	OLED_ShowString(3,12,"DO:");
	while(1)
	{		
					
		/*按键扫描控制左右转向灯*/
		KeyNum = Key_GetNum();	
		if (KeyNum == 1)
		{
			mada_on();
			LED1Mode ++;
			LED1Mode %= 2;
			LED1_SetMode(LED1Mode);
			
			if(LED1Mode == 1)
			{
				LED2Mode = 0;
				LED2_SetMode(LED2Mode);
			}
		}
		if (KeyNum == 2)
		{
			mada_off();
			LED2Mode ++;
			LED2Mode %= 2;
			LED2_SetMode(LED2Mode);
			
			if(LED2Mode == 1)
			{
				LED1Mode = 0;
				LED1_SetMode(LED1Mode);
			}
		}
		OLED_ShowNum(1, 10, LED1Mode, 1);
		OLED_ShowNum(2, 10, LED2Mode, 1);
		
		/*刹车灯光警告*/
		MPU_diff = MPU6050_BrakeDetect();
		if(MPU_diff < -1250)
		{			
			MAX7219_AllOn();
		}
		else
		{
			MAX7219_Center6x6();
		}
		
		
		/*雨滴传感器和光敏传感器控制尾灯亮度*/
		
		raindata = (float)(Get_RainSen_Adc_Average(ADC_Channel_5,5));
		LightSensor_GetFlag();
		if(RainSen_DO_read() == 0 && raindata < RAIN_THRESHOLD )
		{
			
			OLED_ShowString(4,1,"rain now");
			Buzzer_on();
			MAX7219_SetBrightness(15);
			//JQ8900_Play_IO1();
		}
		else
		{
			OLED_ShowString(4,1,"NO  RAIN");
			Buzzer_off();
			MAX7219_SetBrightness(6);
			//JQ8900_No_Play_IO1();
		}
		
		OLED_ShowNum(3,15,RainSen_DO_read(),1);		
		OLED_ShowNum(3,7,raindata,4);		
		}		
	
}


/*		定时器中断执行轮询任务		*/
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		Key_Tick();
		LED_Tick();
		//USART_SendAlert_Tick();
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}


//LightSensor_Flag == 1
//MAX7219_SetBrightness(6);
//		Delay_ms(2000);
//		MAX7219_SetBrightness(15);
//		Delay_ms(2000);	


	
//	while(1)
//	{
//		OLED_ShowString(1, 1, "LED1Mode:");
//		OLED_ShowString(2, 1, "LED2Mode:");
//		OLED_ShowString(3,1,"DO:");
//		OLED_ShowNum(3,4,RainSen_DO_read(),1);

//		MAX7219_Center6x6();
//		KeyNum = Key_GetNum();
//		raindata = (float)(Get_RainSen_Adc_Average(ADC_Channel_5,5));
//		LightSensor_GetFlag();		
//		
//		if (KeyNum == 1)
//		{
//			mada_on();
//			LED1Mode ++;
//			LED1Mode %= 2;
//			LED1_SetMode(LED1Mode);
//			
//			if(LED1Mode == 1)
//			{
//				LED2Mode = 0;
//				LED2_SetMode(LED2Mode);
//			}
//		}
//		if (KeyNum == 2)
//		{
//			mada_off();
//			LED2Mode ++;
//			LED2Mode %= 2;
//			LED2_SetMode(LED2Mode);
//			
//			if(LED2Mode == 1)
//			{
//				LED1Mode = 0;
//				LED1_SetMode(LED1Mode);
//			}
//		}
//		
//		if(RainSen_DO_read() == 0 && raindata < RAIN_THRESHOLD && LightSensor_Flag == 1)
//		{
//			
//			OLED_ShowString(4,1,"rain now");
//			Buzzer_on();
//		}
//		else
//		{
//			OLED_ShowString(4,1,"NO  RAIN");
//			Buzzer_off();
//		}
//		
//		OLED_ShowNum(1, 10, LED1Mode, 1);
//		OLED_ShowNum(2, 10, LED2Mode, 1);		
//		
//		OLED_ShowString(5,1,"rain%:");
//		OLED_ShowNum(5,7,raindata,4);
//	}
		
		
		
//		OLED_ShowString(1,1,"ON ");
//		JQ8900_Play_IO1();
//		Delay_ms(5000);
//		
//		OLED_ShowString(1,1,"OFF");
//		JQ8900_No_Play_IO1();
//		Delay_ms(3000);
	




/*			刹车提示功能			*/
//		MPU_diff = MPU6050_BrakeDetect();
//		OLED_ShowSignedNum(1,1,MPU_diff,5);
//		if(MPU_diff < -1250)
//		{
//			LED1_ON();
//			
//			MAX7219_AllOn();
//		}
//		else
//		{
//			LED1_OFF();
//			MAX7219_AllOn();
//		}



/* 				尾灯测试			*/
//	MAX7219_AllOn();          // 全亮测试
//	OLED_ShowString(1,1,"all");
//    Delay_ms(2000);           // 亮2秒
//    
//    MAX7219_Clear();          // 清屏
//	OLED_ShowString(1,1,"not");
//    Delay_ms(500);
//    
//    MAX7219_Center6x6();      // 显示中间6×6
//	OLED_ShowString(1,1,"666");
//    Delay_ms(2000);           // 亮2秒




//							雨滴传感器
//		float raindata; 
//		OLED_ShowString(1,1,"DO:");
//		OLED_ShowNum(1,4,RainSen_DO_read(),1);
//		raindata = (float)(Get_RainSen_Adc_Average(ADC_Channel_5,5));
//		if(RainSen_DO_read() == 0 && raindata < RAIN_THRESHOLD)
//		{
//			OLED_ShowString(2,1,"rain now");
//			LED1_ON();
//			Buzzer_on();
//		}
//		else
//		{
//			OLED_ShowString(2,1,"NO  RAIN");
//			LED1_OFF();
//			Buzzer_off();
//		}
//		OLED_ShowString(3,1,"rain%:");
//		OLED_ShowNum(3,7,raindata,4);
		
		
		
//					光敏传感器		
//		LightSensor_GetFlag();
//		if(LightSensor_Flag == 1)
//		{
//			LED1_ON();
//		}
//		else
//		{
//			LED1_OFF();
//		}




/* 左右提示灯功能		//可以写进应用层
		
	OLED_ShowString(1, 1, "i:");
	OLED_ShowString(2, 1, "LED1Mode:");
	OLED_ShowString(3, 1, "LED2Mode:");
	
	while(1)
	{
		KeyNum = Key_GetNum();
		
		if (KeyNum == 1)
		{
			LED1Mode ++;
			LED1Mode %= 2;
			LED1_SetMode(LED1Mode);
			
			if(LED1Mode == 1)
			{
				LED2Mode = 0;
				LED2_SetMode(LED2Mode);
			}
		}
		if (KeyNum == 2)
		{
			LED2Mode ++;
			LED2Mode %= 2;
			LED2_SetMode(LED2Mode);
			
			if(LED2Mode == 1)
			{
				LED1Mode = 0;
				LED1_SetMode(LED1Mode);
			}
		}
		
		OLED_ShowNum(1, 3, i ++, 5);
		OLED_ShowNum(2, 10, LED1Mode, 1);
		OLED_ShowNum(3, 10, LED2Mode, 1);		
	}

*/
