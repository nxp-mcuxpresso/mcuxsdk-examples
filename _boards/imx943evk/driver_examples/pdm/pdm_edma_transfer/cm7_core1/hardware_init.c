/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "app.h"
#include "board.h"
#include "pin_mux.h"
#include "fsl_codec_common.h"
#include "fsl_wm8962.h"
#include "fsl_codec_adapter.h"
#include "sm_platform.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* clang-format off */

    hal_clk_t hal_pdmClkCfg = {
        .clk_id = PDM_CLOCK_ROOT,
        .pclk_id = hal_clock_audiopll1,
        .clk_round_opt = hal_clk_round_auto,
        .rate = 24000000UL,
    };
    /* clang-format on */
    SM_Platform_Init();
    BOARD_InitDebugConsolePins();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    HAL_ClockSetRate(&hal_pdmClkCfg);
    HAL_ClockEnable(&hal_pdmClkCfg);
}
/*${function:end}*/
