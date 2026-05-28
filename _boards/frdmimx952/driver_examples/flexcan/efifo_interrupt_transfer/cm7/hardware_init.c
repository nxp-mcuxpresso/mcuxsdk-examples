/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "app.h"
#include "board.h"
#include "pin_mux.h"
#include "fsl_common.h"
#include "clock_config.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    pca6416a_handle_t handle;
    /* clang-format off */
    clk_t flexcanclk = {
        .clkId = kCLOCK_can1,
        .pclkId = kCLOCK_osc24m,
        .rate = 24000000UL,
        //.enable_clk = true,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
    };
    clk_t lpi2cclk = {
        .clkId = kCLOCK_lpi2c2,
        .pclkId = kCLOCK_osc24m,
        .rate = 24000000UL,
        //.enable_clk = true,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
    };
    /* clang-format on */
    SystemPlatformInit();
    BOARD_InitBootPins();
    BOARD_InitCANPins();
    BOARD_InitI2C2Pins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_ConfigMPU();

    CLOCK_SetRate(&flexcanclk);
    CLOCK_EnableClock(flexcanclk.clkId);
    CLOCK_SetRate(&lpi2cclk);
    CLOCK_EnableClock(lpi2cclk.clkId);

    BOARD_InitPCA6416A(&handle);

    /* CAN1 uses PDM_CLK/PDM_BIT_STREAM0 pins directly - CH_CAN_SEL is for CAN2 only */
    /* Initialize TJA1057BT: set STBY=0 for Normal mode */
    PCA6416A_SetDirection(&handle, (1 << BOARD_PCA6416A_CAN_STBY), kPCA6416A_Output);
    PCA6416A_ClearPins(&handle, (1 << BOARD_PCA6416A_CAN_STBY));
}
/*${function:end}*/
