/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Attach peripheral clock */
    CLOCK_SetClockDiv(kCLOCK_DivDAC0, 1u);
    CLOCK_AttachClk(kFRO_LF_DIV_to_DAC0);

    /* Enable DAC0 */
    SPC0->ACTIVE_CFG1 |= 0x10;
}
/*${function:end}*/
