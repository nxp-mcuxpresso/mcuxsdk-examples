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
    hal_clk_t hal_lpi2cClkCfg = {
        .clk_id = hal_clock_lpi2c6,
        .pclk_id = hal_clock_osc24m,
        .div = 1,
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
    };
    /* DMA clocked by M33 clk, configurated by SM */
    ///* 250MHz DMA clock */
    //const clock_root_config_t dmaClkCfg = {
    //    .clockOff = false,
    //    .mux = kCLOCK_WAKEUPAXI_ClockRoot_MuxSysPll1Dfs0, // 4000MHz
    //    .div = 16
    //};
    /* clang-format on */
    SM_Platform_Init();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    HAL_ClockSetRootClk(&hal_lpi2cClkCfg);
    //CLOCK_SetRootClock(kCLOCK_Root_WakeupAxi, &dmaClkCfg);
}

uint32_t LPI2C6_GetFreq(void)
{
    return HAL_ClockGetIpFreq(hal_clock_lpi2c6);
}

/*${function:end}*/
