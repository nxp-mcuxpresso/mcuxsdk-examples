/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "app.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_common.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* clang-format off */
    clk_t lpspiClkCfg = {
        .clkId = kCLOCK_lpspi7,
        .pclkId = kCLOCK_osc24m,
        .rate = 24000000UL,
        //.enable_clk = true,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
    };

    /* 250MHz DMA clock */
    /* DMA clocked by M33 clk, configurated by SM */
    //const clock_root_config_t dmaClkCfg = {
    //    .clockOff = false,
    //    .mux = kCLOCK_WAKEUPAXI_ClockRoot_MuxSysPll1Pfd0, // 1000MHz
    //    .div = 4
    //};

    /* clang-format on */
    SystemPlatformInit();
    BOARD_InitBootPins();
    BOARD_InitLPSPIPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_SetRate(&lpspiClkCfg);
    CLOCK_EnableClock(lpspiClkCfg.clkId);

}

uint32_t LPSPI7_GetFreq(void)
{
    return CLOCK_GetRate(kCLOCK_lpspi7);
}

/*${function:end}*/
