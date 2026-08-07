/*
 * Copyright 2024, 2026 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "fsl_clock.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* Board common setting: MPU, Power and Clock Tree, TRDC, and Debug Console init. */
    BOARD_CommonSetting();
    BOARD_InitFLEXIO_PINOUTPUTPins();
    CLOCK_EnableClock(kCLOCK_MAIN_hsp_flexio1);
    SystemCoreClockUpdate();

    CLOCK_SetRootClockDiv(kCLOCK_Root_MAIN_flexio0_fclk, 20U);
}
/*${function:end}*/
