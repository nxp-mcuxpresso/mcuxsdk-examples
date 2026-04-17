/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "fsl_clock.h"
#include "fsl_reset.h"
#include "board.h"
#include <stdbool.h>
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* Enter safe mode before initialize the external osc clock */
    CLOCK_AttachClk(kFRO12M_to_MAIN_CLK);
    CLOCK_SetupExtClocking(24000000U);

    BOARD_InitDEBUG_UARTPins();
    BOARD_InitFREQMEPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Attach peripheral clock */
    CLOCK_EnableClock(kCLOCK_GateFREQME);
    CLOCK_SetClockDiv(kCLOCK_DivFRO_HF, 1u);

    /* Release peripheral reset */
    RESET_ReleasePeripheralReset(kINPUTMUX0_RST_SHIFT_RSTn);
    
    CLOCK_SetupOsc32KClocking(kCLOCK_Osc32kToAll);
}
/*${function:end}*/
