/*
 * Copyright 2019-2023, 2026 NXP
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
#define BOARD_NAME        "MIMX8MM-EVK"
#define CPU_CORE_NAME     "Cortex-A53"
#define MANUFACTURER_NAME "NXP"
#define BOARD_DOMAIN_ID   (0U)

/* The UART to use for debug messages. */
#define BOARD_DEBUG_UART_TYPE     kSerialPort_Uart
#define BOARD_DEBUG_UART_BAUDRATE (115200U)
#define BOARD_DEBUG_UART_INSTANCE (4U) /* Use UART4 for the M-Core/A53 debug console */

#define BOARD_DEBUG_UART_BASEADDR UART4_BASE
#define BOARD_DEBUG_UART_CLK_FREQ (CLOCK_GetClockRootFreq(kCLOCK_Uart4ClkRoot))
#define BOARD_UART_IRQ            UART4_IRQn
#define BOARD_UART_IRQ_HANDLER    UART4_IRQHandler

#define BOARD_DEBUG_CONSOLE_TYPE     BOARD_DEBUG_UART_TYPE
#define BOARD_DEBUG_CONSOLE_PORT     BOARD_DEBUG_UART_BASEADDR
#define BOARD_DEBUG_CONSOLE_BAUDRATE BOARD_DEBUG_UART_BAUDRATE

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
 * API
 ******************************************************************************/

void BOARD_InitDebugConsole(void);
void BOARD_InitDebugConsolePins(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _BOARD_H_ */
