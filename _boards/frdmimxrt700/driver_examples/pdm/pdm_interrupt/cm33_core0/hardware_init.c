/*
 * Copyright 2026 NXP
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
    BOARD_InitBootPins();
    BOARD_InitDMICPins();
    BOARD_BootClockRUN();

    BOARD_InitDebugConsole();

    CLOCK_AttachClk(kAUDIO_PLL_PFD3_to_MICFIL0);
    CLOCK_SetClkDiv(kCLOCK_DivMicfil0Clk, 15U);

    RESET_ClearPeripheralReset(kPDM_RST_SHIFT_RSTn);
}
/*${function:end}*/
