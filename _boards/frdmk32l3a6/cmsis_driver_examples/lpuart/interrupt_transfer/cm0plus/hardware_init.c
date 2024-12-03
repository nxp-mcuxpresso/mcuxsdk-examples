/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "app.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_intmux.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_BootClockRUN();
    INTMUX_Init(INTMUX1);
    /* Enable LPUART0 intmux source on INTMUX channel 0 */
    INTMUX_EnableInterrupt(INTMUX1, 0, LPUART0_IRQn);
    CLOCK_SetIpSrc(kCLOCK_Lpuart0, kCLOCK_IpSrcFircAsync);
}

uint32_t LPUART0_GetFreq(void)
{
    return CLOCK_GetIpFreq(kCLOCK_Lpuart0);
}

uint32_t LPUART1_GetFreq(void)
{
    return CLOCK_GetIpFreq(kCLOCK_Lpuart1);
}

uint32_t LPUART2_GetFreq(void)
{
    return CLOCK_GetIpFreq(kCLOCK_Lpuart2);
}
/*${function:end}*/
