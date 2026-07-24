/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "app.h"
#include "clock_config.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* Board common setting: MPU, Power and Clock Tree, TRDC, and Debug Console init. */
    BOARD_CommonSetting();
    BOARD_InitLEDsPins();

    /* Enable clock gate for GPIO1 */
    CLOCK_EnableClock(kCLOCK_MAIN_hsp_rgpio1);
}

void Led_Init(void)
{
    LED_1_INIT();
}
/*${function:end}*/
