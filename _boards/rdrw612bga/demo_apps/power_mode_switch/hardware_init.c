/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_BootClockLPR();
    BOARD_InitDebugConsole();
    BOARD_InitSleepPinConfig();

    /* Disable T3 256M clock and SFRO. As a result, DTRNG and GDET are now not working.
     * This is to demonstrate low runtime power. */
    CLOCK_DisableClock(kCLOCK_T3PllMci256mClk);
    /* Deinitialize T3 clocks */
    CLOCK_DeinitT3RefClk();

    CLOCK_EnableXtal32K(true);
    CLOCK_AttachClk(kXTAL32K_to_CLK32K);
    CLOCK_AttachClk(kLPOSC_to_OSTIMER_CLK);
}
/*${function:end}*/
