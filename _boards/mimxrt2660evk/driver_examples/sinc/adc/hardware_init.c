/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* Board common setting: MPU, Power and Clock Tree, TRDC, and Debug Console init. */
    BOARD_CommonSetting();
    BOARD_InitSINCPins();

    CLOCK_SetRootClockMux(kCLOCK_Root_MAIN_sinc1_fclk, kCLOCK_SINC1_ClockRoot_PERI3);
}
/*${function:end}*/
