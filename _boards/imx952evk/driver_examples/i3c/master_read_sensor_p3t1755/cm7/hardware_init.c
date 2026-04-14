/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "board.h"
#include "pin_mux.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* clang-format off */
    clk_t i3cClkCfg = {
        .clkId = I3C_MASTER_CLOCK_ROOT,
        .pclkId = kCLOCK_osc24m,
        .rate = 24000000UL,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
    };
    /* clang-format on */

    SystemPlatformInit();
    BOARD_InitBootPins();
    BOARD_InitI3CPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_SetRate(&i3cClkCfg);
    CLOCK_EnableClock(i3cClkCfg.clkId);
}
/*${function:end}*/
