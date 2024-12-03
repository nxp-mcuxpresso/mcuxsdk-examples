/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "fsl_common.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_trgmux.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    TRGMUX_SetTriggerSource(TRGMUX0, kTRGMUX_Trgmux0Dac0, kTRGMUX_TriggerInput0, kTRGMUX_SourceLptmr0Trg);
}
/*${function:end}*/
