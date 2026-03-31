/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_common.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_trgmux.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Enable trigger mux. */
    CLOCK_EnableClock(kCLOCK_Trgmux);
    /* Set trigger mux always high as lcu input0. */
    TRGMUX_SetTriggerSource(TRGMUX, kTRGMUX_Lcu0_0, kTRGMUX_TriggerInput0, kTRGMUX_SourceAlwaysHigh);
}
/*${function:end}*/
