/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();

    /* Change from debug uart(lpuart1) to lpuart0 */
    uint32_t uartClkSrcFreq = BOARD_DEBUG_UART_CLK_FREQ;
    CLOCK_SetIpSrc(kCLOCK_Lpuart0, kCLOCK_IpSrcFro6M);
    DbgConsole_Init(0U, BOARD_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE, uartClkSrcFreq);
    CLOCK_SetIpSrc(kCLOCK_Lpuart0, kCLOCK_IpSrcFro6M);
}
/*${function:end}*/
