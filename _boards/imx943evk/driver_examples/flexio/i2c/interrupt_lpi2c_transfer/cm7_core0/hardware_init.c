/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "board.h"
#include "pin_mux.h"
#include "app.h"
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

    SystemPlatformInit();
    BOARD_InitDebugConsolePins();
    BOARD_InitFlexio1UartPins();
    BOARD_BootClockRUN();
    BOARD_InitI2C4Pins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_ConfigMPU();

    CLOCK_SetRate(&lpi2cclk);
    CLOCK_EnableClock(lpi2cclk.clkId);
    CLOCK_SetRate(&flexioclk);
    CLOCK_EnableClock(flexioclk.clkId);
}
/*${function:end}*/
