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
#include "app.h"
#include "fsl_spc.h"

/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* attach FRO HF to DAC0 */
    CLOCK_AttachClk(kFRO12M_to_DAC0);
    CLOCK_SetClockDiv(kCLOCK_DivDAC0, 1U);

    /* attach FRO HF to ADC0 */
    CLOCK_AttachClk(kFRO12M_to_ADC0);
    CLOCK_SetClockDiv(kCLOCK_DivADC0, 1U);

    /* Enable OPAMP and DAC */
    SPC_EnableActiveModeAnalogModules(SPC0, (kSPC_controlOpamp0 | kSPC_controlDac0));
}
/*${function:end}*/
