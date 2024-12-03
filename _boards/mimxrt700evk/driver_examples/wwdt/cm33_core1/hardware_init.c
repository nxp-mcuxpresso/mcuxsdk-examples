/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
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
    BOARD_InitAHBSC();
    BOARD_InitPins();
    BOARD_BootClockRUN();

    BOARD_InitDebugConsole();

    CLOCK_EnableClock(kCLOCK_Gpio8);
    RESET_ClearPeripheralReset(kGPIO8_RST_SHIFT_RSTn);

    CLOCK_AttachClk(kLPOSC_to_WWDT2);
}
/*${function:end}*/
