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
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();

    BOARD_InitDebugConsole();

    CLOCK_AttachClk(kOSC_CLK_to_I3C01);
    CLOCK_SetClkDiv(kCLOCK_DivI3c01Clk, 1U);

    CLOCK_AttachClk(kMAIM_PLL_PFD0_to_I3C01_PCLK);
    CLOCK_SetClkDiv(kCLOCK_DivI3c01PClk, 1U);

    CLOCK_EnableClock(kCLOCK_I3c0);
    RESET_ClearPeripheralReset(kI3C0_RST_SHIFT_RSTn);
}
/*${function:end}*/
