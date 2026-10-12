/*
 * Copyright 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#if (IMXRTL3459AAA_cm55_core0_SERIES)
#include <stdint.h>
#include "fsl_common.h"
#include "fsl_clock.h"
#include "fsl_debug_console.h"
#include "board.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

void BOARD_InitDebugConsole(void)
{
    /* Source and enable the debug UART clock here, so examples that do not call
       BOARD_InitBootClocks() also get a working console. The peripheral reset is
       released by the flexcomm driver. */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);
    (void)CLOCK_EnableClock(BOARD_DEBUG_UART_PERIPH_CLK);
    DbgConsole_Init(BOARD_DEBUG_UART_INSTANCE, BOARD_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE, BOARD_DEBUG_UART_CLK_FREQ);
}

void BOARD_InitDebugConsoleSWO(void)
{
    DbgConsole_Init(BOARD_DEBUG_CONSOLE_SWO_PORT, BOARD_DEBUG_CONSOLE_SWO_BAUDRATE, kSerialPort_Swo, SystemCoreClock);
}

#endif