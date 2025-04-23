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
#include "fsl_inputmux.h"
#include "fsl_inputmux_connections.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootClocks();
    BOARD_InitSWD_DEBUGPins();
    BOARD_InitDEBUG_UARTPins();
    BOARD_InitDebugConsole();
    BOARD_InitQTMRPins();
    RESET_ReleasePeripheralReset(kAonQTMR0_RST_SHIFT_RSTn);
    
    INPUTMUX_Init(AON__INPUTMUX);
    INPUTMUX_AttachSignal(AON__INPUTMUX, kINPUTMUXAON_INDEX_QTMR1_TMR0, kINPUTMUXAON_AonTrigIn0ToQtmr1Tmrn);
}
/*${function:end}*/
