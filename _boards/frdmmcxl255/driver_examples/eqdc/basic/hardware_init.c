/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "fsl_inputmux.h"
#include "fsl_inputmux_connections.h"
#include "fsl_debug_console.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    CLOCK_EnableClock(kCLOCK_GateINPUTMUX0);
    CLOCK_EnableClock(kCLOCK_GateLPUART2);
    CLOCK_EnableClock(kCLOCK_GatePERIPH_GROUP1);

    /* Release peripheral RESET */
    RESET_PeripheralReset(kPORT1_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kQDC0_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kINPUTMUX0_RST_SHIFT_RSTn);

    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    INPUTMUX_Init(DEMO_INPUTMUX);
    INPUTMUX_AttachSignal(DEMO_INPUTMUX, 0, kINPUTMUX_TrigIn6ToQdc0Phasea);
    INPUTMUX_AttachSignal(DEMO_INPUTMUX, 0, kINPUTMUX_TrigIn3ToQdc0Phaseb);
    INPUTMUX_AttachSignal(DEMO_INPUTMUX, 0, kINPUTMUX_TrigIn0ToQdc0Index);
}
/*${function:end}*/
