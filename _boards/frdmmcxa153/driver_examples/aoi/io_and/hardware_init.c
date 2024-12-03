/*
 * Copyright 2021 NXP
 * All rights reserved.
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

/*
 * Function description:
 * This function is initialize the TRIG_OUT
 */
void IO_Configuration(void)
{
    /* Two IOs are used for AOI input */
    INPUTMUX_AttachSignal(INPUTMUX0, kINPUTMUX_INDEX_AOI0_TRIGSEL0, kINPUTMUX_TrigIn0ToAoi0Mux);
    INPUTMUX_AttachSignal(INPUTMUX0, kINPUTMUX_INDEX_AOI0_TRIGSEL1, kINPUTMUX_TrigIn1ToAoi0Mux);
}

void BOARD_InitHardware(void)
{
    RESET_PeripheralReset(kAOI0_RST_SHIFT_RSTn);

    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    INPUTMUX_Init(INPUTMUX0);
    /* Set the AOI_OUT to TRIG_OUT0 */
    INPUTMUX_AttachSignal(INPUTMUX0, kINPUTMUX_INDEX_EXT_TRIGSEL0, kINPUTMUX_Aoi0Out0ToExtTrigger);
}

/*${function:end}*/
