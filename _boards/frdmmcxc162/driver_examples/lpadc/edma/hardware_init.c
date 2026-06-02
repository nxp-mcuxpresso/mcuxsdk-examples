/*
 * Copyright 2026 NXP
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
#include "fsl_edma_soc.h"
/*${header:end}*/

/*${function:start}*/

void BOARD_InitHardware(void)
{
    BOARD_InitDEBUG_UARTPins();
    BOARD_InitADCPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Attach peripheral clock */
    CLOCK_AttachClk(kFRO_HF_to_ADC);
    CLOCK_SetClockDiv(kCLOCK_DivADC, 3u);

    CLOCK_SetupFRO16KClocking(kCLOCK_Clk16kToSysAndCore);
    
    /* Release peripheral reset */
    RESET_ReleasePeripheralReset(kDMA0_RST_SHIFT_RSTn);

    /* Enable INPUTMUX0 */
    CLOCK_EnableClock(kCLOCK_GateINPUTMUX0);
    RESET_ReleasePeripheralReset(kINPUTMUX0_RST_SHIFT_RSTn);

    /* Connect LPTMR trigger output to ADC trigger input */
    INPUTMUX_AttachSignal(INPUTMUX0, 0U, kINPUTMUX_Lptmr0ToAdc0Trig);
}

/*${function:end}*/
