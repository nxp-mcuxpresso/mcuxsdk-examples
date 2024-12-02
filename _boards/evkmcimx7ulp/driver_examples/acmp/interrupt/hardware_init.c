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
#include "clock_config.h"
#include "board.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/

void BOARD_InitHardware(void)
{
    /*Enable PMC0->ACTRL BUFEN bit filed to make sure ACMP voltage reference available*/
    PMC0->ACTRL |= PMC0_ACTRL_BUFEN_MASK;

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
}

/*${function:end}*/
