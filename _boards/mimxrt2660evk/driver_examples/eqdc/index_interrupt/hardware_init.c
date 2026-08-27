/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "fsl_xbar.h"
#include "fsl_modcon.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* Board common setting: MPU, Power and Clock Tree, TRDC, and Debug Console init. */
    BOARD_CommonSetting();
    BOARD_InitEQDCPins();

    /* PHASE_A: PIO3_0 (XBAR0_INOUT00, J94-13) -> XBAR0 -> EQDC0_PHASE_A
     * PHASE_B: PIO3_1 (XBAR0_INOUT01, J94-14) -> XBAR0 -> EQDC0_PHASE_B
     * INDEX:   PIO3_2 (XBAR0_INOUT02, J94-15) -> XBAR0 -> EQDC0_INDEX */

    /* Configure MODCON: set XBAR0 INOUT0, INOUT1, INOUT2 direction to Input. */
    uint32_t modconCfg = MODCON_GetCFG((uint32_t)kModCon_MAIN_IOMUX, 0U);
    MODCON_SetCFG((uint32_t)kModCon_MAIN_IOMUX, 0U, MODCON_CFG_DIR(modconCfg | (1U << 0U) | (1U << 1U) | (1U << 2U)));

    /* Configure XBAR0: connect pad inputs directly to EQDC signals. */
    XBAR_Init(kXBAR_HSP_XBAR0);
    XBAR_SetSignalsConnection(kHSP__XBAR_0_InputHspXbar0In0, kHSP__XBAR_0_OutputHspEqdc0PhaseAIn);
    XBAR_SetSignalsConnection(kHSP__XBAR_0_InputHspXbar0In1, kHSP__XBAR_0_OutputHspEqdc0PhaseBIn);
    XBAR_SetSignalsConnection(kHSP__XBAR_0_InputHspXbar0In2, kHSP__XBAR_0_OutputHspEqdc0IndexIn);
}
/*${function:end}*/
