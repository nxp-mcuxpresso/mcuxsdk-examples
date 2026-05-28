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
    pca6416a_handle_t handle;
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

    clk_t lpi2cclk = {
        .clkId = kCLOCK_lpi2c6,
        .pclkId = kCLOCK_osc24m,
        .rate = 24000000UL,
        //.enable_clk = true,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
    };

    /* clang-format on */
    SystemPlatformInit();
    BOARD_InitBootPins();
    BOARD_InitGPIO2Pins();
    BOARD_InitI2C6Pins();
    BOARD_InitLPSPIPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_SetRate(&lpspiClkCfg);
    CLOCK_EnableClock(lpspiClkCfg.clkId);

    CLOCK_SetRate(&lpi2cclk);
    CLOCK_EnableClock(lpi2cclk.clkId);

    BOARD_InitPCA6416A(&handle);

    PCA6416A_SetDirection(&handle, (1 << BOARD_PCA6416A_ENET2_SAI2_SEL), kPCA6416A_Output);

    /* Select ENET2_SAI2_SEL etc */
    PCA6416A_ClearPins(&handle, (1 << BOARD_PCA6416A_ENET2_SAI2_SEL));
}

uint32_t LPSPI7_GetFreq(void)
{
    return CLOCK_GetRate(kCLOCK_lpspi7);
}

/*${function:end}*/
