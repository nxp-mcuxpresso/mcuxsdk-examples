/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include <stdint.h>
#include "fsl_common.h"
#include "fsl_debug_console.h"

void BOARD_InitDebugConsole(void)
{
    uint32_t uartClkSrcFreq = CLOCK_GetIpClkSrcFreq(BOARD_DEBUG_UART_CLOCK_NAME);
    DbgConsole_Init(BOARD_DEBUG_UART_INSTANCE, BOARD_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE, uartClkSrcFreq);
}
