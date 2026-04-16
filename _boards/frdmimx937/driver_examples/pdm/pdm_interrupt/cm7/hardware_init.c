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
    clk_t audiopll1vcoCLKCfg = {
        .clkId = kCLOCK_audiopll1ctl,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 3932160000,
    };
    clk_t audiopll1CLKCfg = {
        .clkId = kCLOCK_audiopll1,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 393216000,
    };
    clk_t pdmClkCfg = {
        .clkId = kCLOCK_pdm,
        .pclkId = kCLOCK_audiopll1,
        .rate = 196608000UL,
        //.enable_clk = true,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
    };
    /* clang-format on */
    BOARD_InitBootPins();
    BOARD_InitPDMPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    CLOCK_SetRate(&audiopll1vcoCLKCfg);
    CLOCK_EnableClock(audiopll1vcoCLKCfg.clkId);
    CLOCK_SetRate(&audiopll1CLKCfg);
    CLOCK_EnableClock(audiopll1CLKCfg.clkId);
    CLOCK_SetRate(&pdmClkCfg);
    CLOCK_EnableClock(pdmClkCfg.clkId);
    BOARD_ConfigMPU();
}
/*${function:end}*/
