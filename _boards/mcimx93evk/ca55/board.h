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

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief The board name */
#define BOARD_NAME        "MIMX93-EVK"
#define CPU_CORE_NAME     "Cortex-A55"
#define MANUFACTURER_NAME "NXP"

/* The UART to use for debug messages. */
#define BOARD_DEBUG_UART_TYPE     kSerialPort_Uart
#define BOARD_DEBUG_UART_BAUDRATE (115200U)

/* Use LPUART2 which is the M-Core debug console on the i.MX93 EVK. */
#ifndef BOARD_DEBUG_UART_INSTANCE
#define BOARD_DEBUG_UART_INSTANCE (2U)
#endif

#define BOARD_DEBUG_UART_BASEADDR LPUART2_BASE
#define BOARD_DEBUG_UART_CLK_FREQ (CLOCK_GetIpFreq(kCLOCK_Root_Lpuart2))
#define BOARD_UART_IRQ            LPUART2_IRQn
#define BOARD_UART_IRQ_HANDLER    LPUART2_IRQHandler

#define BOARD_DEBUG_CONSOLE_TYPE     BOARD_DEBUG_UART_TYPE
#define BOARD_DEBUG_CONSOLE_PORT     BOARD_DEBUG_UART_BASEADDR
#define BOARD_DEBUG_CONSOLE_BAUDRATE BOARD_DEBUG_UART_BAUDRATE

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
 * API
 ******************************************************************************/

void BOARD_InitClock(void);
void BOARD_InitDebugConsole(void);
void BOARD_InitMemory(void);
void BOARD_RdcInit(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _BOARD_H_ */
