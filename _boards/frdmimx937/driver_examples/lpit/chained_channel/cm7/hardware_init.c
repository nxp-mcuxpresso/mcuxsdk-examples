/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "app.h"
#include "fsl_clock.h"
#include "board.h"
#include "pin_mux.h"
#include "fsl_common.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* clang-format off */

    clk_t lpit_clk = {
        .clkId = LPIT_MASTER_CLOCK_ROOT,
        .pclkId = kCLOCK_syspll1dfs1div2,
        //.enable_clk = true,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
    };

    /* clang-format on */

    SystemPlatformInit();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_ConfigMPU();

    CLOCK_SetParent(&lpit_clk);
    CLOCK_SetRate(&lpit_clk);
    CLOCK_EnableClock(lpit_clk.clkId);
}
/*${function:end}*/
