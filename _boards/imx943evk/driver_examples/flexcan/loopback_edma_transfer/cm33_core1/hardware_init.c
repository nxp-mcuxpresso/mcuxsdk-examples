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
    clk_t flexcanClkCfg = {
        .clkId = FLEXCAN_CLOCK_ROOT,
        .pclkId = kCLOCK_Osc24m,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 24000000UL,
    };
    /* clang-format on */

    SystemPlatformInit();
    BOARD_InitDebugConsolePins();
    BOARD_InitCAN1Pins();

    BOARD_InitI2C6Pins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_ConfigMPU();

    CLOCK_SetParent(&flexcanClkCfg);
    CLOCK_SetRate(&flexcanClkCfg);
    CLOCK_EnableClock(flexcanClkCfg.clkId);

    BOARD_EXPANDER_SetPinAsOutput(BOARD_PCA6416_I2C6_S1_ID, CAN1_ENABLE);
    BOARD_EXPANDER_SetPinAsOutput(BOARD_PCA6416_I2C6_S1_ID, CAN1_STBY_B);

    /* Initialize TJA1463AT with normal mode */
    /* Clear CAN STBY_N and EN signal of TJA1463AT */
    BOARD_EXPANDER_SetPinToLow(BOARD_PCA6416_I2C6_S1_ID, CAN1_ENABLE);
    BOARD_EXPANDER_SetPinToLow(BOARD_PCA6416_I2C6_S1_ID, CAN1_STBY_B);
    SDK_DelayAtLeastUs(100U, SystemCoreClock);

    /* Setup CAN EN and STBY_N signal of TJA1436AT */
    BOARD_EXPANDER_SetPinToHigh(BOARD_PCA6416_I2C6_S1_ID, CAN1_ENABLE);
    BOARD_EXPANDER_SetPinToHigh(BOARD_PCA6416_I2C6_S1_ID, CAN1_STBY_B);
}
/*${function:end}*/
