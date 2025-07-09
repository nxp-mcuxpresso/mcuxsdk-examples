/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_common.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_spc.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    SPC_EnableActiveModeAnalogModules(SPC0, (kSPC_controlOpamp0 | kSPC_controlOpamp1));

    CLOCK_SetClockDiv(kCLOCK_DivADC, 1U);
    CLOCK_AttachClk(kFRO_LF_DIV_to_ADC);
}
/*${function:end}*/
