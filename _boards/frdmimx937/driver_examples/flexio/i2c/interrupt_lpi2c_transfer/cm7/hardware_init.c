/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "board.h"
#include "pin_mux.h"
#include "app.h"
#include "clock_config.h"
#include "fsl_iomuxc.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* clang-format off */
    clk_t lpi2cclk = {
        .clkId = kCLOCK_lpi2c6,
        .pclkId = kCLOCK_osc24m,
        .rate = 24000000UL,
        //.enable_clk = true,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
    };
    
    clk_t flexioclk = {
        .clkId = kCLOCK_flexio1,
        .pclkId = kCLOCK_osc24m,
        .rate = 24000000UL,
        //.enable_clk = true,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
    };    
    /* clang-format on */
    SystemPlatformInit();
    BOARD_InitBootPins();
    BOARD_InitFLEXIOPins();
    BOARD_InitI2C6Pins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_ConfigMPU();

    CLOCK_SetParent(&lpi2cclk);
    CLOCK_SetRate(&lpi2cclk);
    CLOCK_EnableClock(lpi2cclk.clkId);
    CLOCK_SetParent(&flexioclk);
    CLOCK_SetRate(&flexioclk);
    CLOCK_EnableClock(flexioclk.clkId);
}
/*${function:end}*/
