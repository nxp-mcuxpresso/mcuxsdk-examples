/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "fsl_smc.h"
#include "fsl_lpi2c.h"
#include "peripherals.h"
#include "board.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
/* Initialize debug console. */
void DEMO_InitDebugConsole(void)
{
    uint32_t uartClkSrcFreq;

    CLOCK_SetIpSrc(kCLOCK_Lpuart2, kCLOCK_IpSrcSircAsync);
    uartClkSrcFreq = CLOCK_GetIpFreq(kCLOCK_Lpuart2);

    DbgConsole_Init(BOARD_DEBUG_UART_INSTANCE, BOARD_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE, uartClkSrcFreq);
}

void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    DEMO_InitDebugConsole();

    BOARD_InitBootPeripherals();
}
/*${function:end}*/
