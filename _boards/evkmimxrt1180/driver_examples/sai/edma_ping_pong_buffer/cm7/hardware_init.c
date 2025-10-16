/*
 * Copyright 2023, 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_edma.h"
#include "fsl_trdc.h"
#include "app.h"
#include "fsl_codec_common.h"
#if defined DEMO_CODEC_WM8962
#include "fsl_wm8962.h"
#else
#include "fsl_cs42448.h"
#endif
#include "fsl_codec_adapter.h"
/*${header:end}*/

/*${variable:start}*/
#if DEMO_CODEC_WM8962
wm8962_config_t wm8962Config = {
    .i2cConfig = {.codecI2CInstance = BOARD_CODEC_I2C_INSTANCE, .codecI2CSourceClock = BOARD_CODEC_I2C_CLOCK_FREQ},
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
    .format       = {.mclk_HZ    = 24576000U / 2,
                     .sampleRate = kWM8962_AudioSampleRate48KHz,
                     .bitWidth   = kWM8962_AudioBitWidth16bit},
    .masterSlave  = false,
};
codec_config_t boardCodecConfig = {.codecDevType = kCODEC_WM8962, .codecDevConfig = &wm8962Config};
#elif DEMO_CODEC_CS42448
cs42448_config_t cs42448Config = {
    .DACMode      = kCS42448_ModeSlave,
    .ADCMode      = kCS42448_ModeSlave,
    .reset        = BORAD_CodecReset,
    .master       = false,
    .i2cConfig    = {.codecI2CInstance = DEMO_CS42448_I2C_INSTANCE, .codecI2CSourceClock = BOARD_CODEC_I2C_CLOCK_FREQ},
    .format       = {.mclk_HZ = 24576000, .sampleRate = 48000U, .bitWidth = 16U},
    .bus          = kCS42448_BusI2S,
    .slaveAddress = CS42448_I2C_ADDR,
};

codec_config_t boardCodecConfig = {.codecDevType = kCODEC_CS42448, .codecDevConfig = &cs42448Config};
#else
#error "no codec enabled, please check."
#endif

/*
 * AUDIO PLL setting: Frequency = Fref * (DIV_SELECT + NUM / DENOM) / (2^POST)
 *                              = 24 * (32 + 768/1000) / 2
 *                              = 393.216 MHZ
 */
const clock_audio_pll_config_t audioPllConfig = {
    .loopDivider = 32,   /* PLL loop divider. Valid range for DIV_SELECT divider value: 27~54. */
    .postDivider = 1,    /* Divider after the PLL, should only be 1, 2, 4, 8, 16. */
    .numerator   = 768,  /* 30 bit numerator of fractional loop divider. */
    .denominator = 1000, /* 30 bit denominator of fractional loop divider */
};
/*${variable:end}*/

/*${function:start}*/
void BOARD_EnableSaiMclkOutput(bool enable)
{
    if (enable)
    {
        BLK_CTRL_NS_AONMIX->SAI1_MCLK_CTRL |= BLK_CTRL_NS_AONMIX_SAI1_MCLK_CTRL_SAI1_MCLK_DIR_MASK;
    }
    else
    {
        BLK_CTRL_NS_AONMIX->SAI1_MCLK_CTRL &= ~BLK_CTRL_NS_AONMIX_SAI1_MCLK_CTRL_SAI1_MCLK_DIR_MASK;
    }
}

void BOARD_InitHardware(void)
{
    BOARD_CommonSetting();
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    
#if DEMO_CODEC_WM8962
    BOARD_InitWM8962Pins();
#else
    BOARD_InitCS42448Pins();
#endif

    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Init Audio Pll. */
    CLOCK_InitAudioPll(&audioPllConfig);
    /*Clock setting for SAI1*/
    CLOCK_SetRootClockMux(kCLOCK_Root_Sai1, kCLOCK_SAI1_ClockRoot_MuxAudioPllOut);
#if DEMO_CODEC_WM8962
    CLOCK_SetRootClockDiv(kCLOCK_Root_Sai1, 32);
    /*Clock setting for LPI2C*/
    CLOCK_SetRootClockMux(kCLOCK_Root_Lpi2c0102, kCLOCK_LPI2C0102_ClockRoot_MuxOscRc24M);
    CLOCK_SetRootClockDiv(kCLOCK_Root_Lpi2c0102, 1);
#else
    CLOCK_SetRootClockDiv(kCLOCK_Root_Sai1, 16);
    /*Clock setting for LPI2C*/
    CLOCK_SetRootClockMux(kCLOCK_Root_Lpi2c0304, kCLOCK_LPI2C0304_ClockRoot_MuxOscRc24M);
    CLOCK_SetRootClockDiv(kCLOCK_Root_Lpi2c0304, 1);
#endif
    /*Enable MCLK clock*/
    BOARD_EnableSaiMclkOutput(true);

#if DEMO_CODEC_CS42448
    /* enable codec power */
    RGPIO_PinWrite(DEMO_CODEC_POWER_GPIO, DEMO_CODEC_POWER_GPIO_PIN, 1U);
#endif
}

#if DEMO_CODEC_CS42448
void BORAD_CodecReset(bool state)
{
    if (state)
    {
        RGPIO_PinWrite(DEMO_CODEC_RESET_GPIO, DEMO_CODEC_RESET_GPIO_PIN, 1U);
    }
    else
    {
        RGPIO_PinWrite(DEMO_CODEC_RESET_GPIO, DEMO_CODEC_RESET_GPIO_PIN, 0U);
    }
}
#endif
/*${function:end}*/
