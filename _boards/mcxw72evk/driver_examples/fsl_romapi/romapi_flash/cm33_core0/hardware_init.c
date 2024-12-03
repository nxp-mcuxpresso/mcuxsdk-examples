/*
 * Copyright 2021 NXP
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

    CIU2->CIU2_CPU_CPU2_CTRL |= CIU2_CIU2_CPU_CPU2_CTRL_CPU2_BOOT_IMEM_MUX_EN(1U);
}
/*${function:end}*/
