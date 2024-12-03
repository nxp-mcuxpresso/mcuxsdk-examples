/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "fsl_inputmux.h"
#include "app.h"
#include <stdbool.h>
/*${header:end}*/

/*${function:start}*/
void IO_Config(void)
{
    /* Two IOs are used for EVTG input */
    INPUTMUX_AttachSignal(INPUTMUX0, 0U, kINPUTMUX_TrigIn0ToEvtgTrigger);
    INPUTMUX_AttachSignal(INPUTMUX0, 1U, kINPUTMUX_TrigIn1ToEvtgTrigger);
}

void BOARD_InitHardware(void)
{
    /* Enables the clock for INPUTMUX: Enables clock */
    CLOCK_EnableClock(kCLOCK_InputMux0);
    /* Enables the clock for INPUTMUX: Enables clock */
    CLOCK_EnableClock(kCLOCK_Evtg);

    /* attach FRO 12M to FLEXCOMM4 (debug console) */
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom4Clk, 1u);
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    INPUTMUX_Init(INPUTMUX0);

    /* Set the EVTG_OUT to TRIG_OUT0 */
    INPUTMUX_AttachSignal(INPUTMUX0, 0U, kINPUTMUX_EvtgOut0AToExtTrigger);
}
/*${function:end}*/
