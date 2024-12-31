/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "app.h"
#include "board.h"
#include "pin_mux.h"
#include "sm_platform.h"

/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* clang-format off */
    hal_clk_t hal_lpspiMstClkCfg = {
        .clk_id = EXAMPLE_LPSPI_MASTER_CLK_ID,
        .pclk_id = hal_clock_osc24m,
        .clk_round_opt = hal_clk_round_auto,
        .rate = 24000000UL,
    };

    hal_clk_t hal_lpspiSlvClkCfg = {
        .clk_id = EXAMPLE_LPSPI_SLAVE_CLK_ID,
        .pclk_id = hal_clock_osc24m,
        .clk_round_opt = hal_clk_round_auto,
        .rate = 24000000UL,
    };

    hal_clk_t hal_lptpmclk = {
        .clk_id = LPTPM_CLOCK_ROOT,
        .pclk_id = hal_clock_osc24m,
        .clk_round_opt = hal_clk_round_auto,
        .rate = 24000000UL,
    };

    /* clang-format on */

    BOARD_ConfigMPU();
    SM_Platform_Init();
    BOARD_InitDebugConsolePins();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    HAL_ClockSetRate(&hal_lpspiMstClkCfg);
    HAL_ClockEnable(&hal_lpspiMstClkCfg);
    HAL_ClockSetRate(&hal_lpspiSlvClkCfg);
    HAL_ClockEnable(&hal_lpspiSlvClkCfg);
    HAL_ClockSetRate(&hal_lptpmclk);
    HAL_ClockEnable(&hal_lptpmclk);

    BOARD_EXPANDER_SetPinAsOutput(BOARD_PCA6416_I2C6_S3_ID, SPI8_SEL1);
    BOARD_EXPANDER_SetPinAsOutput(BOARD_PCA6416_I2C6_S3_ID, SPI8_SEL3);
    BOARD_EXPANDER_SetPinToLow(BOARD_PCA6416_I2C6_S3_ID, SPI8_SEL1);
    BOARD_EXPANDER_SetPinToLow(BOARD_PCA6416_I2C6_S3_ID, SPI8_SEL3);
}
/*${function:end}*/
