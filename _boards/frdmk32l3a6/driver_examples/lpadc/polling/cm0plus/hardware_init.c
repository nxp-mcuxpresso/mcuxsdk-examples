/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_intmux.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    CLOCK_SetIpSrcDiv(kCLOCK_Lpadc0, kCLOCK_IpSrcFircAsync, 2U, 0U);
    CLOCK_SetIpSrc(kCLOCK_Intmux0, kCLOCK_IpSrcFircAsync);

    INTMUX_Init(INTMUX1);
    /* Enable ADC intmux source on INTMUX channel 0 */
    INTMUX_EnableInterrupt(INTMUX1, 0, LPADC0_IRQn);
}
/*${function:end}*/
