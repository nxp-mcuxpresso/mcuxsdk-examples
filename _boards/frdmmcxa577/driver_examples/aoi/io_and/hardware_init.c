/*
 * Copyright 2026 NXP
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
    /*  Trigin10 connect to Aoi0Input 0 */
    INPUTMUX_AttachSignal(INPUTMUX0, kINPUTMUX_INDEX_AOI0_TRIGSEL0, kINPUTMUX_Trigin10ToAoi0input);
    /*  Trigin11 connect to Aoi0Input 1 */
    INPUTMUX_AttachSignal(INPUTMUX0, kINPUTMUX_INDEX_AOI0_TRIGSEL1, kINPUTMUX_Trigin11ToAoi0input);
}

void BOARD_InitHardware(void)
{
    BOARD_InitDEBUG_UARTPins();
    BOARD_InitAOIPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    INPUTMUX_Init(INPUTMUX0);
    /* Set the AOI_OUT to TRIG_OUT3 */
    INPUTMUX_AttachSignal(INPUTMUX0, kINPUTMUX_INDEX_TRIG_OUTSEL3, kINPUTMUX_Aoi0out0ToTrigout);
}

/*${function:end}*/
