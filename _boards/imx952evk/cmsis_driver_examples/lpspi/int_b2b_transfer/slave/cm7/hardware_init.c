/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "app.h"
#include "pin_mux.h"
#include "fsl_common.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_common.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* clang-format off */
    pca6416a_handle_t handle;

    clk_t lpspiclk = {
        .clkId = EXAMPLE_LPSPI_SLAVE_CLOCK_NAME,
        .pclkId = kCLOCK_osc24m,
        .rate = 24000000UL,
        //.enable_clk = true,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
    };

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
    BOARD_ConfigMPU();

    CLOCK_SetRate(&lpspiclk);
    CLOCK_EnableClock(lpspiclk.clkId);
    CLOCK_SetRate(&lpi2cclk);
    CLOCK_EnableClock(lpi2cclk.clkId);

    BOARD_InitPCA6416A(&handle);

    PCA6416A_SetDirection(&handle, (1 << BOARD_PCA6416A_ENET2_SAI2_SEL), kPCA6416A_Output);

    /* Select ENET2_SAI2_SEL etc */
    PCA6416A_ClearPins(&handle, (1 << BOARD_PCA6416A_ENET2_SAI2_SEL));
}

uint32_t LPSPI7_GetFreq(void)
{
    return CLOCK_GetRate(EXAMPLE_LPSPI_SLAVE_CLOCK_NAME);
}

/*${function:end}*/
