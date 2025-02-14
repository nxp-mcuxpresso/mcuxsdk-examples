/*
 * Copyright 2024-2025 NXP
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
    CLOCK_SetIpSrc(kCLOCK_Lpuart0, kCLOCK_IpSrcFro192M);
    CLOCK_SetIpSrcDiv(kCLOCK_Lpuart0, 15U);

    uint32_t uartClkSrcFreq = (CLOCK_GetIpFreq(kCLOCK_Lpuart0));

    DbgConsole_Init(0U, BOARD_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE, uartClkSrcFreq);
}
/*${function:end}*/
