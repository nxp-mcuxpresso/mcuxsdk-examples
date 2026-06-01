/*
 * Copyright 2024 NXP
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
    clk_t lpspiClkCfg = {
        .clkId = DEMO_LPSPI_CLOCK,
        .pclkId = kCLOCK_Osc24m,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 24000000UL,
    };

    clk_t flexioClkCfg = {
        .clkId = DEMO_FLEXIO_CLOCK,
        .pclkId = kCLOCK_Osc24m,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 24000000UL,
    };

    /* clang-format on */

    SystemPlatformInit();
    BOARD_InitDebugConsolePins();
    BOARD_InitFlexio1SpiPins();
    BOARD_InitLPSPI3Pins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_ConfigMPU();

    CLOCK_SetRate(&flexioClkCfg);
    CLOCK_EnableClock(flexioClkCfg.clkId);
    CLOCK_SetRate(&lpspiClkCfg);
    CLOCK_EnableClock(lpspiClkCfg.clkId);
}
/*${function:end}*/
