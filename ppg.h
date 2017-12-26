#include <stdint.h>

bool PPG_Init(void);

bool PPG_Enable(void);

bool PPG_Disable(void);

bool PPG_Reset(void);

bool PPG_GetHR(float* value_out);

size_t PPG_GetRawData(uint8_t* buffer, size_t length);

uint8_t PPG_GetRate(void);
