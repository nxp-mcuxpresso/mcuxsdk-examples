/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "clock_config.h"
#include "fsl_gpio.h"
#include "fsl_port.h"
#include "pin_mux.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* Structure for OSC configuration */
    osc_config_t oscConfig;
    oscConfig.freq                   = BOARD_XTAL0_CLK_HZ;
    oscConfig.capLoad                = 0U;
    oscConfig.workMode               = kOSC_ModeOscLowPower;
    oscConfig.oscerConfig.enableMode = kOSC_ErClkEnable;

    BOARD_InitBootPins();
    CLOCK_InitOsc0(&oscConfig);
    CLOCK_SetXtal0Freq(BOARD_XTAL0_CLK_HZ);
}
/*${function:end}*/
