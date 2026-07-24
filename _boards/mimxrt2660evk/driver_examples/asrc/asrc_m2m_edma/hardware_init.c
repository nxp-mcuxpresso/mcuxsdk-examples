/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "fsl_codec_common.h"
#include "fsl_wm8962.h"
#include "fsl_codec_adapter.h"
#include "fsl_edma.h"
#include "app.h"
/*${header:end}*/

/*${variable:start}*/
edma_channel_config_t channelConfig = {
    .enableMasterIDReplication = true,
    // .securityLevel             = kEDMA_ChannelSecurityLevelSecure,
    .protectionLevel           = kEDMA_ChannelProtectionLevelPrivileged,
};

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
    /* RT2660 does not need a separate BLK_CTRL MCLK-direction toggle — SAI0
     * MCLK direction is fixed by the IOMUX/SAI configuration. Kept as a no-op
     * so shared upstream code that calls this helper continues to link. */
    (void)enable;
}

void BOARD_InitHardware(void)
{
    /* Board common setting: MPU, Power and Clock Tree, TRDC, and Debug Console init. */
    BOARD_CommonSetting();
    BOARD_InitI2CPins();
    BOARD_InitSAIPins();

    /* Audio PLL is already brought up (F_OUT = 49.152 MHz, 48 kHz family) by
     * ConfigCGUAna() inside BOARD_InitBootClocks(). Here we just point the SAI0
     * MCLK0 root at that AUDIOPLL source and divide by 2 to land at
     * 24.576 MHz — the WM8962 MCLK target for 48 kHz / 16-bit stereo. */
    clock_root_config_t saiRootCfg = {
        .mux = kCLOCK_SAI0_MCLK0_ClockRoot_AUDIOPLL,
        .div = 2,
    };
    CLOCK_SetRootClock(kCLOCK_Root_AUDIO_sai0_mclk0, &saiRootCfg);

    BOARD_EnableSaiMclkOutput(true);
}
void Board_SaiEdmaConfig(edma_config_t *config)
{
    config->enableMasterIdReplication       = true;
    config->channelConfig[DEMO_SAI_CHANNEL] = &channelConfig;
}

void Board_AsrcEdmaConfig(edma_config_t *config)
{
    config->enableMasterIdReplication            = true;
    config->channelConfig[DEMO_ASRC_IN_CHANNEL]  = &channelConfig;
    config->channelConfig[DEMO_ASRC_OUT_CHANNEL] = &channelConfig;
}
/*${function:end}*/
