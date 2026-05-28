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
#include "clock_config.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* clang-format off */
    clk_t flexcanClkCfg = {
        .clkId = kCLOCK_can1,
        .pclkId = kCLOCK_osc24m,
        .rate = 24000000UL,
        //.enable_clk = true,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
    };
    /* clang-format on */
    SystemPlatformInit();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_ConfigMPU();

    CLOCK_SetRate(&flexcanClkCfg);
    CLOCK_EnableClock(flexcanClkCfg.clkId);
}
/*${function:end}*/
