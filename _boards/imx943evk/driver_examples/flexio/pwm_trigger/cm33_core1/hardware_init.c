/*
 * Copyright 2025 NXP
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
    clk_t hal_flexioClkCfg = {
        .clkId = DEMO_FLEXIO_CLOCK,
        .pclkId = kCLOCK_Osc24m,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 24000000UL,
    };
    clk_t hal_lptpmclk = {
        .clkId = LPTPM_CLOCK_ROOT,
        .pclkId = kCLOCK_Osc24m,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 24000000UL,
    };
    /* clang-format on */

    SystemPlatformInit();
    BOARD_InitDebugConsolePins();
    BOARD_InitFlexio1PwmPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_ConfigMPU();

    CLOCK_SetRate(&hal_flexioClkCfg);
    CLOCK_EnableClock(hal_flexioClkCfg.clkId);
    CLOCK_SetRate(&hal_lptpmclk);
    CLOCK_EnableClock(hal_lptpmclk.clkId);
}
/*${function:end}*/
