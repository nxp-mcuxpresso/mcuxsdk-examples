/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_clock.h"
#include "fsl_reset.h"
#include "board.h"
/*${header:end}*/

/*${variable:start}*/

/*${variable:end}*/
/*${function:start}*/
void BOARD_InitHardware(void)
{
    CLOCK_EnableClock(kCLOCK_GateADC0);

    /* Release ADC peripheral RESET */
    RESET_PeripheralReset(kADC0_RST_SHIFT_RSTn);

    BOARD_InitDEBUG_UARTPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}
/*${function:end}*/