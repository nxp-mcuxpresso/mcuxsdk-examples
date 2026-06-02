/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_spc.h"
/*${header:end}*/

/*${variable:start}*/

/*${variable:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* attach FRO 12M to FLEXCOMM4 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom4Clk, 1U);

    /* Use FRO HF clock for some of the ctimer */
    CLOCK_AttachClk(kFRO_HF_to_CTIMER4);
    CLOCK_SetClkDiv(kCLOCK_DivCtimer4Clk, 4U);

    /* attach FRO HF to DAC0 */
    CLOCK_AttachClk(kFRO_HF_to_DAC0);
    CLOCK_SetClkDiv(kCLOCK_DivDac0Clk, 1U);

    /* enable DAC0 and VREF */
    SPC_EnableActiveModeAnalogModules(SPC0, (kSPC_controlVref | kSPC_controlDac0));

    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}
/*${function:end}*/
