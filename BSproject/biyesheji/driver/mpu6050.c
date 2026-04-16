#include "stm32f10x.h"                  // Device header

// MPU6050地址
#define MPU6050_ADDR 0xD0
// MPU6050寄存器
#define SMPLRT_DIV   0x19
#define CONFIG       0x1A
#define GYRO_CONFIG  0x1B
#define ACCEL_CONFIG 0x1C
#define ACCEL_XOUT_H 0x3B
#define PWR_MGMT_1   0x6B
#define MPU6050_SCL_Pin GPIO_Pin_10
#define MPU6050_SDA_Pin GPIO_Pin_11

/**************** I2C2初始化 ****************/
void I2C2_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    I2C_InitTypeDef I2C_InitStructure;
    I2C_InitStructure.I2C_ClockSpeed = 400000;
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0x00;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;

    I2C_Init(I2C2, &I2C_InitStructure);
    I2C_Cmd(I2C2, ENABLE);
}

/**************** 写寄存器 ****************/
void MPU6050_WriteReg(uint8_t reg, uint8_t data)
{
    I2C_GenerateSTART(I2C2, ENABLE);
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));

    I2C_Send7bitAddress(I2C2, MPU6050_ADDR, I2C_Direction_Transmitter);
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    I2C_SendData(I2C2, reg);
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    I2C_SendData(I2C2, data);
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    I2C_GenerateSTOP(I2C2, ENABLE);
}

/**************** 读取多字节 ****************/
void MPU6050_ReadBuf(uint8_t reg, uint8_t *buf, uint8_t len)
{
    I2C_GenerateSTART(I2C2, ENABLE);
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));

    I2C_Send7bitAddress(I2C2, MPU6050_ADDR, I2C_Direction_Transmitter);
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    I2C_SendData(I2C2, reg);
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    I2C_GenerateSTART(I2C2, ENABLE);
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));

    I2C_Send7bitAddress(I2C2, MPU6050_ADDR, I2C_Direction_Receiver);
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

    while(len)
    {
        if(len == 1)
        {
            I2C_AcknowledgeConfig(I2C2, DISABLE);
            I2C_GenerateSTOP(I2C2, ENABLE);
        }

        if(I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED))
        {
            *buf++ = I2C_ReceiveData(I2C2);
            len--;
        }
    }

    I2C_AcknowledgeConfig(I2C2, ENABLE);
}

/**************** 初始化MPU6050 ****************/
void MPU6050_Init(void)
{
    I2C2_Init();

    MPU6050_WriteReg(PWR_MGMT_1, 0x00);
    MPU6050_WriteReg(SMPLRT_DIV, 0x07);
    MPU6050_WriteReg(CONFIG, 0x06);
    MPU6050_WriteReg(GYRO_CONFIG, 0x18);
    MPU6050_WriteReg(ACCEL_CONFIG, 0x01);
}

/**************** 读取加速度 ****************/
void MPU6050_ReadAccel(int16_t *ax, int16_t *ay, int16_t *az)
{
    uint8_t buf[6];

    MPU6050_ReadBuf(ACCEL_XOUT_H, buf, 6);

    *ax = (buf[0]<<8) | buf[1];
    *ay = (buf[2]<<8) | buf[3];
    *az = (buf[4]<<8) | buf[5];
}

int16_t MPU6050_GetAccel_Z(void)
{
    int16_t ax, ay, az;
    MPU6050_ReadAccel(&ax, &ay, &az);
    return az;
}

/**************** 滤波 + 刹车检测 ****************/
#define FILTER_SIZE 10

static int16_t accel_buf[FILTER_SIZE] = {0};
static uint8_t index_buf = 0;

static int16_t Filter(int16_t new_data)
{
    accel_buf[index_buf++] = new_data;
    if(index_buf >= FILTER_SIZE) index_buf = 0;

    int32_t sum = 0;
    for(int i=0;i<FILTER_SIZE;i++)
        sum += accel_buf[i];

    return sum / FILTER_SIZE;
}

/**************** 刹车判断 ****************/
int16_t MPU6050_BrakeDetect(void)
{
    static int16_t last = 0;

    int16_t current = MPU6050_GetAccel_Z();
    int16_t filtered = Filter(current);

    int16_t diff = filtered - last;
    last = filtered;

	 return diff;
	
//    // 阈值（需要你实测微调）
//    if(diff < -3000)
//        return 1;

//    return 0;
}
