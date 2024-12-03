/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "board.h"
#include "pin_mux.h"
#include "app.h"
#include "clock_config.h"
#include "fsl_iomuxc.h"
#include "fsl_codec_common.h"
#include "fsl_wm8960.h"
#include "fsl_codec_adapter.h"
#include "sm_platform.h"
/*${header:end}*/

/*${variable:start}*/
wm8960_config_t wm8960Config = {
    .i2cConfig        = {.codecI2CInstance = BOARD_CODEC_I2C_INSTANCE},
    .route            = kWM8960_RoutePlaybackandRecord,
    .rightInputSource = kWM8960_InputDifferentialMicInput2,
    .playSource       = kWM8960_PlaySourceDAC,
    .slaveAddress     = WM8960_I2C_ADDR,
    .bus              = kWM8960_BusI2S,
    .format           = {.sampleRate = kWM8960_AudioSampleRate16KHz, .bitWidth = kWM8960_AudioBitWidth16bit},
    .master_slave     = false,
};
codec_config_t boardCodecConfig = {.codecDevType = kCODEC_WM8960, .codecDevConfig = &wm8960Config};
/*${variable:end}*/

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
    hal_clk_t hal_lpi2cCLKCfg = {
        .clk_id = hal_clock_lpi2c4,
        .pclk_id = hal_clock_osc24m,
        .div = 1,
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
    };
    hal_clk_t hal_saiCLKCfg = {
        .clk_id = hal_clock_sai3,
        .pclk_id = hal_clock_audiopll1, // select audiopll1out source(393216000 Hz)
        .div = 32, // output 12288000 Hz
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
    };
    hal_clk_t hal_pdmClkCfg = {
        .clk_id = hal_clock_pdm,
        .pclk_id = hal_clock_audiopll1,
        .div = 2,
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
    };
    /* clang-format on */

    SM_Platform_Init();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    HAL_ClockSetPllClk(&hal_audiopll1vcoCLKCfg);
    HAL_ClockSetPllClk(&hal_audiopll1CLKCfg);
    HAL_ClockSetRootClk(&hal_pdmClkCfg);
    HAL_ClockSetRootClk(&hal_lpi2cCLKCfg);
    HAL_ClockSetRootClk(&hal_saiCLKCfg);

    /* Select PDM/SAI signals */
    pcal6408_handle_t handle;
    BOARD_InitPCAL6408_I2C4(&handle);
    PCAL6408_SetDirection(&handle, (1 << BOARD_PCAL6408_SLOT_SAI3_SEL), kPCAL6408_Output);
    PCAL6408_ClearPins(&handle, (1 << BOARD_PCAL6408_SLOT_SAI3_SEL));
    SDK_DelayAtLeastUs(10000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    wm8960Config.i2cConfig.codecI2CSourceClock = DEMO_I2C_CLK_FREQ;
    wm8960Config.format.mclk_HZ                = DEMO_SAI_CLK_FREQ;
}
/*${function:end}*/
