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
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* clang-format off */
    clk_t flexioClkCfg = {
        .clkId = DEMO_FLEXIO_CLOCK,
        .pclkId = kCLOCK_Osc24m,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 24000000UL,
    };
    SystemPlatformInit();
    BOARD_InitFlexio1UartPins();
    BOARD_BootClockRUN();

    CLOCK_SetRate(&flexioClkCfg);
    CLOCK_EnableClock(flexioClkCfg.clkId);
}
/*${function:end}*/
