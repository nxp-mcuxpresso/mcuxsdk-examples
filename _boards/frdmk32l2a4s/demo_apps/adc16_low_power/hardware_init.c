/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_trgmux.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}

void BOARD_ConfigTriggerSource(void)
{
    TRGMUX_SetTriggerSource(TRGMUX0, kTRGMUX_Trgmux0Adc0, kTRGMUX_TriggerInput0, kTRGMUX_SourceLptmr0Trg);
}
/*${function:end}*/
