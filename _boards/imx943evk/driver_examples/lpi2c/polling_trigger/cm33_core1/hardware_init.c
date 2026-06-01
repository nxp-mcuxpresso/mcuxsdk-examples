/*
 * Copyright 2025 NXP
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
    clk_t hal_lpi2cMstclk = {
        .clkId = LPI2C_MASTER_CLOCK_ROOT,
        .pclkId = kCLOCK_Osc24m,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 24000000UL,
    };

    clk_t hal_lpi2cSlvclk = {
        .clkId = LPI2C_SLAVE_CLOCK_ROOT,
        .pclkId = kCLOCK_Osc24m,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 24000000UL,
    };

    clk_t hal_lptpmclk = {
        .clkId = LPTPM_CLOCK_ROOT,
        .pclkId = kCLOCK_Osc24m,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 24000000UL,
    };
    /* clang-format on */

    SystemPlatformInit();
    BOARD_InitDebugConsolePins();
    BOARD_InitI2C4Pins();
    BOARD_InitI2C6Pins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_ConfigMPU();

    CLOCK_SetRate(&hal_lpi2cMstclk);
    CLOCK_EnableClock(hal_lpi2cMstclk.clkId);
    CLOCK_SetRate(&hal_lpi2cSlvclk);
    CLOCK_EnableClock(hal_lpi2cSlvclk.clkId);
    CLOCK_SetRate(&hal_lptpmclk);
    CLOCK_EnableClock(hal_lptpmclk.clkId);

    BOARD_MUX_Select(BOARD_PCA9544_I2C6_ID, BOARD_S0_CHAN_IDX);
}
/*${function:end}*/
