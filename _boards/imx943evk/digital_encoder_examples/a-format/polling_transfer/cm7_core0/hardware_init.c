/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "app.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* clang-format off */
    clk_t flexioClkCfg = {
        .clkId = DEMO_FLEXIO_CLOCK,
        .pclkId = kCLOCK_Syspll1dfs1div2, /* 400 MHz */
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 80000000UL, /* 80 MHz */
    };
    /* clang-format on */

    SystemPlatformInit();
    BOARD_InitDebugConsolePins();
    BOARD_InitFlexio1Pins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_ConfigMPU();

    CLOCK_SetParent(&flexioClkCfg);
    CLOCK_SetRate(&flexioClkCfg);
    CLOCK_EnableClock(flexioClkCfg.clkId);
}
/*${function:end}*/
