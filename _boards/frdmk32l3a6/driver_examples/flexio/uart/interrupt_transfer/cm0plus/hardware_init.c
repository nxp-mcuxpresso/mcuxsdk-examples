/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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
    INTMUX_Init(INTMUX1);
    INTMUX_EnableInterrupt(INTMUX1, 0, FLEXIO0_IRQn);
    /* Set PCC FLEXIO0 selection */
    CLOCK_SetIpSrc(kCLOCK_Flexio0, kCLOCK_IpSrcFircAsync);
}
/*${function:end}*/
