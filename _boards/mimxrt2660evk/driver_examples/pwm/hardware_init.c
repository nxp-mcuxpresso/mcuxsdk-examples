/*
 * Copyright 2018, 2026 NXP
 * All rights reserved.
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
    /* Board common setting: MPU, Power and Clock Tree, TRDC, and Debug Console init. */
    BOARD_CommonSetting();
    BOARD_InitPWMPins();

    /* Set the PWM Fault inputs to a low value */
    XBAR_Init(kXBAR_HSP_XBAR0);
    XBAR_SetSignalsConnection(kHSP_XBAR_0_InputLogicLow, kHSP_XBAR_0_OutputHspFlexpwm1FaultIn0);
    XBAR_SetSignalsConnection(kHSP_XBAR_0_InputLogicLow, kHSP_XBAR_0_OutputHspFlexpwm1FaultIn1);
    XBAR_SetSignalsConnection(kHSP_XBAR_0_InputLogicLow, kHSP_XBAR_0_OutputHspFlexpwm1FaultIn2);
    XBAR_SetSignalsConnection(kHSP_XBAR_0_InputLogicLow, kHSP_XBAR_0_OutputHspFlexpwm1FaultIn3);

    CLOCK_SetRootClockDiv(kCLOCK_Root_CGU_MAIN_ROOTCLK, 10u);
}
/*${function:end}*/
