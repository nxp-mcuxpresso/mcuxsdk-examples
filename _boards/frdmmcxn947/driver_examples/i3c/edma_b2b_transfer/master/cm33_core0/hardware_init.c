/*
 * Copyright 2022-2023, 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    RESET_PeripheralReset(kDMA0_RST_SHIFT_RSTn);

    /* Attach FRO 12M to FLEXCOMM4 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom4Clk, 1U);

    /* Attach PLL0 clock to I3C, 150MHz / 6 = 25MHz. */
    CLOCK_AttachClk(kPLL0_to_I3C1FCLK);
    CLOCK_SetClkDiv(kCLOCK_DivI3c1FClk, 6U);

    SYSCON->CLOCK_CTRL |= SYSCON_CLOCK_CTRL_FRO1MHZ_CLK_ENA_MASK;
    CLOCK_AttachClk(kCLK_1M_to_I3C1FCLKS);
    CLOCK_SetClkDiv(kCLOCK_DivI3c1FClkS, 1U);

    BOARD_InitBootPins();
    BOARD_InitI3CPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}
/*${function:end}*/
