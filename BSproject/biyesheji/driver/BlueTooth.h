#ifndef __BlueTooth_H__
#define __BlueTooth_H__

#define ALERT_BYTE     0xA5     // 报警数据
#define ALERT_STRING   "ALERT!"  // 报警字符串

void USART1_Init(void);
void USART1_SendByte(uint8_t Byte);	
void USART1_SendString(char *String);
void USART_SendAlert_Tick(void);


#endif
