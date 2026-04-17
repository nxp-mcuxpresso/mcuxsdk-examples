/*
 * Copyright 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_common.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/

void BOARD_InitHardware(void)
{
    BOARD_InitDEBUG_UARTPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Attach peripheral clock */
    CLOCK_SetClockDiv(kCLOCK_DivADC, 1u);
    CLOCK_AttachClk(kFRO_HF_to_ADC);

    /* enable VREF */
    SPC0->ACTIVE_CFG1 |= 0x1;
}

/*${function:end}*/
