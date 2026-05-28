/*
 * Copyright 2026 NXP
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
#include "fsl_wm8962.h"
#include "fsl_codec_adapter.h"
#include "fsl_sai.h"
/*${header:end}*/

/*${variable:start}*/
wm8962_config_t wm8962Config = {
    .i2cConfig = {.codecI2CInstance = BOARD_CODEC_I2C_INSTANCE},
    .route =
        {
            .enableLoopBack            = false,
            .leftInputPGASource        = kWM8962_InputPGASourceInput1,
            .leftInputMixerSource      = kWM8962_InputMixerSourceInputPGA,
            .rightInputPGASource       = kWM8962_InputPGASourceInput3,
            .rightInputMixerSource     = kWM8962_InputMixerSourceInputPGA,
            .leftHeadphoneMixerSource  = kWM8962_OutputMixerDisabled,
            .leftHeadphonePGASource    = kWM8962_OutputPGASourceDAC,
            .rightHeadphoneMixerSource = kWM8962_OutputMixerDisabled,
            .rightHeadphonePGASource   = kWM8962_OutputPGASourceDAC,
        },
    .slaveAddress = WM8962_I2C_ADDR,
    .bus          = kWM8962_BusI2S,
    .format       = {.sampleRate = kWM8962_AudioSampleRate16KHz, .bitWidth = kWM8962_AudioBitWidth16bit},
    .fllClock =
        {
            .fllClockSource        = kWM8962_FLLClkSourceMCLK,
            .fllReferenceClockFreq = 12288000U,
            .fllOutputFreq         = 12288000U,
        },
    .sysclkSource = kWM8962_SysClkSourceMclk, /* use MCLK pin as sysclk's source */
    .masterSlave  = false,                    /* sai running as master mode, so codec running as slave mode */
};

codec_config_t boardCodecConfig = {.codecDevType = kCODEC_WM8962, .codecDevConfig = &wm8962Config};
/*${variable:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* clang-format off */
    clk_t audiopll1vcoCLKCfg = {
        .clkId = kCLOCK_audiopll1ctl,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 3932160000,
    };
    clk_t audiopll1CLKCfg = {
        .clkId = kCLOCK_audiopll1,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 393216000,
    };
    clk_t lpi2cCLKCfg = {
        .clkId = kCLOCK_lpi2c4,
        .pclkId = kCLOCK_osc24m,
        .rate = 24000000UL,
        //.enable_clk = true,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
    };

    clk_t saiCLKCfg = {
        .clkId = kCLOCK_sai3,
        .pclkId = kCLOCK_audiopll1, // select audiopll1out source(393216000 Hz)
        .rate = 12288000UL, // output 12288000 Hz
        //.enable_clk = true,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
    };
    sai_master_clock_t saiMasterCfg = {
        .mclkOutputEnable = true,
     };

    /* clang-format on */
    SystemPlatformInit();
    BOARD_InitBootPins();
    BOARD_InitI2C4Pins();
    BOARD_InitSAIPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_ConfigMPU();

    /* Set AUDMIX to bypass mode */
    BOARD_SetAudmixMode(0x0U);

    CLOCK_SetRate(&audiopll1vcoCLKCfg);
    CLOCK_EnableClock(audiopll1vcoCLKCfg.clkId);
    CLOCK_SetRate(&audiopll1CLKCfg);
    CLOCK_EnableClock(audiopll1CLKCfg.clkId);
    CLOCK_SetRate(&lpi2cCLKCfg);
    CLOCK_EnableClock(lpi2cCLKCfg.clkId);
    CLOCK_SetParent(&saiCLKCfg);
    CLOCK_SetRate(&saiCLKCfg);
    CLOCK_EnableClock(saiCLKCfg.clkId);

    /* select MCLK direction(Enable MCLK clock) */
    saiMasterCfg.mclkSourceClkHz = DEMO_SAI_CLK_FREQ;            /* setup source clock for MCLK */
    saiMasterCfg.mclkHz          = saiMasterCfg.mclkSourceClkHz; /* setup target clock of MCLK */
    SAI_SetMasterClockConfig(DEMO_SAI, &saiMasterCfg);

    wm8962Config.i2cConfig.codecI2CSourceClock = DEMO_I2C_CLK_FREQ;
    wm8962Config.format.mclk_HZ                = DEMO_SAI_CLK_FREQ;
}
/*${function:end}*/
