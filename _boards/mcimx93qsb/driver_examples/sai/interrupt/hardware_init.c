/*
 * Copyright 2022-2023 NXP
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
    const clock_root_config_t lpi2cClkCfg = {
        .clockOff = false,
	.mux = 0, // 24MHz oscillator source
	.div = 1
    };
    const clock_root_config_t saiClkCfg = {
        .clockOff = false,
        .mux = 1, // select audiopll1out source(393216000 Hz)
        .div = 32, // output 12288000 Hz
    };
    sai_master_clock_t saiMasterCfg = {
        .mclkOutputEnable = true,
     };
    /* clang-format on */

    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_SetRootClock(LPI2C_MASTER_CLOCK_ROOT, &lpi2cClkCfg);
    CLOCK_EnableClock(LPI2C_MASTER_CLOCK_GATE);
    CLOCK_SetRootClock(SAI_CLOCK_ROOT, &saiClkCfg);
    CLOCK_EnableClock(SAI_CLOCK_GATE);
    CLOCK_SetRootClock(BOARD_PCAL6524_I2C_CLOCK_ROOT, &lpi2cClkCfg);
    CLOCK_EnableClock(BOARD_PCAL6524_I2C_CLOCK_GATE);

    /* Select SAI3 signals */
    pcal6524_handle_t handle;
    BOARD_InitPCAL6524(&handle);
    PCAL6524_SetDirection(&handle, (1 << BOARD_PCAL6524_EXP_SEL), kPCAL6524_Output);
    PCAL6524_ClearPins(&handle, (1 << BOARD_PCAL6524_EXP_SEL));

    /* select MCLK direction(Enable MCLK clock) */
    saiMasterCfg.mclkSourceClkHz = DEMO_SAI_CLK_FREQ;            /* setup source clock for MCLK */
    saiMasterCfg.mclkHz          = saiMasterCfg.mclkSourceClkHz; /* setup target clock of MCLK */
    SAI_SetMasterClockConfig(DEMO_SAI, &saiMasterCfg);

    wm8962Config.i2cConfig.codecI2CSourceClock = DEMO_I2C_CLK_FREQ;
    wm8962Config.format.mclk_HZ                = DEMO_SAI_CLK_FREQ;
}

void BOARD_SAI_RXConfig(sai_transceiver_t *config, sai_sync_mode_t sync)
{
    config->syncMode = sync;
    SAI_RxSetConfig(DEMO_SAI, config);
    SAI_RxSetBitClockRate(DEMO_SAI, DEMO_AUDIO_MASTER_CLOCK, DEMO_AUDIO_SAMPLE_RATE, DEMO_AUDIO_BIT_WIDTH,
                          DEMO_AUDIO_DATA_CHANNEL);
}

/*${function:end}*/
