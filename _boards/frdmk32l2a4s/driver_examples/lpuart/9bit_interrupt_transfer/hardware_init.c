/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "clock_config.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_intmux.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    CLOCK_SetIpSrc(kCLOCK_Lpuart2, kCLOCK_IpSrcSircAsync);

    INTMUX_Init(INTMUX0);
    INTMUX_EnableInterrupt(INTMUX0, 0, LPUART2_IRQn);
}
/*${function:end}*/
