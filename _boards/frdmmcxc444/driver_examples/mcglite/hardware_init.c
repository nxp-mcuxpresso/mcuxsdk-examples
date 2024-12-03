/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "app.h"
#include "pin_mux.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_InitOsc0();
}

void BOARD_InitOsc0(void)
{
    const osc_config_t oscConfig = {.freq        = BOARD_XTAL0_CLK_HZ,
                                    .capLoad     = 0U,
                                    .workMode    = kOSC_ModeOscLowPower,
                                    .oscerConfig = {
                                        .enableMode = kOSC_ErClkEnable,
#if (defined(FSL_FEATURE_OSC_HAS_EXT_REF_CLOCK_DIVIDER) && FSL_FEATURE_OSC_HAS_EXT_REF_CLOCK_DIVIDER)
                                        .erclkDiv = 0U,
#endif
                                    }};

    CLOCK_InitOsc0(&oscConfig);

    /* Passing the XTAL0 frequency to clock driver. */
    CLOCK_SetXtal0Freq(BOARD_XTAL0_CLK_HZ);
}
/*${function:end}*/
