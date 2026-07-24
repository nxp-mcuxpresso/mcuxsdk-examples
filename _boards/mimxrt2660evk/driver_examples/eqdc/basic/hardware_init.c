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

    /* Route encoder signals via XBAR1 INOUT → XBAR0 → EQDC0
     * PIO3_0 (J94-13) → HSP_XBAR1_INOUT00 → XBAR0_In86 → EQDC0_PHASE_A
     * PIO3_1 (J94-14) → HSP_XBAR1_INOUT01 → XBAR0_In87 → EQDC0_PHASE_B
     * PIO3_2 (J94-15) → HSP_XBAR1_INOUT02 → XBAR0_In88 → EQDC0_INDEX
     * Pin mux for PIO3_0~2 as XBAR1_INOUT must be set in pin_mux.c */
    XBAR_Init(kXBAR_HSP_XBAR0);
    XBAR_SetSignalsConnection(kHSP_XBAR_0_InputHspXbar1In0, kHSP_XBAR_0_OutputHspEqdc0PhaseAIn);
    XBAR_SetSignalsConnection(kHSP_XBAR_0_InputHspXbar1In1, kHSP_XBAR_0_OutputHspEqdc0PhaseBIn);
    XBAR_SetSignalsConnection(kHSP_XBAR_0_InputHspXbar1In2, kHSP_XBAR_0_OutputHspEqdc0IndexIn);
}
/*${function:end}*/
