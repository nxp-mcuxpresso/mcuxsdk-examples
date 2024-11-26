/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "app.h"
#include "board.h"
#include "sm_platform.h"
#include "pin_mux.h"
#include "fsl_common.h"
#include "clock_config.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    pca6416a_handle_t handle;
    /* clang-format off */
    hal_clk_t hal_flexcanClkCfg = {
        .clk_id = FLEXCAN_CLOCK_ROOT,
        .pclk_id = hal_clock_osc24m,
        .clk_round_opt = hal_clk_round_auto,
        .rate = 24000000UL,
    };
    hal_clk_t hal_lpi2cClkCfg = {
        .clk_id = BOARD_PCA6416A_I2C_CLOCK_ROOT,
        .pclk_id = hal_clock_osc24m,
        .clk_round_opt = hal_clk_round_auto,
        .rate = 24000000UL,
    };

    /* clang-format on */
    SM_Platform_Init();
    BOARD_InitDebugConsolePins();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    HAL_ClockSetRate(&hal_flexcanClkCfg);
    HAL_ClockEnable(&hal_flexcanClkCfg);
    HAL_ClockSetRate(&hal_lpi2cClkCfg);
    HAL_ClockEnable(&hal_lpi2cClkCfg);

    BOARD_InitPCA6416A(&handle);

    PCA6416A_SetDirection(&handle, (1 << BOARD_PCA6416A_CAN3_EN), kPCA6416A_Output);
    PCA6416A_SetDirection(&handle, (1 << BOARD_PCA6416A_CAN3_STBY_B), kPCA6416A_Output);

    /* Initialize TJA1463AT with normal mode */
    /* Clear CAN STBY_N and EN signal of TJA1463AT */
    PCA6416A_ClearPins(&handle, (1 << BOARD_PCA6416A_CAN3_STBY_B) | (1 << BOARD_PCA6416A_CAN3_EN));
    SDK_DelayAtLeastUs(100U, SystemCoreClock);

    /* Setup CAN EN and STBY_N signal of TJA1436AT */
    PCA6416A_SetPins(&handle, (1 << BOARD_PCA6416A_CAN3_STBY_B) | (1 << BOARD_PCA6416A_CAN3_EN));
}
/*${function:end}*/
