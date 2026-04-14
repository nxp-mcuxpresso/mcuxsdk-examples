/*
 * Copyright 2024 NXP
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
    clk_t lpspiMstClkCfg = {
        .clkId = EXAMPLE_LPSPI_MASTER_CLK_ID,
        .pclkId = kCLOCK_osc24m,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 24000000UL,
    };

    clk_t lpspiSlvClkCfg = {
        .clkId = EXAMPLE_LPSPI_SLAVE_CLK_ID,
        .pclkId = kCLOCK_osc24m,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 24000000UL,
    };
    /* clang-format on */

    SystemPlatformInit();
    BOARD_InitBootPins();
    BOARD_InitLPSPIPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_SetRate(&lpspiMstClkCfg);
    CLOCK_EnableClock(lpspiMstClkCfg.clkId);
    CLOCK_SetRate(&lpspiSlvClkCfg);
    CLOCK_EnableClock(lpspiSlvClkCfg.clkId);
}
/*${function:end}*/
