/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

/* Initialize debug console. */
void BOARD_InitDebugConsole(void)
{
    const clock_root_config_t uartClkCfg = {
        .clockOff = false,
        .mux = 0, // 24MHz oscillator source
        .div = 1
    };

    CLOCK_SetRootClock(BOARD_DEBUG_UART_CLOCK_ROOT, &uartClkCfg);
    CLOCK_EnableClock(BOARD_DEBUG_UART_CLOCK_GATE);

    DbgConsole_Init(BOARD_DEBUG_UART_INSTANCE, BOARD_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE,
                    BOARD_DEBUG_UART_CLK_FREQ);
}
