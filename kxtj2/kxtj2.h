/* kxtj2.h - (c) 2017 Blocks Wearables Ltd.
   ----------------------------------------
   Contains logic for interation with KXTJ2 accelerometer.
*/
#pragma once
#include <stdint.h>
#include <stdbool.h>

// Default I2C address of KXTJ2 unit
#define KXTJ2_I2C_ADDRESS 0x1C


/**
 * User-provided funcion for writing to a register.
 * \param reg The number of the register to which to write.
 * \param value The value to write.
 * \return true if successful, false otherwise.
 */
extern bool Kxtj2_WriteRegister(uint8_t reg, uint8_t value);

/**
 * User-provided funcion for reading a single byte from a register.
 * \param reg The number of the register from which to read.
 * \param value_out Reference to the value to which to write.
 * \return true if successful, false otherwise.
 */
extern bool Kxtj2_ReadRegister(uint8_t reg, uint8_t* value_out);

/**
 * Gets the current X, Y and Z co-ordinates.
 * \param buffer The array to which to write the co-ordinates in order.
 */
void Kxtj2_GetXYZ(float buffer[3]);

/**
 * Enables the accelerometer.
 * \return true if successful, false otherwise.
 */
bool Kxtj2_PowerOff(void);

/**
 * Disables the accelerometer.
 * \return true if successful, false otherwise.
 */
bool Kxtj2_PowerOn(void);
