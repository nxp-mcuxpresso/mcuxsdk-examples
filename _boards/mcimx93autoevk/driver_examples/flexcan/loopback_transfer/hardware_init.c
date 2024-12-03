/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "app.h"
#include "board.h"
#include "pin_mux.h"
#include "fsl_common.h"
#include "clock_config.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* clang-format off */

    const clock_root_config_t flexcanClkCfg = {
        .clockOff = false,
	.mux = 2,
	.div = 10
    };
    /* clang-format on */
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_SetRootClock(FLEXCAN_CLOCK_ROOT, &flexcanClkCfg);
    CLOCK_EnableClock(FLEXCAN_CLOCK_GATE);
}
/*${function:end}*/
