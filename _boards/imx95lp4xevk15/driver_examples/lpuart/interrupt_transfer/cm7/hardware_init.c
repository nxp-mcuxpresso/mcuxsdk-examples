/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "board.h"
#include "sm_platform.h"
#include "pin_mux.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* clang-format off */
    hal_clk_t hal_clk = {
        .clk_id = BOARD_DEBUG_UART_CLOCK_ROOT_FOR_CM7_WITH_SM,
        .pclk_id = hal_clock_osc24m,
        .div = 1,
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
    };
    /* clang-format on */

    SM_Platform_Init();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    HAL_ClockSetRootClk(&hal_clk);
}
/*${function:end}*/
