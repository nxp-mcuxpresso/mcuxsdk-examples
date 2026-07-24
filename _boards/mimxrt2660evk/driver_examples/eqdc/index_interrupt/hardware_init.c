/*
 * Copyright 2026 NXP
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
    BOARD_InitEQDCPins();

    /* J94-13 (PIO3_0/XBAR1_INOUT0) -> XBAR0_IN52 -> XBAR0_OUT123 -> EQDC0_PHASE_A
     * J94-14 (PIO3_1/XBAR1_INOUT1) -> XBAR0_IN53 -> XBAR0_OUT124 -> EQDC0_PHASE_B
     * J94-15 (PIO3_2/XBAR1_INOUT2) -> XBAR0_IN54 -> XBAR0_OUT125 -> EQDC0_INDEX */
    XBAR_Init(kXBAR_HSP_XBAR0);
    XBAR_SetSignalsConnection(kHSP_XBAR_0_InputHspXbar1In0, kHSP_XBAR_0_OutputHspEqdc0PhaseAIn);
    XBAR_SetSignalsConnection(kHSP_XBAR_0_InputHspXbar1In1, kHSP_XBAR_0_OutputHspEqdc0PhaseBIn);
    XBAR_SetSignalsConnection(kHSP_XBAR_0_InputHspXbar1In2, kHSP_XBAR_0_OutputHspEqdc0IndexIn);
}
/*${function:end}*/
