/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "board.h"
#include "fsl_clock.h"
#include "pin_mux.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* clang-format off */
    clk_t lpi2cClkCfg = {
        .clkId = kCLOCK_lpi2c1,
        .pclkId = kCLOCK_osc24m,
        .rate = 24000000UL,
        //.enable_clk = true,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
    };
    /* clang-format on */

    SystemPlatformInit();
    BOARD_InitBootPins();
    BOARD_InitI2C6Pins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_ConfigMPU();

    CLOCK_SetParent(&lpi2cClkCfg);
    CLOCK_SetRate(&lpi2cClkCfg);
    CLOCK_EnableClock(lpi2cClkCfg.clkId);
}
/*${function:end}*/
