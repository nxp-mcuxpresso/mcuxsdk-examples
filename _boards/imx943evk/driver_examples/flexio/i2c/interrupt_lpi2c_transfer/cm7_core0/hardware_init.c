/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "board.h"
#include "pin_mux.h"
#include "app.h"
#include "sm_platform.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* clang-format off */
    clk_t lpi2cclk = {
        .clkId = DEMO_LPI2C_CLOCK,
        .pclkId = kCLOCK_Osc24m,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 24000000UL,
    };
    
    clk_t flexioclk = {
        .clkId = DEMO_FLEXIO_CLOCK,
        .pclkId = kCLOCK_Osc24m,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 24000000UL,
    };
    /* clang-format on */

    SM_Platform_Init();
    BOARD_InitDebugConsolePins();
    BOARD_BootClockRUN();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_SetRate(&lpi2cclk);
    CLOCK_EnableClock(lpi2cclk.clkId);
    CLOCK_SetRate(&flexioclk);
    CLOCK_EnableClock(flexioclk.clkId);
}
/*${function:end}*/
