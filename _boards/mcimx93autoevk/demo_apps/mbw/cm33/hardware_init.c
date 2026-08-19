/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "app.h"
#include "pin_mux.h"
#include "board.h"
#include "clock_config.h"
#include "mbw_def.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* clang-format off */
    const clock_root_config_t lpitClkCfg = {
        .clockOff = false,
	.mux = 1,
	.div = 4
    };
    /* clang-format on */

    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_SetRootClock(DEMO_LPIT_CLOCK_NAME, &lpitClkCfg);
    CLOCK_EnableClock(DEMO_LPIT_CLOCK_GATE);
}
/*${function:end}*/
