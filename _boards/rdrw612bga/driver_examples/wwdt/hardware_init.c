/*
 * Copyright 2022 NXP
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

/*${keep_header:start}*/
#include "fsl_power.h"
/*${keep_header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    GPIO_PortInit(GPIO, BOARD_LED_BLUE_GPIO_PORT);

    CLOCK_AttachClk(kLPOSC_to_WDT0_CLK);
    POWER_EnableResetSource(kPOWER_ResetSourceWdt);

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}
/*${function:end}*/
