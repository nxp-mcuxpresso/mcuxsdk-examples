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
#include "sm_platform.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* clang-format off */
    hal_clk_t hal_lpspiclk = {
        .clk_id = EXAMPLE_LPSPI_MASTER_CLOCK_NAME,
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
}

uint32_t LPSPI1_GetFreq(void)
{
    return HAL_ClockGetIpFreq(EXAMPLE_LPSPI_MASTER_CLOCK_NAME);
}

/*${function:end}*/
