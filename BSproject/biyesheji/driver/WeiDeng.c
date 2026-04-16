#include "stm32f10x.h"                  // Device header
#include "Delay.h"

//  配置SPI引脚：PB13 SCK, PB15 MOSI
//  配置CS引脚： PB12 CS


/************* CS控制 *************/
#define CS_LOW()  GPIO_ResetBits(GPIOB, GPIO_Pin_12)
#define CS_HIGH() GPIO_SetBits(GPIOB, GPIO_Pin_12)

/************* SPI发送16位数据 *************/
static void SPI2_Send16Bit(uint16_t data)
{
    // 等待发送缓冲区空
    while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
    
    // 发送16位数据
    SPI_I2S_SendData(SPI2, data);
    
    // 等待发送完成
    while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY) == SET);
    
    // 读取接收寄存器以清除RXNE标志
    SPI_I2S_ReceiveData(SPI2);
}

/************* 写寄存器 *************/
void MAX7219_WriteReg(uint8_t addr, uint8_t data)
{
    uint16_t command = ((uint16_t)addr << 8) | data;
    
    CS_LOW();
    SPI2_Send16Bit(command);
    CS_HIGH();
}

/************* 清屏 *************/
void MAX7219_Clear(void)
{
    for(int i = 1; i <= 8; i++)
    {
        MAX7219_WriteReg(i, 0x00);
    }
}

/************* 初始化 *************/
void MAX7219_Init(void)
{
    // 1. 使能时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 2. 配置SPI引脚：PB13 SCK, PB15 MOSI
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    // 3. 配置CS引脚：PB12
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    CS_HIGH();  // 初始状态CS无效
    
    // 4. SPI配置
    SPI_InitTypeDef SPI_InitStructure;
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    
    SPI_Init(SPI2, &SPI_InitStructure);
    SPI_Cmd(SPI2, ENABLE);
    
    // 5. 等待SPI稳定
    volatile uint32_t delay = 1000;
    while(delay--);
    
    /******** MAX7219配置 ********/
    MAX7219_WriteReg(0x0F, 0x00);   // 关闭测试模式
    MAX7219_WriteReg(0x0C, 0x01);   // 正常工作模式
    MAX7219_WriteReg(0x0B, 0x07);   // 扫描所有8位
    MAX7219_WriteReg(0x0A, 0x0F);   // 亮度最大
    MAX7219_WriteReg(0x09, 0x00);   // 不译码模式
    
    MAX7219_Clear();
}

/************* 函数1：全亮 *************/
void MAX7219_AllOn(void)
{
    for(int i = 1; i <= 8; i++)
    {
        MAX7219_WriteReg(i, 0xFF);  // 每一行所有8个LED都点亮
    }
}

/************* 函数2：只亮中间6×6点阵 *************/
void MAX7219_Center6x6(void)
{
    // 对于8×8点阵，中间6×6意味着：
    // - 第1行和第8行：全灭（不亮）
    // - 第2-7行：左右两边各1个LED灭，中间6个亮
    
    // 第1行：全灭
    MAX7219_WriteReg(1, 0x00);
    
    // 第2-7行：0x7E = 01111110，即第1和第8列灭，中间6列亮
    MAX7219_WriteReg(2, 0x7E);
    MAX7219_WriteReg(3, 0x7E);
    MAX7219_WriteReg(4, 0x7E);
    MAX7219_WriteReg(5, 0x7E);
    MAX7219_WriteReg(6, 0x7E);
    MAX7219_WriteReg(7, 0x7E);
    
    // 第8行：全灭
    MAX7219_WriteReg(8, 0x00);
}


/************* 设置亮度 *************/
// brightness: 0-15 (0最暗, 15最亮)
void MAX7219_SetBrightness(uint8_t brightness)
{
    // 限制亮度范围
    if(brightness > 15)
    {
        brightness = 15;
    }
    
    // 写入亮度寄存器 (0x0A)
    MAX7219_WriteReg(0x0A, brightness);
}




