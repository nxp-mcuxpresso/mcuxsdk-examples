/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "fsl_sai.h"
#include "fsl_codec_common.h"
#include "fsl_wm8962.h"
#include "fsl_codec_adapter.h"
#include "fsl_debug_console.h"
#include "fsl_modcon.h"
/*${header:end}*/

/*${variable:start}*/
codec_handle_t codecHandle    = {0};
wm8962_config_t wm8962Config = {
    .i2cConfig    = {.codecI2CInstance    = BOARD_CODEC_I2C_INSTANCE,
                     .codecI2CSourceClock = BOARD_CODEC_I2C_CLOCK_FREQ},
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
    .format       = {.mclk_HZ    = 24576000U,
                     .sampleRate = kWM8962_AudioSampleRate48KHz,
                     .bitWidth   = kWM8962_AudioBitWidth16bit},
    .masterSlave  = false,
};
codec_config_t boardCodecConfig = {.codecDevType = kCODEC_WM8962, .codecDevConfig = &wm8962Config};
/*${variable:end}*/

/*${function:start}*/

void BOARD_EnableSaiMclkOutput(bool enable)
{
    /* On RT2660 the SAI0 MCLK output to the pad is dynamically clock-gated by
     * default (MODCON SAI0 CFG[OVERRIDE_IPG_MCLK_EN] resets to 0). An external
     * audio codec needs a free-running MCLK, so clear the dynamic gate to keep
     * MCLK toggling continuously. Without this the WM8962 receives no SYSCLK
     * and CODEC_Init's power-up register writes are silently dropped. */
    uint32_t cfg = MODCON_GetCFG(kModCon_AUDIO_SAI0, 0U);

    if (enable)
    {
        cfg |= MODCON_CFG_OVERRIDE_IPG_MCLK_EN_MASK;
    }
    else
    {
        cfg &= ~MODCON_CFG_OVERRIDE_IPG_MCLK_EN_MASK;
    }

    MODCON_SetCFG(kModCon_AUDIO_SAI0, 0U, cfg);
}

int BOARD_CODEC_Init(void)
{
    if (CODEC_Init(&codecHandle, &boardCodecConfig) != kStatus_Success)
    {
        return -1;
    }

    /* Initial volume kept low for hearing safety. */
    CODEC_SetVolume(&codecHandle, kCODEC_PlayChannelHeadphoneLeft | kCODEC_PlayChannelHeadphoneRight, 50);

    return 0;
}

void BOARD_InitHardware(void)
{
    BOARD_CommonSetting();
    BOARD_InitDEBUG_UARTPins();
    BOARD_InitSAIPins();
    /* maestro_playback streams from the SD card via USDHC0. RT2660-EVK routes
     * SD_PWREN through the U23 PCAL6524 expander and CARD_DET / EXP_nIRQ2
     * through a second PCA9555 expander, both on LPI2C0. Bring up USDHC0 pads
     * plus both expanders' I2C/INT pins before BOARD_SD_Config runs, otherwise
     * SD_PWREN never asserts and BOARD_SD_Config blocks waiting for the card. */
    BOARD_InitUSDHC0Pins();
    BOARD_Init6524Pins();
    BOARD_InitPCA9555Pins();
    BOARD_InitDebugConsole();

    /* Audio PLL is already brought up (F_OUT = 49.152 MHz, 48 kHz family) by
     * ConfigCGUAna() inside BOARD_InitBootClocks(). Point the SAI0 MCLK0 root
     * at that AUDIOPLL source and divide by 2 to land at 24.576 MHz — the
     * WM8962 MCLK target for 48 kHz / 16-bit stereo. */
    clock_root_config_t saiRootCfg = {
        .mux = kCLOCK_SAI0_MCLK0_ClockRoot_AUDIOPLL,
        .div = 2,
    };
    CLOCK_SetRootClock(kCLOCK_Root_AUDIO_sai0_mclk0, &saiRootCfg);

    BOARD_EnableSaiMclkOutput(true);
}

/* Enable SAI0 MCLK output to the on-board WM8962 codec by asserting I2S_MCR[MOE].
 * Called from streamer_pcm.c right after SAI_Init(): the MODCON gate set in
 * BOARD_EnableSaiMclkOutput() alone does not drive MCLK; the codec only receives
 * SYSCLK once MOE is set, otherwise its power-up register writes are dropped and
 * the DAC/headphone path stays powered down. Since mclkHz == mclkSourceClkHz the
 * MCR post-divider is bypassed (MCLK = 24.576 MHz). */
void BOARD_MasterClockConfig(void)
{
    sai_master_clock_t mclkConfig = {0};

    mclkConfig.mclkOutputEnable = true;
    mclkConfig.mclkHz           = DEMO_AUDIO_MASTER_CLOCK;
    mclkConfig.mclkSourceClkHz  = DEMO_SAI_CLK_FREQ;

    SAI_SetMasterClockConfig(DEMO_SAI, &mclkConfig);
}
/*${function:end}*/
