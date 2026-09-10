/*
 * Copyright 2024, 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "fsl_xbar.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    RESET_ReleasePeripheralReset(kFLEXPWM0_RST_SHIFT_RSTn);
    RESET_ReleasePeripheralReset(kXBAR0_RST_SHIFT_RSTn);

    BOARD_InitDEBUG_UARTPins();
    BOARD_InitPWMPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    XBAR_Init(kXBAR0);
    XBAR_SetSignalsConnection(kXBAR0_InputLogicLow, kXBAR0_OutputPwmFault0);
    XBAR_SetSignalsConnection(kXBAR0_InputLogicLow, kXBAR0_OutputPwmFault1);
    XBAR_SetSignalsConnection(kXBAR0_InputLogicLow, kXBAR0_OutputPwmFault2);
    XBAR_SetSignalsConnection(kXBAR0_InputLogicLow, kXBAR0_OutputPwmFault3);
}
/*${function:end}*/
