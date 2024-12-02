/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_common.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* Release peripheral reset */
    RESET_ReleasePeripheralReset(kOPAMP0_RST_SHIFT_RSTn);

    /* Enable OPAMP */
    SPC0->ACTIVE_CFG1 |= 0x100;

    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}
/*${function:end}*/
