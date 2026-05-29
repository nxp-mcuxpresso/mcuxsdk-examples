/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "app.h"
#include "clock_config.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitFlexCANPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /*Clock setting for FLEXCAN*/
    CLOCK_SetClkDiv(kCLOCK_DivFlexcan012PeClk, 1U);
    CLOCK_AttachClk(kAIPS_PLAT_CLK_to_FLEXCAN012_PE);
}
/*${function:end}*/
