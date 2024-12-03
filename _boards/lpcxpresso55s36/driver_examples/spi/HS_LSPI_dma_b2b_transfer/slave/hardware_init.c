/*
 * Copyright  2017 NXP
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
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom0Clk, 0u, false);
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom0Clk, 1u, true);
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    BOARD_InitPins();
    BOARD_BootClockPLL150M();
    BOARD_InitDebugConsole();

    CLOCK_SetClkDiv(kCLOCK_DivPllClk, 0U, true);  /*!< Reset PLLDIV divider counter and halt it */
    CLOCK_SetClkDiv(kCLOCK_DivPllClk, 3U, false); /*!< Set PLLDIV divider to value 3 */

    /* attach 50 MHz clock to HSLSPI */
    CLOCK_AttachClk(kPLL0_to_PLLCLKDIV); /*!< Switch PLLCLKDIV to PLL0 */
    CLOCK_AttachClk(kPLL_CLK_DIV_to_HSLSPI);
}
/*${function:end}*/
