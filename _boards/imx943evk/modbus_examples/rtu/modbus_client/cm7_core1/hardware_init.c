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
    clk_t clk = {
        .clkId = kCLOCK_IpInvalid,
        .pclkId = kCLOCK_Osc24m,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 24000000UL,
    };

    clk_t lptpm6clk = {
        .clkId = LPTPM6_CLOCK_ROOT,
        .pclkId = kCLOCK_Osc24m,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 24000000UL,
    };

    clk_t lptpm5clk = {
        .clkId = LPTPM5_CLOCK_ROOT,
        .pclkId = kCLOCK_Osc24m,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 24000000UL,
    };
    /* clang-format on */

    clk.clkId = BOARD_GetUartClkId(DEMO_LPUART_INSTANCE_IDX);
    SystemPlatformInit();
    BOARD_InitLpuartPins(DEMO_LPUART_INSTANCE_IDX);
    BOARD_InitDebugConsolePins();
    BOARD_InitGPIOPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_ConfigMPU();
    CLOCK_SetParent(&clk);
    CLOCK_SetRate(&clk);
    CLOCK_EnableClock(clk.clkId);

    CLOCK_SetRate(&lptpm6clk);
    CLOCK_EnableClock(lptpm6clk.clkId);
    CLOCK_SetRate(&lptpm5clk);
    CLOCK_EnableClock(lptpm5clk.clkId);

}
/*${function:end}*/
