/*
 * Copyright 2018 NXP
 * Copyright 2017-2018 NXP
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
    /* Initialize the pins. */
    BOARD_InitBootPins();
    /* Enable clock to 30MHz. */
    BOARD_InitBootClocks();
    /* Select the main clock as source clock of USART0. */
    CLOCK_Select(kUART0_Clk_From_MainClk);
}
/*${function:end}*/
