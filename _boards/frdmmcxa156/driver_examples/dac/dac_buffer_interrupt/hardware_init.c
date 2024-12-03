/*
 * Copyright 2024 NXP
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
    /* Attach peripheral clock */
    CLOCK_SetClockDiv(kCLOCK_DivDAC0, 1u);
    CLOCK_AttachClk(kFRO12M_to_DAC0);

    /* Release peripheral reset */
    RESET_ReleasePeripheralReset(kDAC0_RST_SHIFT_RSTn);

    /* Enable DAC0 */
    SPC0->ACTIVE_CFG1 |= 0x10;

    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}
/*${function:end}*/
