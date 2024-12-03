/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "board.h"
#include "pin_mux.h"
#include "app.h"
#include "clock_config.h"
#include "fsl_iomuxc.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* clang-format off */
    const clock_root_config_t flexioClkCfg = {
        .clockOff = false,
	.mux = 0, /* 24MHz oscillator source */
	.div = 1
    };
    /* clang-format on */

    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_SetRootClock(kCLOCK_Root_Flexio1, &flexioClkCfg);
    CLOCK_EnableClock(kCLOCK_Flexio1);
}
/*${function:end}*/
