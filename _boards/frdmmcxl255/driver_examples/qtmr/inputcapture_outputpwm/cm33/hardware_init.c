/*
 * Copyright 2025 NXP
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
    BOARD_InitBootPins();
    BOARD_InitDebugConsole();

    /* Release peripheral reset */
    RESET_ReleasePeripheralReset(kAonQTMR0_RST_SHIFT_RSTn);
    
    INPUTMUX_Init(AON__INPUTMUX);
    INPUTMUX_AttachSignal(AON__INPUTMUX, kINPUTMUXAON_INDEX_QTMR1_TMR0, kINPUTMUXAON_AonTrigIn4ToQtmr1Tmrn);
    INPUTMUX_AttachSignal(AON__INPUTMUX, kINPUTMUXAON_INDEX_AON_TRIG_OUT0, kINPUTMUXAON_Qtmr1Channel0ToAonTrigOutn);

    BOARD_InitQTMRPins();
}
/*${function:end}*/