/* ppg.h - (c) 2017 Blocks Wearables Ltd. */
#pragma once
#include <stdint.h>

/**
 * Initializes the PPG. Call before using other functions.
 * \return true if successful, false otherwise.
 */
bool PPG_Init(void);

/**
 * Enables the PPG.
 * \return true if successful, false otherwise.
 */
bool PPG_Enable(void);

/**
 * Disables the PPG.
 * \return true if successful, false otherwise.
 */
bool PPG_Disable(void);

/**
 * Resets the PPG.
 * \return true if successful, false otherwise.
 */
bool PPG_Reset(void);

/**
 * Reads the current heart rate value.
 * \param value_out Reference to the value to which to write the result.
 * \return true if successful, false otherwise.
 */
uint8_t PPG_GetHR(float* value_out);

/**
 * Reads the current raw heart rate data.
 * \param buffer Array to which to write the result.
 * \param length Length of given array.
 * \return Number of bytes written to array, zero if error.
 */
size_t PPG_GetRawData(uint8_t* buffer, size_t length);

/**
 * Gets the rate at which heart rate data is refreshed.
 * \return The rate of heart rate data update.
 */
uint8_t PPG_GetRate(void);
