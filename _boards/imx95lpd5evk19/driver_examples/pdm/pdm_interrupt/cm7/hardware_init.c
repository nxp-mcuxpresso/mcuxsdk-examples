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
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* clang-format off */

    hal_clk_t hal_audiopll1vcoCLKCfg = {
        .clk_id = hal_clock_audiopll1ctl,
        .clk_round_opt = hal_clk_round_auto,
        .ratel = 3932160000,
        .rateu = 0,
    };
    hal_clk_t hal_audiopll1CLKCfg = {
        .clk_id = hal_clock_audiopll1,
        .clk_round_opt = hal_clk_round_auto,
        .ratel = 393216000,
        .rateu = 0,
    };
    hal_clk_t hal_pdmClkCfg = {
        .clk_id = hal_clock_pdm,
        .pclk_id = hal_clock_audiopll1,
        .div = 2,
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
    };

    SM_Platform_Init();
    /* clang-format on */
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    HAL_ClockSetPllClk(&hal_audiopll1vcoCLKCfg);
    HAL_ClockSetPllClk(&hal_audiopll1CLKCfg);
    HAL_ClockSetRootClk(&hal_pdmClkCfg);
}
/*${function:end}*/
