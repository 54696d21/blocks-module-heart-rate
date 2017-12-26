#pragma once
#include <stdint.h>
#include <stddef.h>

bool Pah8001_ReadRawData(uint8_t buffer[13]);

bool Pah8001_HRFromRawData(const uint8_t rawdata[13], float* hr_out);

bool Pah8001_HRValid(void);

bool Pah8001_Reset(void);

bool Pah8001_PowerOff(void);

bool Pah8001_PowerOn(void);
