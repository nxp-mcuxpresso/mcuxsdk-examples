/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_common.h"
#include "pin_mux.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* Enable clock */
    CLOCK_EnableClock(kCLOCK_GateAonAPB);
    CLOCK_EnableClock(kCLOCK_GateAonPORT);
    CLOCK_EnableClock(kCLOCK_GateAonGPIO);

    BOARD_InitBootClocks();
    BOARD_InitBootPins();
    BOARD_InitDebugConsole();
    BOARD_InitLPACMPPins();
    
    /* 10M to AON CMP0 */
    CLOCK_AttachClk(kFROdiv1_to_AON_CMP0);
}
/*${function:end}*/