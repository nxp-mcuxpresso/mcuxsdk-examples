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
    pcal6524_handle_t handle;
    /* clang-format off */
    hal_clk_t hal_flexcanclk = {
        .clk_id = hal_clock_can2,
        .pclk_id = hal_clock_osc24m,
        .div = 1,
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
    };
    hal_clk_t hal_lpi2cclk = {
        .clk_id = hal_clock_lpi2c2,
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

    PCAL6524_SetDirection(&handle, (1 << BOARD_PCAL6524_EXP_SEL), kPCAL6524_Output);
    PCAL6524_SetDirection(&handle, (1 << BOARD_PCAL6524_CAN_STBY), kPCAL6524_Output);

    /* Route GPIO_IO25,GPIO_IO27 to CAN2 */
    PCAL6524_ClearPins(&handle, (1 << BOARD_PCAL6524_EXP_SEL));

    /* Clear CAN CAN_STBY signal */
    PCAL6524_ClearPins(&handle, (1 << BOARD_PCAL6524_CAN_STBY));
}
/*${function:end}*/
