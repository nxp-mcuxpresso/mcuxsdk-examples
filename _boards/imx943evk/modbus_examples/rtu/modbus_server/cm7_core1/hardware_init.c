/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "app.h"
#include "board.h"
#include "sm_platform.h"
#include "pin_mux.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* clang-format off */
    hal_clk_t hal_clk = {
        .clk_id = hal_clock_invalid,
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

    hal_clk.clk_id = (hal_clk_id_e)BOARD_GetUartClkId(DEMO_LPUART_INSTANCE_IDX);
    SM_Platform_Init();
    BOARD_InitLpuartPins(DEMO_LPUART_INSTANCE_IDX);
    BOARD_InitDebugConsolePins();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    HAL_ClockSetParent(&hal_clk);
    HAL_ClockSetRate(&hal_clk);
    HAL_ClockEnable(&hal_clk);

    HAL_ClockSetRate(&hal_lptpmclk);
    HAL_ClockEnable(&hal_lptpmclk);

    /* Select UART signals to use */
    BOARD_EXPANDER_SetPinAsOutput(BOARD_PCA6416_I2C6_S3_ID, SPI8_SEL1);
    BOARD_EXPANDER_SetPinAsOutput(BOARD_PCA6416_I2C6_S3_ID, SPI8_SEL3);

    BOARD_EXPANDER_SetPinToHigh(BOARD_PCA6416_I2C6_S3_ID, SPI8_SEL1);
    BOARD_EXPANDER_SetPinToHigh(BOARD_PCA6416_I2C6_S3_ID, SPI8_SEL3);
}
/*${function:end}*/
