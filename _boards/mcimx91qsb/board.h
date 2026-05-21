/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _BOARD_H_
#define _BOARD_H_

#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "fsl_debug_console.h"
#include "fsl_clock.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief The board name */
#define BOARD_NAME                   "MIMX91-QSB"
#define MANUFACTURER_NAME            "NXP"

/* The UART to use for debug messages. */
#define BOARD_DEBUG_UART_INSTANCE    (1U)
#define BOARD_DEBUG_UART_BAUDRATE    (115200U)
#define BOARD_DEBUG_UART_TYPE        kSerialPort_Uart
#define BOARD_DEBUG_UART_CLOCK_ROOT  kCLOCK_Root_Lpuart1
#define BOARD_DEBUG_UART_CLOCK_GATE  kCLOCK_Lpuart1
#define BOARD_DEBUG_UART_CLK_FREQ    (CLOCK_GetIpFreq(BOARD_DEBUG_UART_CLOCK_ROOT))

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
 * API
 ******************************************************************************/

void BOARD_InitDebugConsole(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _BOARD_H_ */
