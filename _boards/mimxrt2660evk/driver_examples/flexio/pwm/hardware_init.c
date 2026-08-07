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
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* Board common setting: MPU, Power and Clock Tree, TRDC, and Debug Console init. */
    BOARD_CommonSetting();
    BOARD_InitFLEXIO_PWMPins();

    /* Enable FlexIO clock gate - clock is already configured by BOARD_InitBootClocks() */
    CLOCK_EnableClock(kCLOCK_MAIN_hsp_flexio1);

    CLOCK_SetRootClockDiv(kCLOCK_Root_MAIN_flexio0_fclk, 20U);
}
/*${function:end}*/
