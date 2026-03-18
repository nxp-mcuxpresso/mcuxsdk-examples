/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "app.h"
#include "board.h"
#include "pin_mux.h"
#include "fsl_common.h"
#include "clock_config.h"
#include "sm_platform.h"
#include "hal_clock.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    pcal6524_handle_t handle;

    /* clang-format off */
    hal_clk_t hal_lpspiclk = {
        .clk_id = LPSPI_SLAVE_CLOCK_ROOT,
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

    HAL_ClockSetRootClk(&hal_lpspiclk);
    HAL_ClockSetRootClk(&hal_lpi2cclk);

    BOARD_InitPCAL6524(&handle);

    /* Select SPI pins */
    PCAL6524_SetDirection(&handle, (1 << BOARD_PCAL6524_CH_SPI_SEL), kPCAL6524_Output);
    PCAL6524_SetPins(&handle, (1 << BOARD_PCAL6524_CH_SPI_SEL));

    SDK_DelayAtLeastUs(100U, SystemCoreClock);
}
/*${function:end}*/
