/*
 * Copyright 2018, 2026 NXP
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
    /* Board common setting: MPU, Power and Clock Tree, TRDC, and Debug Console init. */
    BOARD_CommonSetting();
    BOARD_InitFLEXIO_SPIPins();

#if !(defined(RT2660_PRESILICON_DEVELOPMENT) && (RT2660_PRESILICON_DEVELOPMENT == 1))
    CLOCK_SetRootClockDiv(kCLOCK_Root_MAIN_flexio1_fclk, 10U);
    CLOCK_SetRootClockDiv(kCLOCK_Root_MAIN_lpspi1_fclk, 10U);
#endif
}
/*${function:end}*/
