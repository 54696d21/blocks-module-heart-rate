#include <stm32l4xx_hal.h>
#include "pah8001/pah8001.h"
#include "kxtj2/kxtj2.h"

static I2C_HandleTypeDef ppg_i2c_2;

bool Kxtj2_WriteRegister(uint8_t reg, uint8_t data) {
	uint8_t packet[] = { reg, data };
	return HAL_I2C_Master_Transmit(&ppg_i2c_2, KXTJ2_I2C_ADDRESS, packet, 2, 1000) == HAL_OK;
}

bool Kxtj2_ReadRegister(uint8_t reg, uint8_t* buffer) {
	if (HAL_I2C_Master_Transmit(&ppg_i2c_2, KXTJ2_I2C_ADDRESS, &reg, 1, 1000) != HAL_OK) return false;
	return HAL_I2C_Master_Receive(&ppg_i2c_2, KXTJ2_I2C_ADDRESS | 1, buffer, 1, 1000) == HAL_OK;
}


void Pah8001_Delay(uint8_t ms) {
	HAL_Delay(ms);
}

bool PPG_Init(void) {
	// Configure GPIO as I2C
	GPIO_InitTypeDef GPIO_InitStruct = {
		.Pin = GPIO_PIN_11 | GPIO_PIN_10,
		.Mode = GPIO_MODE_AF_OD,
		.Pull = GPIO_PULLUP,
		.Speed = GPIO_SPEED_HIGH,
		.Alternate = GPIO_AF4_I2C2
	};
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	__I2C2_CLK_ENABLE();

	// Configure I2C
	I2C_HandleTypeDef i2c = {
		.Instance = I2C2,
		.Init.Timing = 0x00303D5B,
		.Init.OwnAddress1 = 0,
		.Init.DualAddressMode = I2C_DUALADDRESS_DISABLED,
		.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT,
		.Init.OwnAddress2 = 0,
		.Init.NoStretchMode = I2C_NOSTRETCH_DISABLED,
		.Init.OwnAddress2Masks = I2C_OA2_NOMASK,
		.Init.GeneralCallMode = I2C_GENERALCALL_DISABLED,
	};
	HAL_I2C_Init(&i2c);
	HAL_I2CEx_AnalogFilter_Config(&i2c, I2C_ANALOGFILTER_ENABLED);
	ppg_i2c_2 = i2c;

	return Pah8001_Reset();
}

bool PPG_Reset(void) {
	return Pah8001_Reset();
}

bool Pah8001_ReadRegister(uint8_t reg, uint8_t* buffer, uint8_t length) {
	uint8_t tries = 0;
	while (tries++ < 5) {
		if (HAL_I2C_Master_Transmit(&ppg_i2c_2, 0x66, &reg, 1, 1000) == HAL_OK) break;
	}
	if (tries > 5) return false;

	tries = 0;
	while (tries++ < 5) {
		if (HAL_I2C_Master_Receive(&ppg_i2c_2, 0x67, buffer, length, 1000) == HAL_OK) break;
	}
	if (tries > 5) return false;
	return true;
}

bool Pah8001_WriteRegister(uint8_t reg, uint8_t value) {
	const uint8_t data[] = { reg, value };

	uint8_t tries = 0;
	while (tries++ < 5) {
		if (HAL_I2C_Master_Transmit(&ppg_i2c_2, 0x66, data, 2, 1000) == HAL_OK) break;
	}
	return tries < 5;
}

bool PPG_Enable(void) {
	return Pah8001_PowerOn();
}

bool PPG_Disable(void) {
	return Pah8001_PowerOff();
}


uint8_t PPG_GetHR(float* value_out) {
	uint8_t buffer[13];

	uint8_t res = Pah8001_ReadRawData(buffer);

	if (res != 0)
	{
		return res;
	}
	return Pah8001_HRFromRawData(buffer, value_out) ? 0 : 0x30;
}

size_t PPG_GetRawData(uint8_t* buffer, size_t length) {
	uint8_t data[13];
	if (!Pah8001_GetRawData(data)) return 0;

	size_t i = 0;
	for (; i < sizeof(data) && i < length; i++) {
		buffer[i] = data[i];
	}
	return i;
}

uint8_t PPG_GetRate(void) {
	return 20;
}

bool PPG_Run(float* heartRate) {
	bool dataReady = false;

	HAL_Delay(47);

	float value = 0.f;
	uint8_t res = PPG_GetHR(&value);

	dataReady = (res == 0);
	if (dataReady && *heartRate != value) {
		*heartRate = value;
	}

	return dataReady;
}
