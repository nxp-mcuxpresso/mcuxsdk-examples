/*
 * Copyright 2023 NXP
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* clang-format off */
    /* 250MHz DMA clock */
    clk_t dmaClkCfg = {
        .clkId = kCLOCK_Wakeupaxi,
	.pclkId = kCLOCK_Syspll1dfs0,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 24000000UL,
    };
    /* clang-format on */

    SystemPlatformInit();
    BOARD_InitDebugConsolePins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_ConfigMPU();

    CLOCK_SetRate(&dmaClkCfg);
    CLOCK_EnableClock(dmaClkCfg.clkId);
}
/*${function:end}*/
