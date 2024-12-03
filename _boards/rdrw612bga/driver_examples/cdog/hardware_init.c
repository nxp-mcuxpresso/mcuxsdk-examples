/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_power.h"
/*${header:end}*/

/*${variable:start}*/

/*${variable:end}*/
/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Disable ITRC reset trigger from CDOG */
    ITRC->OUT1_SEL0 = (ITRC->OUT1_SEL0 & ~ITRC_OUT1_SEL0_IN8_SEL0_MASK) | ITRC_OUT1_SEL0_IN8_SEL0(2);
}
/*${function:end}*/
