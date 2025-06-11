/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_common.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_inputmux.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/

void BOARD_InitHardware(void)
{
    /* Attach peripheral clock */
    CLOCK_SetClockDiv(kCLOCK_DivADC0, 1u);
    CLOCK_AttachClk(kFRO12M_to_ADC0);

    CLOCK_SetupFRO16KClocking(kCLKE_16K_SYSTEM | kCLKE_16K_COREMAIN);

    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Enable INPUTMUX0 */
    CLOCK_EnableClock(kCLOCK_GateINPUTMUX0);
    RESET_ReleasePeripheralReset(kINPUTMUX0_RST_SHIFT_RSTn);

    /* Connect LPTMR trigger output to ADC trigger input */
    INPUTMUX_AttachSignal(INPUTMUX0, 0U, kINPUTMUX_Lptmr0ToAdc0Trigger);
}

/*${function:end}*/
