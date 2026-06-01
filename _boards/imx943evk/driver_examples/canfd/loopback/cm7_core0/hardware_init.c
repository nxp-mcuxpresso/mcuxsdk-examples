/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "app.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    pca6416a_handle_t handle;
    /* clang-format off */
    clk_t flexcanClkCfg = {
        .clkId = FLEXCAN_CLOCK_ROOT,
        .pclkId = kCLOCK_Osc24m,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 24000000UL,
    };
    clk_t lpi2cClkCfg = {
        .clkId = BOARD_PCA6416A_I2C_CLOCK_ROOT,
        .pclkId = kCLOCK_Osc24m,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 24000000UL,
    };

    /* clang-format on */
    SystemPlatformInit();
    BOARD_InitDebugConsolePins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_ConfigMPU();

    CLOCK_SetRate(&flexcanClkCfg);
    CLOCK_EnableClock(flexcanClkCfg.clkId);
    CLOCK_SetRate(&lpi2cClkCfg);
    CLOCK_EnableClock(lpi2cClkCfg.clkId);

    BOARD_InitPCA6416A(&handle);

    PCA6416A_SetDirection(&handle, (1 << BOARD_PCA6416A_CAN3_EN), kPCA6416A_Output);
    PCA6416A_SetDirection(&handle, (1 << BOARD_PCA6416A_CAN3_STBY_B), kPCA6416A_Output);

    /* Initialize TJA1463AT with normal mode */
    /* Clear CAN STBY_N and EN signal of TJA1463AT */
    PCA6416A_ClearPins(&handle, (1 << BOARD_PCA6416A_CAN3_STBY_B) | (1 << BOARD_PCA6416A_CAN3_EN));
    SDK_DelayAtLeastUs(100U, SystemCoreClock);

    /* Setup CAN EN and STBY_N signal of TJA1436AT */
    PCA6416A_SetPins(&handle, (1 << BOARD_PCA6416A_CAN3_STBY_B) | (1 << BOARD_PCA6416A_CAN3_EN));
}
/*${function:end}*/
