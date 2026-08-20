/*
 * mpu6050.c
 *
 *  Created on: Aug 21, 2026
 *      Author: yeosa
 */

#include "mpu6050.h"

#define REG_PWR_MGMT_1 0x6B
#define REG_ACCEL_XOUT_H 0x3B

//기본 설정 기준 감가
#define ACCEL_SENS 16384.0f
#define GYRO_SENSE 131.0f

HAL_StatusTypeDef MPU6050_Init(I2C_HandleTypeDef* hi2c)
{
	uint8_t data = 0x00;
	return HAL_I2C_Mem_Write(hi2c, MPU6050_ADDR, REG_PWR_MGMT_1, 1, &data, 1, HAL_MAX_DELAY);
}

HAL_StatusTypeDef MPU6050_Read(I2C_HandleTypeDef* hi2c, MPU6050_Data* data)
{
	uint8_t raw[14];
	HAL_StatusTypeDef status = HAL_I2C_Mem_Read(hi2c, MPU6050_ADDR, REG_ACCEL_XOUT_H, 1, raw, 14, HAL_MAX_DELAY);
	if(status != HAL_OK) return status;

	uint16_t ax_raw = (int16_t)(raw[0] << 8 | raw[1]);
	uint16_t ay_raw = (int16_t)(raw[2] << 8 | raw[3]);
	uint16_t az_raw = (int16_t)(raw[4] << 8 | raw[5]);
	uint16_t gx_raw = (int16_t)(raw[8] << 8 | raw[9]);
	uint16_t gx_raw = (int16_t)(raw[10] << 8 | raw[11]);
	uint16_t gx_raw = (int16_t)(raw[12] << 8 | raw[13]);

	data->ax = ax_raw / ACCEL_SENS;
	data->ay = ay_raw / ACCEL_SENS;
	data->az = az_raw / ACCEL_SENS;
	data->gx = gx_raw / GYRO_SENS;
	data->gy = gy_raw / GYRO_SENS;
	data->gz = gz_raw / GYRO_SENS;

	return HAL_OK;
}
