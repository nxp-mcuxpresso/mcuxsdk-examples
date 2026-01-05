/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include <stdbool.h>
#include "FreeRTOS.h"
#include "fsl_power.h"
/*${header:end}*/

/*${variable:start}*/

/*${variable:end}*/
/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitAHBSC();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    CLOCK_AttachClk(k32KHZ_WAKE_to_OSTIMER);
    CLOCK_SetClkDiv(kCLOCK_DivOstimerClk, 1U);
    RESET_ClearPeripheralReset(kPINT_RST_SHIFT_RSTn);
    CLOCK_EnableClock(kCLOCK_Pint);
}
/*${function:end}*/
