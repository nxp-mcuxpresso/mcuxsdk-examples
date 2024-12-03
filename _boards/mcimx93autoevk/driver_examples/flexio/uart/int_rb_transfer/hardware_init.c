/*
 * Copyright 2023 NXP
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
    /* clang-format off */
    const clock_root_config_t flexioClkCfg = {
        .clockOff = false,
        .mux = 0, /* 24MHz oscillator source */
        .div = 1
    };
    BOARD_InitBootPins();
    BOARD_BootClockRUN();

    CLOCK_SetRootClock(kCLOCK_Root_Flexio1, &flexioClkCfg);
}
/*${function:end}*/
