/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "app.h"
#include "pin_mux.h"
#include "board.h"
#include "sm_platform.h"
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

    /* clang-format on */
    SM_Platform_Init();
    BOARD_InitDebugConsolePins();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();

    hal_clk.clk_id = (hal_clk_id_e)BOARD_GetUartClkId(BOARD_DEBUG_CONSOLE_PORT);
    HAL_ClockSetRate(&hal_clk);
    HAL_ClockEnable(&hal_clk);
}

/*${function:end}*/
