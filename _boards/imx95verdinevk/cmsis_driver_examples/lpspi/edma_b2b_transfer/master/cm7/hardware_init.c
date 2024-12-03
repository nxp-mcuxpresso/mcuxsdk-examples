/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "app.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_common.h"
#include "sm_platform.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* clang-format off */
    hal_clk_t hal_lpspiClkCfg = {
        .clk_id = hal_clock_lpspi1,
        .pclk_id = hal_clock_osc24m,
        .div = 1,
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
    };

    /* 250MHz DMA clock */
    /* DMA clocked by M33 clk, configurated by SM */
    //const clock_root_config_t dmaClkCfg = {
    //    .clockOff = false,
    //    .mux = kCLOCK_WAKEUPAXI_ClockRoot_MuxSysPll1Pfd0, // 1000MHz
    //    .div = 4
    //};

    /* clang-format on */
    SM_Platform_Init();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    HAL_ClockSetRootClk(&hal_lpspiClkCfg);
}

uint32_t LPSPI1_GetFreq(void)
{
    return HAL_ClockGetIpFreq(hal_clock_lpspi1);
}

/*${function:end}*/
