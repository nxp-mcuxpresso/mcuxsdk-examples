/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _BOARD_H_
#define _BOARD_H_

#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "fsl_clock.h"
#include "clock_config.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief The board name */
#define BOARD_NAME        "MIMX943-EVK"
#define MANUFACTURER_NAME "NXP"

/* The UART to use for debug messages. */
#define BOARD_DEBUG_UART_TYPE     kSerialPort_Uart
#define BOARD_DEBUG_UART_BAUDRATE (115200U)
#define BOARD_DEBUG_UART_INSTANCE (8U) /* Use LPUART8 */

#define BOARD_DEBUG_CONSOLE_TYPE BOARD_DEBUG_UART_TYPE
#define BOARD_DEBUG_CONSOLE_PORT BOARD_DEBUG_UART_INSTANCE
#define BOARD_DEBUG_CONSOLE_BAUDRATE BOARD_DEBUG_UART_BAUDRATE

#define BOARD_UART_IRQ            LPUART8_IRQn
#define BOARD_UART_IRQ_HANDLER    LPUART8_IRQHandler

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
 * API
 ******************************************************************************/

void BOARD_InitClock(void);
void BOARD_InitDebugConsole(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _BOARD_H_ */
