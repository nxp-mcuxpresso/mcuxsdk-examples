/*
 * Copyright 2024, 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_common.h"
#include "pin_mux.h"
#include "board.h"
#include "app.h"

/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* Board common setting: MPU, Power and Clock Tree, TRDC, and Debug Console init. */
    BOARD_CommonSetting();
    BOARD_InitFLEXIO_UARTPins();

    CLOCK_EnableClock(kCLOCK_MAIN_hsp_flexio1);

    CLOCK_SetRootClockDiv(kCLOCK_Root_MAIN_flexio1_fclk, 10U);
}
/*${function:end}*/
