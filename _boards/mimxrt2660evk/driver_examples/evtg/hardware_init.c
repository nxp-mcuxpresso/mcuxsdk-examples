/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "app.h"
#include "fsl_xbar.h"
#include "fsl_modcon.h"
/*${header:end}*/

/*${function:start}*/
void IO_Config(void)
{
    /* Pin routing (configured by BOARD_InitEVTGPins):
     *   PIO3_0 -> XBAR1_INOUT00 (IBENA=1, input from external signal)
     *   PIO3_1 -> XBAR1_INOUT01 (IBENA=1, input from external signal)
     *   PIO2_26 -> XBAR0_INOUT26 (IBENA=1, output driven by XBAR0)
     *
     * Signal path:
     *   PIO3_0/PIO3_1 -> XBAR1 -> EVTG0 (INA0/INB0) -> EVTG0_OUTA0 -> XBAR0 -> PIO2_26
     */

    /* Configure MODCON: set XBAR1 INOUT0, INOUT1 direction to Input (CFG[1] controls XBAR1). */
    /* XBAR0 INOUT26 defaults to Output direction (CFG[0] bit26=0), no change needed. */
    uint32_t modconCfg = MODCON_GetCFG((uint32_t)kModCon_MAIN_IOMUX, 1U);
    MODCON_SetCFG((uint32_t)kModCon_MAIN_IOMUX, 1U, MODCON_CFG_DIR(modconCfg | (1U << 0U) | (1U << 1U)));

    /* XBAR1: route XBAR1_IN0/IN1 to EVTG0 INA0/INB0. */
    XBAR_Init(kXBAR_HSP_XBAR1);
    XBAR_SetSignalsConnection(kHSP__XBAR_1_InputHspXbar1In0, kHSP__XBAR_1_OutputHspEvtg0Ina0);
    XBAR_SetSignalsConnection(kHSP__XBAR_1_InputHspXbar1In1, kHSP__XBAR_1_OutputHspEvtg0Inb0);

    /* XBAR0: route EVTG0_OUTA0 to XBAR0_OUT26 (drives PIO2_26 via XBAR0 INOUT26). */
    XBAR_Init(kXBAR_HSP_XBAR0);
    XBAR_SetSignalsConnection(kHSP__XBAR_0_InputHspEvtg0Outa0, kHSP__XBAR_0_OutputHspXbar0Out26);
}

void BOARD_InitHardware(void)
{
    /* Board common setting: MPU, Power and Clock Tree, TRDC, and Debug Console init. */
    BOARD_CommonSetting();
    BOARD_InitEVTGPins();
    CLOCK_EnableClock(kCLOCK_MAIN_hsp_evtg0);
}
/*${function:end}*/
