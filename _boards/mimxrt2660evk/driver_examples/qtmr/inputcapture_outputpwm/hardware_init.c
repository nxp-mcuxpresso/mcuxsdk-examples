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
    BOARD_InitQTMRPins();

    /* Input:  PIO3_0 (XBAR0_INOUT00, J94-13) -> XBAR0 -> QTMR0_IN0
     * Output: QTMR0_OUT1 -> XBAR2 -> XBAR0_IN3 -> XBAR0_OUT1 -> PIO3_1 (XBAR0_INOUT01, J94-14) */

    /* Configure MODCON: set XBAR0 INOUT0 direction to Input (INOUT1 is output, default). */
    uint32_t modconCfg = MODCON_GetCFG((uint32_t)kModCon_MAIN_IOMUX, 0U);
    MODCON_SetCFG((uint32_t)kModCon_MAIN_IOMUX, 0U, MODCON_CFG_DIR(modconCfg | (1U << 0U)));

    /* Input path: XBAR0_INOUT00 -> QTMR0_IN0. */
    XBAR_Init(kXBAR_HSP_XBAR0);
    XBAR_SetSignalsConnection(kHSP__XBAR_0_InputHspXbar0In0, kHSP__XBAR_0_OutputHspQtmr0In0);

    /* Output path: QTMR0_OUT1 -> XBAR2 -> XBAR0_IN3 -> XBAR0_OUT1 -> XBAR0_INOUT01 (PIO3_1). */
    XBAR_Init(kXBAR_HSP_XBAR2);
    XBAR_SetSignalsConnection(kHSP__XBAR_2_InputHspQtmr0Out1, kHSP__XBAR_2_OutputHspXbar0Xbar1In3);
    XBAR_SetSignalsConnection(kHSP__XBAR_0_InputHspXbar2Out1, kHSP__XBAR_0_OutputHspXbar0Out1);
}
/*${function:end}*/
