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
#include "fsl_power.h"
/*${header:end}*/

/*${variable:start}*/

/*${variable:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_BootClockPLL150M();
    BOARD_InitDebugConsole();

    /* attach main clock divide to FLEXCOMM0 (debug console) */
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom0Clk, 0U, false);
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom0Clk, 1U, true);
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    /* Use 12 MHz clock for the ctimer */
    CLOCK_SetClkDiv(kCLOCK_DivCtimer2Clk, 0U, false);
    CLOCK_SetClkDiv(kCLOCK_DivCtimer2Clk, 1U, true);
    CLOCK_AttachClk(kFRO_HF_to_CTIMER2);

    CLOCK_SetClkDiv(kCLOCK_DivDac0Clk, 1U, true);
    CLOCK_AttachClk(kMAIN_CLK_to_DAC0);

    /* Disable VREF power down */
    POWER_DisablePD(kPDRUNCFG_PD_VREF);

    /* Disable DAC0 power down */
    POWER_DisablePD(kPDRUNCFG_PD_DAC0);
}
/*${function:end}*/
