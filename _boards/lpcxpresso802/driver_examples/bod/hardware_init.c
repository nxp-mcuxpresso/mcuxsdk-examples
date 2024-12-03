/*
 * Copyright 2021 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include <stdbool.h>
#include "fsl_power.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* Attach 12 MHz clock to USART0 (debug console) */
    CLOCK_Select(BOARD_DEBUG_USART_CLK_ATTACH);
    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}

void APP_InitBod(void)
{
    POWER_SetBodLevel(kBod_ResetLevel0, kBod_InterruptLevel2, true);
    POWER_DisablePD(kPDRUNCFG_PD_BOD);
}

void APP_DeinitBod(void)
{
    POWER_SetBodLevel(kBod_ResetLevel0, kBod_InterruptLevelReserved, false);
}
/*${function:end}*/
