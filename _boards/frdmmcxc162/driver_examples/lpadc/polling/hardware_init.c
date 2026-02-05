/*
 * Copyright 2026 NXP
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
    BOARD_InitDEBUG_UARTPins();
    BOARD_InitADCPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Attach peripheral clock */
    CLOCK_SetClockDiv(kCLOCK_DivADC, 3u);
    CLOCK_AttachClk(kFRO_HF_to_ADC);
}
/*${function:end}*/
