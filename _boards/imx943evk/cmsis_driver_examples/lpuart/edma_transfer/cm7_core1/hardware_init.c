/*
 * Copyright 2024 NXP
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

    /* clang-format on */
    SystemPlatformInit();
    BOARD_InitDebugConsolePins();
    BOARD_BootClockRUN();
    BOARD_ConfigMPU();

    clk.clkId = BOARD_GetUartClkId(BOARD_DEBUG_CONSOLE_PORT);
    CLOCK_SetParent(&clk);
    CLOCK_SetRate(&clk);
    CLOCK_EnableClock(clk.clkId);
}
/*${function:end}*/
