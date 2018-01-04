#include <stm32l4xx_hal.h>
#include "pah8001/pah8001.h"
#include "kxtj2/kxtj2.h"
#ifdef UART_DEBUG
#include "blocks-fw.h"
#endif

//#define VERIFY_LIB
#ifdef VERIFY_LIB
#include "pah8001testpatten.h"
#endif
static I2C_HandleTypeDef i2c2;


#ifdef VERIFY_LIB
float verify_library()
{
    int i = 0 ;
    int32_t version = 0;
    float myHR = 0 ;
    float grade = 0 ;
    int8_t ready_flag;
    int ret = 0;
    version = PxiAlg_Version();
    fw_info("Library version %d" CRLF,version);
    for(i=0; i<sizeof(PPG_Data)/sizeof(PPG_Data[0]); i++)
    {
        ret = PxiAlg_Process((unsigned char*)PPG_Data[i], (float *)fMEMS_Data[i]);
        if(ret != 0)
        {
            fw_info("Error" CRLF);
        }
        PxiAlg_HrGet(&myHR);
        PxiAlg_GetSigGrade(&grade);
        if(myHR != 0)
        {
            ready_flag = PxiAlg_GetReadyFlag();
        }
    }
    PxiAlg_HrGet(&myHR);
    return myHR ;
}
#endif
bool Kxtj2_WriteRegister(uint8_t reg, uint8_t data)
{
    uint8_t packet[] = { reg, data };
    return HAL_I2C_Master_Transmit(&i2c2, KXTJ2_I2C_ADDRESS, packet, 2, 1000) == HAL_OK;
}

bool Kxtj2_ReadRegister(uint8_t reg, uint8_t* buffer)
{
    if (HAL_I2C_Master_Transmit(&i2c2, KXTJ2_I2C_ADDRESS, &reg, 1, 1000) != HAL_OK) return false;
    return HAL_I2C_Master_Receive(&i2c2, KXTJ2_I2C_ADDRESS | 1, buffer, 1, 1000) == HAL_OK;
}


void Pah8001_Delay(uint8_t ms)
{
    HAL_Delay(ms);
}

bool PPG_Init(void)
{
    // Configure GPIO as I2C
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin       = GPIO_PIN_11|GPIO_PIN_10;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    __I2C2_CLK_ENABLE();

    // Configure I2C
    i2c2.Instance = I2C2;
    i2c2.Init.Timing = 0x00303D5B;
    i2c2.Init.OwnAddress1 = 0;
    i2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLED;
    i2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    i2c2.Init.OwnAddress2 = 0;
    i2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLED;
    i2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    i2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLED;
    HAL_I2C_Init(&i2c2);
    HAL_I2CEx_AnalogFilter_Config(&i2c2, I2C_ANALOGFILTER_ENABLED);

    return Pah8001_Reset();
}

bool PPG_Reset(void)
{
    return Pah8001_Reset();
}

bool Pah8001_ReadRegister(uint8_t reg, uint8_t* buffer, uint8_t length)
{
    uint8_t tries = 0;
    while (tries++ < 5) {
        if (HAL_I2C_Master_Transmit(&i2c2, 0x66, &reg, 1, 1000) == HAL_OK) break;
    }
    if (tries > 5) return false;

    tries = 0;
    while (tries++ < 5) {
        if (HAL_I2C_Master_Receive(&i2c2, 0x67, buffer, length, 1000) == HAL_OK) break;
    }
    if (tries > 5) return false;
    return true;
}

bool Pah8001_WriteRegister(uint8_t reg, uint8_t value)
{
    const uint8_t data[] = { reg, value };

    uint8_t tries = 0;
    while (tries++ < 5) {
        if (HAL_I2C_Master_Transmit(&i2c2, 0x66, data, 2, 1000) == HAL_OK) break;
    }
    return tries < 5;
}

bool PPG_Enable(void)
{
    return Pah8001_PowerOn();
}

bool PPG_Disable(void)
{
    return Pah8001_PowerOff();
}


uint8_t PPG_GetHR(float* value_out)
{
    uint8_t buffer[13];

    uint8_t res = Pah8001_ReadRawData(buffer);
#ifdef VERIFY_LIB
    float HR;
    HR = verify_library();
    fw_info("HR %d" CRLF,(uint16_t)HR);
#endif

    if (res != 0)
    {
#ifdef UART_DEBUG
        fw_info("read fail" CRLF);
#endif
        return res;
    }
    return Pah8001_HRFromRawData(buffer, value_out) ? 0 : 0x30;
}

size_t PPG_GetRawData(uint8_t* buffer, size_t length)
{
    uint8_t data[13];
    if (!Pah8001_GetRawData(data)) return 0;

    size_t i = 0;
    for (; i < sizeof(data) && i < length; i++) {
        buffer[i] = data[i];
    }
    return i;
}

uint8_t PPG_GetRate(void)
{
    return 20;
}
