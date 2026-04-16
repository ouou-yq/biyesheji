#include "stm32f10x.h"                  // Device header
#include "delay.h"	      //包含需要的头文件

#define RainSen_AO_Port 	GPIOA
#define RainSen_AO_Pin 		GPIO_Pin_5 

#define RainSen_DO_Port 	GPIOA
#define RainSen_DO_Pin 		GPIO_Pin_6

 
/*函数名：初始化ADC */									   
void RainSen_AO_Adc_Init(void)
{    
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );	  //使能GPIOA通道时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE );	  //使能ADC1通道时钟
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);                      //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M
	
	GPIO_InitTypeDef GPIO_InitStructure;             //定义一个设置GPIO的变量
	GPIO_InitStructure.GPIO_Pin = RainSen_AO_Pin;        //准备设置PA5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	 //模拟输入引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(RainSen_AO_Port, &GPIO_InitStructure);           //设置PA5
    	
	ADC_DeInit(ADC1);                                  //复位ADC1,将外设 ADC1 的全部寄存器重设为缺省值
	ADC_InitTypeDef ADC_InitStructure;                              //定义一个设置ADC的变量
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	                   //ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	                       //模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	                   //模数转换工作在单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	   //转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	               //ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;	                               //顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);                                    //根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   
 
	ADC_Cmd(ADC1, ENABLE);	                                               //使能指定的ADC1	
	ADC_ResetCalibration(ADC1);	                                           //使能复位校准  	 
	while(ADC_GetResetCalibrationStatus(ADC1));                     	   //等待复位校准结束	
	ADC_StartCalibration(ADC1);	                                           //开启AD校准
	while(ADC_GetCalibrationStatus(ADC1));	                               //等待校准结束
}
 
/*-------------------------------------------------*/
/*函数名：获得ADC结果                              */
/*参  数：ch: 通道数                               */
/*-------------------------------------------------*/	
int Get_RainSen_Adc(int ch)   
{	
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_55Cycles5 );	//ADC1,ADC通道,采样时间为239.5周期	  			    
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		                        //使能指定的ADC1的软件转换启动功能	
	while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) == RESET);	//等待
	//RESET为未完成转换 SET为完成转换，转换完成后 ECO由硬件自动置1，那么while循环就自动跳出
	return ADC_GetConversionValue(ADC1);	                            //返回最近一次ADC1规则组的转换结果
}
/*-------------------------------------------------*/
/*函数名：平均多次ADC结果，提高精度                */
/*参  数：channel: 通道数                               */
/*参  数：count: 平均次数                          */
/*-------------------------------------------------*/	
int Get_RainSen_Adc_Average(int channel,int count)
{
	int sum_val=0;
	char t;
	
	for(t=0;t<count;t++)             //循环读取times次
	{
		sum_val+=Get_RainSen_Adc(channel);       //计算总值
		Delay_ms(5);                 //延时
	}
	return sum_val/count;           //返回平均值
} 


void RainSen_DO_Init(void)
{    	 
	GPIO_InitTypeDef GPIO_InitStructure;                     //定义一个设置GPIO的变量
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);   //使能GPIOA端口时钟
	GPIO_InitStructure.GPIO_Pin = RainSen_DO_Pin;       //设置PA0/1 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;    //浮空输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(RainSen_DO_Port, &GPIO_InitStructure);            		 //设置PA
	
}
 
int RainSen_DO_read(void)
{	
	//感应版没有水滴时，DO输出高电平；感应版有水滴时，DO输出低电平
	// 1为没下雨  0为下雨
	if(GPIO_ReadInputDataBit(RainSen_DO_Port, RainSen_DO_Pin) == 1)		
	{ 
		//Delay_ms(100);
		return 1;
	}
	return 0;
}
