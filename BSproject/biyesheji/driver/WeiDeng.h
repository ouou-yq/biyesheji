#ifndef __WeiDeng_H__
#define __WeiDeng_H__

/************* SPI发送16位数据 *************/
static void SPI2_Send16Bit(uint16_t data);
void MAX7219_WriteReg(uint8_t addr, uint8_t data);
void MAX7219_Clear(void);
void MAX7219_Init(void);
void MAX7219_AllOn(void);
void MAX7219_Center6x6(void);

/************* 设置亮度 *************/
// brightness: 0-15 (0最暗, 15最亮)
void MAX7219_SetBrightness(uint8_t brightness);






//void MAX7219_GPIO_Init(void);
//static void MAX7219_Write(uint8_t addr, uint8_t data);
//void MAX7219_Init(void);
//void MAX7219_SetAll(u8 state);
//void MAX7219_SetBrightness(u8 level);



#endif
