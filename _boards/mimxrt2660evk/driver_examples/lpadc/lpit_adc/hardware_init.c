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
    BOARD_InitADCPin();

    /* LPIT0_CH0_TRIG_OUT -> XBAR2_IN26 -> XBAR2_OUT0 -> XBAR0_IN2 -> XBAR0_OUT10 -> ADC0_1_CH0_1_TRIG_IN0 */
    XBAR_Init(kXBAR_HSP_XBAR2);
    XBAR_SetSignalsConnection(kHSP__XBAR_2_InputHspLpit0TrigOut0, kHSP__XBAR_2_OutputHspXbar0Xbar1In2);
    XBAR_Init(kXBAR_HSP_XBAR0);
    XBAR_SetSignalsConnection(kHSP__XBAR_0_InputHspXbar2Out0, kHSP__XBAR_0_OutputHspAdc01Ch01TrigIn0);

    CLOCK_SetRootClockDiv(kCLOCK_Root_MAIN_adc0_fclk, 50u);
}
/*${function:end}*/
