/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include <stdbool.h>
#include "app.h"
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_inputmux.h"
#include "fsl_reset.h"
/*${header:end}*/

/*${function:start}*/

void IO_Configuration(void)
{
    /* Attach trigger inputs to AOI0 MUX0 and MUX1 */
    INPUTMUX_AttachSignal(INPUTMUX0, kINPUTMUX_INDEX_AOI0_MUX0, kINPUTMUX_TrigIn1ToAoi0Muxn);
    INPUTMUX_AttachSignal(INPUTMUX0, kINPUTMUX_INDEX_AOI0_MUX1, kINPUTMUX_TrigIn2ToAoi0Muxn);
}

void BOARD_InitHardware(void)
{
    /* Release peripheral reset */
    RESET_ReleasePeripheralReset(kAOI0_RST_SHIFT_RSTn);
    
    CLOCK_EnableClock(kCLOCK_GateAOI0);

    BOARD_InitDEBUG_UARTPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    INPUTMUX_Init(INPUTMUX0);
    
    /* Route AOI0 output 0 to external trigger */
    INPUTMUX_AttachSignal(INPUTMUX0, kINPUTMUX_INDEX_EXT_TRIG0, kINPUTMUX_Aoi0Out0ToExtTrign);
}

/*${function:end}*/