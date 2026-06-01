/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "app.h"
#include "board.h"
#include "pin_mux.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* clang-format off */
    clk_t hal_clk = {
        .clkId = kCLOCK_IpInvalid,
        .pclkId = kCLOCK_Osc24m,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 24000000UL,
    };
    /* clang-format on */
    clk_t hal_lpit_clk = {
        .clkId = LPIT_MASTER_CLOCK_ROOT,
        .pclkId = kCLOCK_Osc24m,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 24000000UL,
    };

    hal_clk.clkId = BOARD_GetUartClkId(DEMO_LPUART_INSTANCE_IDX);
    SystemPlatformInit();
    BOARD_InitLpuartPins(DEMO_LPUART_INSTANCE_IDX);
    BOARD_BootClockRUN();
    BOARD_ConfigMPU();
    CLOCK_SetRate(&hal_clk);
    CLOCK_EnableClock(hal_clk.clkId);
    CLOCK_SetRate(&hal_lpit_clk);
    CLOCK_EnableClock(hal_lpit_clk.clkId);
}
/*${function:end}*/
