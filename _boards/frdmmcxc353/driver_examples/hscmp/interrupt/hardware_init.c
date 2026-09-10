/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* Attach peripheral clock */
    CLOCK_AttachClk(kFRO_HF_DIV_to_CMP0_RR);
    CLOCK_SetClockDiv(kCLOCK_DivCMP0_FUNC, 4U);

    BOARD_InitDEBUG_UARTPins();
    BOARD_InitLEDsPins();
    BOARD_InitCMPPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Enable CMP0 and CMP0_DAC */
    SPC0->ACTIVE_CFG1 |= ((1U << 16U) | (1U << 20U));
}
/*${function:end}*/
