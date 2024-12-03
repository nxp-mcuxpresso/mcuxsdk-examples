/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* attach 12 MHz clock to FLEXCOMM0 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Div by 2 to get 48MHz, no divider reset */
    CLOCK_SetClkDiv(kCLOCK_DivFrohfClk, 2, false);
    /* attach 48 MHz clock to FLEXCOMM4 */
    CLOCK_AttachClk(kFRO_HF_to_FLEXCOMM4);
}
/*${function:end}*/
