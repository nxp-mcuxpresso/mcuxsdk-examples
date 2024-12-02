/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include <stdbool.h>
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* attach 12 MHz clock to FLEXCOMM0 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);
    BOARD_InitPins();

    BOARD_InitBootClocks();
#if USE_CANOPEN_FD
    /* Set MCAN clock 80Mhz/1=80Mhz for CAN FD. */
    CLOCK_SetClkDiv(kCLOCK_DivCanClk, 1U, true);
#else
    /* Set MCAN clock 80Mhz/4=20Mhz for legacy CAN. */
    CLOCK_SetClkDiv(kCLOCK_DivCanClk, 4U, true);
#endif
    CLOCK_AttachClk(kMCAN_DIV_to_MCAN);
    /* Enable FRO 1M clock for WWDT module. */
    SYSCON->CLOCK_CTRL |= SYSCON_CLOCK_CTRL_FRO1MHZ_CLK_ENA_MASK;
    /* Set clock divider for WWDT clock source. */
    CLOCK_SetClkDiv(kCLOCK_DivWdtClk, 1U, true);

    BOARD_InitDebugConsole();
}
/*${function:end}*/
