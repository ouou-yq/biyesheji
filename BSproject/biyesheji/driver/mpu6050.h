#ifndef __mpu6050_H__
#define __mpu6050_H__


// 初始化
void MPU6050_Init(void);

// 读取加速度
void MPU6050_ReadAccel(int16_t *ax, int16_t *ay, int16_t *az);

// 获取Z轴（或自行改X轴）
int16_t MPU6050_GetAccel_Z(void);

// 刹车检测
int16_t MPU6050_BrakeDetect(void);


#endif
