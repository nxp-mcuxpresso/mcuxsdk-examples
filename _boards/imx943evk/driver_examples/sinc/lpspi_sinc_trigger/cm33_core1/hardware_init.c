/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "app.h"
#include "pin_mux.h"
#include "board.h"
/*${header:end}*/

/*${variable:start}*/

/*${variable:end}*/
/*${function:start}*/
void BOARD_InitHardware(void)
{
    clk_t hal_lptpmclk = {
        .clkId = LPTPM_CLOCK_ROOT,
        .pclkId = kCLOCK_Osc24m,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 24000000UL,
    };
    SystemPlatformInit();
    BOARD_InitDebugConsolePins();
    BOARD_InitLPSPI3Pins();

    BOARD_InitSinc2Pins();

    BOARD_InitI2C6Pins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_ConfigMPU();

    CLOCK_SetRate(&hal_lptpmclk);
    CLOCK_EnableClock(hal_lptpmclk.clkId);

    BOARD_EXPANDER_SetPinAsOutput(BOARD_PCA6416_I2C6_S3_ID, SD2_SEL);
    BOARD_EXPANDER_SetPinToLow(BOARD_PCA6416_I2C6_S3_ID, SD2_SEL);
}
/*${function:end}*/
