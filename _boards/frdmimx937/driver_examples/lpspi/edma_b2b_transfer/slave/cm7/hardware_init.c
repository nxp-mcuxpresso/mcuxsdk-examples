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
    pca6416a_handle_t handle;

    /* clang-format off */
    clk_t lpspiclk = {
        .clkId = LPSPI_SLAVE_CLOCK_ROOT,
        .pclkId = kCLOCK_osc24m,
        .rate = 24000000UL,
        //.enable_clk = true,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
    };
    clk_t lpi2cclk = {
        .clkId = kCLOCK_lpi2c2,
        .pclkId = kCLOCK_osc24m,
        .rate = 24000000UL,
        //.enable_clk = true,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
    };

    /* clang-format on */
    SystemPlatformInit();
    BOARD_InitBootPins();
    BOARD_InitLPSPIPins();
    BOARD_InitI2C2Pins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_SetParent(&lpspiclk);
    CLOCK_SetRate(&lpspiclk);
    CLOCK_EnableClock(lpspiclk.clkId);
    CLOCK_SetParent(&lpi2cclk);
    CLOCK_SetRate(&lpi2cclk);
    CLOCK_EnableClock(lpi2cclk.clkId);

    BOARD_InitPCA6416A(&handle);

    /* Select SPI pins */
    PCA6416A_SetDirection(&handle, (1 << BOARD_PCA6416A_CH_SPI_SEL), kPCA6416A_Output);
    PCA6416A_SetPins(&handle, (1 << BOARD_PCA6416A_CH_SPI_SEL));

    SDK_DelayAtLeastUs(100U, SystemCoreClock);
}
/*${function:end}*/
