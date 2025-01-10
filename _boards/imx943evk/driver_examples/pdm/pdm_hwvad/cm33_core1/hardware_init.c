/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "app.h"
#include "board.h"
#include "pin_mux.h"
#include "sm_platform.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* clang-format off */
    hal_clk_t hal_audiopll1vcoCLKCfg = {
        .clk_id = hal_clock_audiopll1ctl,
        .clk_round_opt = hal_clk_round_auto,
        .ratel = 3932160000,
        .rateu = 0,
    };
    hal_clk_t hal_audiopll1CLKCfg = {
        .clk_id = hal_clock_audiopll1,
        .clk_round_opt = hal_clk_round_auto,
        .ratel = 393216000,
        .rateu = 0,
    };
    hal_clk_t hal_pdmClkCfg = {
        .clk_id = PDM_CLOCK_ROOT,
        .pclk_id = hal_clock_audiopll1,
        .div = 2,
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
    };
/*    hal_clk_t hal_pdmClkCfg = {
        .clk_id = PDM_CLOCK_ROOT,
        .pclk_id = hal_clock_audiopll1,
        .clk_round_opt = hal_clk_round_auto,
        .rate = 24000000UL,
    };*/

    /* clang-format on */
    SM_Platform_Init();
    BOARD_InitDebugConsolePins();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    HAL_ClockSetPllClk(&hal_audiopll1vcoCLKCfg);
    HAL_ClockEnable(&hal_audiopll1vcoCLKCfg);
    HAL_ClockSetPllClk(&hal_audiopll1CLKCfg);
    HAL_ClockEnable(&hal_audiopll1CLKCfg);
    HAL_ClockSetRootClk(&hal_pdmClkCfg);
    //HAL_ClockSetRate(&hal_pdmClkCfg);
    HAL_ClockEnable(&hal_pdmClkCfg);
    BOARD_EXPANDER_SetPinAsOutput(BOARD_PCA6416_I2C6_S3_ID, CAN_PDM_SEL);
    BOARD_EXPANDER_SetPinToLow(BOARD_PCA6416_I2C6_S3_ID, CAN_PDM_SEL);
    BOARD_EXPANDER_SetPinAsOutput(BOARD_PCA6416_I2C3_S5_21_ID, MQS_MIC_SEL);
    BOARD_EXPANDER_SetPinToLow(BOARD_PCA6416_I2C3_S5_21_ID, MQS_MIC_SEL);
}
/*${function:end}*/
