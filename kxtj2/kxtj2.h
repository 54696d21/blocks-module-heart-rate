#pragma once
#include <stdint.h>

#define KXTJ2_I2C_ADDRESS 0x1C

extern bool Kxtj2_WriteRegister(uint8_t reg, uint8_t data);

extern bool Kxtj2_ReadRegister(uint8_t reg, uint8_t* buffer);

void Kxtj2_GetXYZ(float buffer[3]);

bool Kxtj2_PowerOff(void);

bool Kxtj2_PowerOn(void);
