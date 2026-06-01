/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "board.h"
#include "pin_mux.h"
#include "app.h"
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
        .clkId = kCLOCK_Audiopll1ctl,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 3932160000,
    };

    clk_t audiopll1CLKCfg = {
        .clkId = kCLOCK_Audiopll1,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 393216000,
    };

    clk_t pdmClkCfg = {
        .clkId = PDM_CLOCK_ROOT,
        .pclkId = kCLOCK_Audiopll1,
        .rate = 196608000ULL,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
    };

    clk_t lpi2cCLKCfg = {
        .clkId = LPI2C_MASTER_CLOCK_ROOT,
        .pclkId = kCLOCK_Osc24m,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 24000000UL,
    };

/*    clk_t saiCLKCfg = {
        .clkId = SAI_CLOCK_ROOT,
        .pclkId = kCLOCK_Audiopll1, // select audiopll1out source(393216000 Hz)
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 24000000UL,
    };*/
    clk_t saiCLKCfg = {
        .clkId = SAI_CLOCK_ROOT,
        .pclkId = kCLOCK_Audiopll1, // select audiopll1out source(393216000 Hz)
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 12288000ULL, // output 12288000 Hz
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
    };

    /*clk_t pdmClkCfg = {
        .clkId = PDM_CLOCK_ROOT,
        .pclkId = kCLOCK_Audiopll1,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 24000000UL,
    };*/
    sai_master_clock_t saiMasterCfg = {
        .mclkOutputEnable = true,
    };
    /* clang-format on */

    SystemPlatformInit();
    BOARD_InitDebugConsolePins();
    BOARD_InitSAI1Pins();
    BOARD_InitPDMPins();
    BOARD_InitI2C3Pins();
    BOARD_InitI2C6Pins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_ConfigMPU();

    CLOCK_SetRate(&audiopll1vcoCLKCfg);
    CLOCK_EnableClock(audiopll1vcoCLKCfg.clkId);
    CLOCK_SetRate(&audiopll1CLKCfg);
    CLOCK_EnableClock(audiopll1CLKCfg.clkId);
    CLOCK_SetParent(&pdmClkCfg);
    CLOCK_SetRate(&pdmClkCfg);
    CLOCK_EnableClock(pdmClkCfg.clkId);
    CLOCK_SetRate(&lpi2cCLKCfg);
    CLOCK_EnableClock(lpi2cCLKCfg.clkId);
    CLOCK_SetParent(&saiCLKCfg);
    CLOCK_SetRate(&saiCLKCfg);
    CLOCK_EnableClock(saiCLKCfg.clkId);


    BOARD_EXPANDER_SetPinAsOutput(BOARD_PCA6416_I2C6_S3_ID, CAN_PDM_SEL);
    BOARD_EXPANDER_SetPinToLow(BOARD_PCA6416_I2C6_S3_ID, CAN_PDM_SEL);
    BOARD_EXPANDER_SetPinAsOutput(BOARD_PCA6416_I2C3_S5_21_ID, MQS_MIC_SEL);
    BOARD_EXPANDER_SetPinToLow(BOARD_PCA6416_I2C3_S5_21_ID, MQS_MIC_SEL);
    /* Select PDM/SAI signals */
    //pcal6408_handle_t handle;
    //BOARD_InitPCAL6408_I2C4(&handle);
    //PCAL6408_SetDirection(&handle, (1 << BOARD_PCAL6408_SLOT_SAI3_SEL), kPCAL6408_Output);
    //PCAL6408_ClearPins(&handle, (1 << BOARD_PCAL6408_SLOT_SAI3_SEL));
    //SDK_DelayAtLeastUs(10000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    /* Select i2c channel to access codec */
    BOARD_MUX_Select(BOARD_PCA9548_I2C3_ID, BOARD_S4_CHAN_IDX);
    /* select MCLK direction(Enable MCLK clock) */
    saiMasterCfg.mclkSourceClkHz = DEMO_SAI_CLK_FREQ;            /* setup source clock for MCLK */
    saiMasterCfg.mclkHz          = saiMasterCfg.mclkSourceClkHz; /* setup target clock of MCLK */
    SAI_SetMasterClockConfig(DEMO_SAI, &saiMasterCfg);

    wm8962Config.i2cConfig.codecI2CSourceClock = DEMO_I2C_CLK_FREQ;
    wm8962Config.format.mclk_HZ                = DEMO_SAI_CLK_FREQ;
}
/*${function:end}*/
