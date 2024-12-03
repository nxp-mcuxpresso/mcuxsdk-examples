/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
/*${header:end}*/

/*${variable:start}*/
/*${variable:end}*/
/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* attach FRO 12M to FLEXCOMM4 (debug console) */
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom4Clk, 1u);
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    CLOCK_EnableClock(kCLOCK_Freqme);

    CLOCK_SetupExtClocking(BOARD_XTAL0_CLK_HZ);

    CLOCK_SetupFROHFClocking(CLK_FRO_144MHZ);

    CLOCK_SetupClockCtrl(kCLOCK_FRO_HF_ENA | kCLOCK_FRO12MHZ_ENA | kCLOCK_CLKIN_ENA_FM_USBH_LPT);
}
/*${function:end}*/
