#ifndef __RainSensor_H__
#define __RainSensor_H__

								   
void RainSen_AO_Adc_Init(void);
int Get_RainSen_Adc(int ch);   
int Get_RainSen_Adc_Average(int channel,int count);
void RainSen_DO_Init(void);
int RainSen_DO_read(void);



#endif
