/*
 * Copyright 2022, 2026 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"

/*******************************************************************************
 * Code
 ******************************************************************************/
void BOARD_InitHardware(void)
{
    /* Board common setting: MPU, Power and Clock Tree, TRDC, and Debug Console init. */
    BOARD_CommonSetting();
    BOARD_InitFLEXIO_SPIPins();

    CLOCK_SetRootClockDiv(kCLOCK_Root_MAIN_flexio1_fclk, 10U);
    CLOCK_SetRootClockDiv(kCLOCK_Root_MAIN_lpspi1_fclk, 10U);
}
