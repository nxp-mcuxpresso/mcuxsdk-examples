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
    /* Initialize board hardware. */
    pcal6524_handle_t handle;
    /* clang-format off */
    hal_clk_t hal_flexcanclk = {
        .clk_id = hal_clock_can1,
        .pclk_id = hal_clock_osc24m,
        .div = 1,
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
    };

    hal_clk_t hal_lpi2cclk = {
        .clk_id = hal_clock_lpi2c7,
        .pclk_id = hal_clock_osc24m,
        .div = 1,
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
    };
    /* clang-format on */
    SM_Platform_Init();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    HAL_ClockSetRootClk(&hal_flexcanclk);
    HAL_ClockSetRootClk(&hal_lpi2cclk);

    BOARD_InitPCAL6524(&handle);

    PCAL6524_SetDirection(&handle, (1 << BOARD_PCAL6524_CAN1_EN), kPCAL6524_Output);
    PCAL6524_SetDirection(&handle, (1 << BOARD_PCAL6524_CAN1_STBY_B), kPCAL6524_Output);


    /* Initialize TJA1463AT with normal mode */
    /* Clear CAN STBY_N and EN signal of TJA1463AT */
    PCAL6524_ClearPins(&handle, (1 << BOARD_PCAL6524_CAN1_STBY_B) | (1 << BOARD_PCAL6524_CAN1_EN));
    SDK_DelayAtLeastUs(100U, SystemCoreClock);

    /* Setup CAN EN and STBY_N signal of TJA1436AT */
    PCAL6524_SetPins(&handle, (1 << BOARD_PCAL6524_CAN1_STBY_B) | (1 << BOARD_PCAL6524_CAN1_EN));
}
/*${function:end}*/
