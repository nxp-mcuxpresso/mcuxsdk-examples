/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_clock.h"
/*${header:end}*/

/*${variable:start}*/

/*${variable:end}*/
/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* attach FRO 12M to FLEXCOMM4 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom4Clk, 1u);

    /* attach TRACECLKDIV to TRACE */
    CLOCK_AttachClk(kTRACE_DIV_to_TRACE);
    CLOCK_SetClkDiv(kCLOCK_DivTraceClk, 2U);

    /* enable the Flash clock if not already set */
    CLOCK_SetupClockCtrl(SYSCON_CLOCK_CTRL_FRO12MHZ_ENA_MASK);

    /* Disable prefetch buffer and flash cache */
    SYSCON->NVM_CTRL |= SYSCON_NVM_CTRL_DIS_MBECC_ERR_INST_MASK | SYSCON_NVM_CTRL_DIS_MBECC_ERR_DATA_MASK;
    SYSCON->NVM_CTRL |= SYSCON_NVM_CTRL_DIS_FLASH_SPEC_MASK | SYSCON_NVM_CTRL_DIS_DATA_SPEC_MASK;
    SYSCON->NVM_CTRL |= SYSCON_NVM_CTRL_DIS_FLASH_DATA_MASK | SYSCON_NVM_CTRL_DIS_FLASH_INST_MASK | SYSCON_NVM_CTRL_DIS_FLASH_CACHE_MASK;
    SYSCON->LPCAC_CTRL |= SYSCON_LPCAC_CTRL_DIS_LPCAC_MASK;

    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}
/*${function:end}*/
