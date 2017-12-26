#include "kxtj2.h"

#define KXTJ2_REG_XOUT_L        0x06
#define KXTJ2_REG_XOUT_H        0x07
#define KXTJ2_REG_YOUT_L        0x08
#define KXTJ2_REG_YOUT_H        0x09
#define KXTJ2_REG_ZOUT_L        0x0A
#define KXTJ2_REG_ZOUT_H        0x0B
#define KXTJ2_REG_DCST_RESP     0x0C
#define KXTJ2_REG_WHO_AM_I      0x0F
#define KXTJ2_REG_INT_SRC1      0x16
#define KXTJ2_REG_INT_SRC2      0x17
#define KXTJ2_REG_INT_STATUS    0x18
#define KXTJ2_REG_INT_REL       0x1A
#define KXTJ2_REG_CTRL_REGA     0x1B
#define KXTJ2_REG_CTRL_REGC     0x1D
#define KXTJ2_REG_INT_CTRL_REG1 0x1E
#define KXTJ2_REG_INT_CTRL_REG2 0x1F
#define KXTJ2_REG_DATA_CTRL_REG 0x21
#define KXTJ2_REG_WUF_TIMER     0x29
#define KXTJ2_REG_SELF_TEST     0x3A
#define KXTJ2_REG_WUF_THRESH    0x6A


static int signed_cast(uint8_t value)
{
    return value & 0x80 == 0x80 ? (~value) - 1 : value;
}

bool Kxtj2_Init(void)
{
    if (!Kxtj2_WriteRegister(KXTJ2_REG_CTRL_REGC, 0x80)) return false;
    HAL_Delay(2);
    if (!Kxtj2_WriteRegister(KXTJ2_REG_CTRL_REGA, 0x00)) return false;
    HAL_Delay(1);
    if (!Kxtj2_WriteRegister(KXTJ2_REG_DATA_CTRL_REG, 0x02)) return false;
    HAL_Delay(1);
    if (!Kxtj2_WriteRegister(KXTJ2_REG_INT_CTRL_REG1, 0x10)) return false;
    HAL_Delay(1);
    return true;
}

void Kxtj2_GetXYZ(float buffer[3])
{
    uint8_t value;
    if (!Kxtj2_ReadRegister(KXTJ2_REG_XOUT_H, &value)) return false;
    buffer[0] = signed_cast(value);
    if (!Kxtj2_ReadRegister(KXTJ2_REG_YOUT_H, &value)) return false;
    buffer[1] = signed_cast(value);
    if (!Kxtj2_ReadRegister(KXTJ2_REG_ZOUT_H, &value)) return false;
    buffer[2] = signed_cast(value);
}

bool Kxtj2_PowerOn(void)
{
    return Kxtj2_WriteRegister(KXTJ2_REG_CTRL_REGA, 0x00u);
}

bool Kxtj2_PowerOff(void)
{
    return Kxtj2_WriteRegister(KXTJ2_REG_CTRL_REGA, 0x80u);
}
