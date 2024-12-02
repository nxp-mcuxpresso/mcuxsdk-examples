/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_common.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_debug_console.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* TODO: Use SMC functions. */
    SMC->PMPROT = 0xAAU;
    BOARD_InitBootPins();

    CLOCK_SetIpSrc(kCLOCK_Lpuart1, kCLOCK_IpSrcSircAsync);

    uint32_t uartClkSrcFreq = CLOCK_GetIpFreq(kCLOCK_Lpuart1);

    DbgConsole_Init(BOARD_DEBUG_UART_INSTANCE, BOARD_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE, uartClkSrcFreq);
}
/*${function:end}*/
