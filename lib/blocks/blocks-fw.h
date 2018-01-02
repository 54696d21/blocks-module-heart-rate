/*
 * Blocks Module Firmware
 *
 * Copyright (C) 2016 BayLibre, SAS - All Rights Reserved
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 *
 * Author(s) :
 * Neil Armstrong <narmstrong@baylibre.com>
 */

#ifndef __blocks_fw_H
#define __blocks_fw_H

#ifdef STM32L052xx
#   include "stm32l0xx_hal.h"
#   include "stm32l0xx_hal_dma.h"
#   include "stm32l0xx_hal_uart.h"
#elif defined(STM32L476xx)
#   include "stm32l4xx_hal.h"
#   include "stm32l4xx_hal_dma.h"
#   include "stm32l4xx_hal_uart.h"
#else
#	error Target not supported
#endif
UART_HandleTypeDef huart1;

/* Runtime Defines -----------------------------------------------------------*/

/* Change next lines to disable peripherals */
#define FW_DISABLE_I2C		0
#define FW_DISABLE_UART		0

#if FW_DISABLE_UART
#define FW_DISABLE_DEBUG	1
#define FW_DISABLE_INFO		1
#else
/* Change next lines to enable debug/verbuse UART output */
#define FW_DISABLE_DEBUG	0
#define FW_DISABLE_INFO		0
#endif

/* UART Debug/Info/Error Reporting System */
#if FW_DISABLE_UART
#define fw_snprintf(...) {}
#else
#define fw_snprintf(fmt, ...) { 					\
	char pbuf[256];							\
	int psize = 0;							\
	psize = mini_snprintf(pbuf, 256, fmt, ##__VA_ARGS__);		\
	HAL_UART_Transmit(&huart1, pbuf, psize, HAL_MAX_DELAY); 	\
}
#endif

#if !FW_DISABLE_DEBUG
#	define fw_debug(fmt, ...)	fw_snprintf("dbg: " fmt, ##__VA_ARGS__);
#else
#	define fw_debug(fmt, ...)	{}
#endif
#if !FW_DISABLE_INFO
#	define fw_info(fmt, ...)	fw_snprintf(fmt, ##__VA_ARGS__);
#else
#	define fw_info(fmt, ...)	{}
#endif
#define fw_err(fmt, ...)	fw_snprintf("err: " fmt, ##__VA_ARGS__);

#define CRLF	"\r\n"

#endif /* __blocks_fw_H */
