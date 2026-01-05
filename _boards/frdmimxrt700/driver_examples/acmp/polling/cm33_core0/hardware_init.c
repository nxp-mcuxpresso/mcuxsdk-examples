/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "fsl_reset.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitCMPPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    RESET_PeripheralReset(kACMP0_RST_SHIFT_RSTn);
}
/*${function:end}*/
