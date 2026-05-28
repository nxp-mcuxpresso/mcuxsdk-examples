/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "app.h"
#include "board.h"
#include "pin_mux.h"
#include "fsl_common.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    clk_t lpit_clk = {
        .clkId = LPIT_MASTER_CLOCK_ROOT,
	.pclkId = kCLOCK_syspll1dfs1div2,
	.rate = 133333333UL, /* 400Mhz/3 = 133.33Mhz */
	//.enable_clk = true,
	.clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
    };

    clk_t adc_clk = {
        .clkId = kCLOCK_adc,
        .pclkId = kCLOCK_syspll1dfs1div2,
        .rate = 80000000UL, /* 80MHz */
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
    };

    SystemPlatformInit();
    BOARD_InitBootPins();
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
