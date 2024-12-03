/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "fsl_inputmux.h"
/*${header:end}*/

/*${function:start}*/

void Inputmux_Configuration(void)
{
    /*  Init input_mux */
    INPUTMUX_Init(DEMO_INPUTMUX_BASEADDR);
    INPUTMUX_AttachSignal(DEMO_INPUTMUX_BASEADDR, 0, kINPUTMUX_SctOut4ToOpamp0Trigger);
}
void BOARD_InitHardware(void)
{
    /* attach FRO 12M to FLEXCOMM4 (debug console) */
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom4Clk, 1u);
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    /* attach FRO HF to SCT */
    CLOCK_SetClkDiv(kCLOCK_DivSctClk, 1u);
    CLOCK_AttachClk(kFRO_HF_to_SCT);

    /* enable analog module */
    SPC0->ACTIVE_CFG1 |= 0x100;

    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}
/*${function:end}*/
