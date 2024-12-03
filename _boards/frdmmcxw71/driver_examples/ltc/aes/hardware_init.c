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
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    /* Software enable of the XO analog and output to the 2.4GHz radio domain */
    RFMC->RF2P4GHZ_CTRL |= RFMC_RF2P4GHZ_CTRL_XO_EN(1);
}
/*${function:end}*/
