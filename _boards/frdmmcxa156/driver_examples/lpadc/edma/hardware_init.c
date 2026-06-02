/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "board.h"
#include "fsl_spc.h"
#include "pin_mux.h"
#include "fsl_common.h"
#include "clock_config.h"
#include "fsl_inputmux.h"

void BOARD_InitHardware(void)
{
    BOARD_InitDEBUG_UARTPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    CLOCK_SetupFRO16KClocking(kCLKE_16K_SYSTEM | kCLKE_16K_COREMAIN);

    CLOCK_AttachClk(kFRO12M_to_ADC1);
    CLOCK_SetClockDiv(kCLOCK_DivADC1, 1U);

    RESET_ReleasePeripheralReset(kINPUTMUX0_RST_SHIFT_RSTn);
    CLOCK_EnableClock(kCLOCK_GateINPUTMUX0);
    
    INPUTMUX_AttachSignal(INPUTMUX0, 0U, kINPUTMUX_Lptmr0ToAdc1Trigger);
}
