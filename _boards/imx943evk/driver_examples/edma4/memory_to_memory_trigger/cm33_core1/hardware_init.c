/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "sm_platform.h"
#include "hal_clock.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "fsl_debug_console.h"
#include "fsl_edma.h"
/*${header:end}*/

/*${function:start}*/

void BOARD_InitHardware(void)
{
    hal_clk_t hal_lptpmclk = {
        .clk_id = LPTPM_CLOCK_ROOT,
        .pclk_id = hal_clock_osc24m,
        .clk_round_opt = hal_clk_round_auto,
        .rate = 24000000UL,
    };
    SM_Platform_Init();
    BOARD_InitDebugConsolePins();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    HAL_ClockSetRate(&hal_lptpmclk);
    HAL_ClockEnable(&hal_lptpmclk);
}
/*${function:end}*/
