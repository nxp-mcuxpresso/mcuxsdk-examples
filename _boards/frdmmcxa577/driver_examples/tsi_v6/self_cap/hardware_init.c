/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "fsl_clock.h"
#include "fsl_reset.h"
#include "board.h"
#include "fsl_debug_console.h"
#include <stdbool.h>
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitDEBUG_UARTPins();
    BOARD_InitLEDsPins();
    BOARD_InitTSIPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* INPUTMUX0: Peripheral clock is enabled */
    CLOCK_EnableClock(kCLOCK_GateINPUTMUX0);
    /* INPUTMUX0 peripheral is released from reset */
    RESET_ReleasePeripheralReset(kINPUTMUX0_RST_SHIFT_RSTn);

    /* Enables the clk_16k[1] */
    CLOCK_SetupFRO16KClocking(kCLKE_16K_SYSTEM | kCLKE_16K_COREMAIN | kCLKE_16K_VBAT);
    CLOCK_SetupOsc32KClocking(kCLOCK_Osc32kToAll);

    CLOCK_SetClockDiv(kCLOCK_DivTSI0, 10);
    CLOCK_AttachClk(kFRO_HF_DIV_to_TSI0);
}
/*${function:end}*/
