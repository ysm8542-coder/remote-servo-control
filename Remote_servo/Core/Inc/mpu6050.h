/*
 * mpu6050.h
 *
 *  Created on: Aug 21, 2026
 *      Author: yeosa
 */

#ifndef SRC_MPU6050_H_
#define SRC_MPU6050_H_

#include "stm32f4xx_hal.h"
#define MPU6050_ADDR (0x68 << 1)

typedef struct {
	float ax, ay, ax;
	float gx, gy, gz;
}MPU6050_Data;

HAL_StatusTypeDef MPU6050_Init(I2C_HandleTypeDef* hi2c);

HAL_StatusTypeDef MPU6050_Read(I2C_HandleTypeDef* hi2c, MPU6050_Data* data);
#endif /* SRC_MPU6050_H_ */
