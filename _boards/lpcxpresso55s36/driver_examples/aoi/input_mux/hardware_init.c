/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "board.h"
#include "pin_mux.h"
#include <stdbool.h>
#include "fsl_power.h"
#include "clock_config.h"
#include "fsl_inputmux.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/

/*
 * Function description:
 * This function is initialize the INPUTMUX module
 */
void Inputmux_Configuration(void)
{
    /*  Init input_mux */
    INPUTMUX_Init(DEMO_INPUTMUX_BASEADDR);

    INPUTMUX_AttachSignal(DEMO_INPUTMUX_BASEADDR, 0, kINPUTMUX_SctOut0ToAoi0InTrigger);
    INPUTMUX_AttachSignal(DEMO_INPUTMUX_BASEADDR, 1, kINPUTMUX_CompOutToAoi0InTrigger);
}

void BOARD_InitHardware(void)
{
    /* attach 12 MHz clock to FLEXCOMM0 (debug console) */
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom0Clk, 0u, false);
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom0Clk, 1u, true);
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    /* reset FLEXCOMM for USART */
    RESET_PeripheralReset(kFC0_RST_SHIFT_RSTn);

    BOARD_InitBootPins();
    BOARD_BootClockPLL150M();
    BOARD_InitDebugConsole();

    POWER_DisablePD(kPDRUNCFG_PD_COMP);
}
/*${function:end}*/
