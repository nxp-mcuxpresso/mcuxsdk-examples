/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "app.h"
#include "board.h"
#include "pin_mux.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* clang-format off */

    clk_t lpit_clk = {
        .clkId = LPIT_MASTER_CLOCK_ROOT,
        .pclkId = kCLOCK_Syspll1dfs1div2,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 133333333UL,
    };

    clk_t adc_clk = {
        .clkId = kCLOCK_Adc,
        .pclkId = kCLOCK_Syspll1dfs1div2,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 80000000UL,
    };

    /* clang-format on */
    SystemPlatformInit();
    BOARD_InitDebugConsolePins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_ConfigMPU();

    CLOCK_SetRate(&lpit_clk);
    CLOCK_EnableClock(lpit_clk.clkId);

    CLOCK_SetParent(&adc_clk);
    CLOCK_SetRate(&adc_clk);
    CLOCK_EnableClock(adc_clk.clkId);
}
/*${function:end}*/
