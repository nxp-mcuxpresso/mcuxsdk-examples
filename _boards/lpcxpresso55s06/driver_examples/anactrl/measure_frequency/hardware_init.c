/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include <stdbool.h>
#include "board.h"
#include "pin_mux.h"
#include "fsl_power.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* attach 12 MHz clock to FLEXCOMM0 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);
    BOARD_InitPins();
    BOARD_BootClockFROHF96M();
    BOARD_InitDebugConsole();

    CLOCK_SetClkDiv(kCLOCK_DivAhbClk, 2U, true);

    CLOCK_EnableClock(kCLOCK_Freqme);
    POWER_DisablePD(kPDRUNCFG_PD_FRO32K);
    /* Enable FRO 1 MHz clock for Frequency Measure module. */
    SYSCON->CLOCK_CTRL |= SYSCON_CLOCK_CTRL_FRO1MHZ_UTICK_ENA_MASK;
}
/*${function:end}*/
