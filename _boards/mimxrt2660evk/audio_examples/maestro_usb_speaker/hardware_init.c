/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "audio_speaker.h"
#include "usb_phy.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "fsl_sai.h"
#include "fsl_codec_common.h"
#include "fsl_wm8962.h"
#include "fsl_codec_adapter.h"
#include "fsl_modcon.h"
/*${header:end}*/

/*${variable:start}*/
codec_handle_t codecHandle   = {0};
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
                     .sampleRate = kWM8962_AudioSampleRate16KHz,
                     .bitWidth   = kWM8962_AudioBitWidth16bit},
    .masterSlave  = false,
};
codec_config_t boardCodecConfig = {.codecDevType = kCODEC_WM8962, .codecDevConfig = &wm8962Config};
/*${variable:end}*/

/*${function:start}*/

void BOARD_EnableSaiMclkOutput(bool enable)
{
    /* RT2660: dynamically clock-gate the SAI0 MCLK output pad through MODCON.
     * WM8962 needs a free-running MCLK, so clear the dynamic gate. */
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
    CODEC_Init(&codecHandle, &boardCodecConfig);

    /* Initial volume kept low for hearing safety. */
    CODEC_SetVolume(&codecHandle, kCODEC_PlayChannelHeadphoneLeft | kCODEC_PlayChannelHeadphoneRight, DEMO_VOLUME);

    return 0;
}

/* RT2660 EHCI uses COMM_USB0_IRQn (vector COMM_USB0_IRQHandler, aliased by
 * USBHS_IRQHandler). The shared app only defines the RT1170 USB_OTG* handlers,
 * so route the ISR here or enumeration never starts. */
extern usb_audio_speaker_struct_t g_UsbDeviceAudioSpeaker;

#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
void USBHS_IRQHandler(void)
{
    USB_DeviceEhciIsrFunction(g_UsbDeviceAudioSpeaker.deviceHandle);
}
#endif

void USB_DeviceClockInit(void)
{
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
    usb_phy_config_struct_t phyConfig = {
        BOARD_USB_PHY_D_CAL,
        BOARD_USB_PHY_TXCAL45DP,
        BOARD_USB_PHY_TXCAL45DM,
    };

    /* RT2660 USBHS: enable the controller clock and init the HS PHY from the
     * COMM_usb0 PHY clock root (RT10xx CLOCK_EnableUsbhsPhyPllClock/
     * kCLOCK_Usbphy480M do not exist here). */
    CLOCK_EnableUsbHsClock();
    USB_EhciPhyInit(CONTROLLER_ID, CLOCK_GetRootClockFreq(kCLOCK_Root_COMM_usb0_phyclk), &phyConfig);
#endif
}

void BOARD_InitHardware(void)
{
    BOARD_CommonSetting();
    BOARD_InitDEBUG_UARTPins();
    BOARD_InitSAIPins();
    BOARD_InitDebugConsole();

    /* Route SAI0 MCLK0 root to AUDIOPLL (49.152 MHz) / 2 = 24.576 MHz. */
    clock_root_config_t saiRootCfg = {
        .mux = kCLOCK_SAI0_MCLK0_ClockRoot_AUDIOPLL,
        .div = 2,
    };
    CLOCK_SetRootClock(kCLOCK_Root_AUDIO_sai0_mclk0, &saiRootCfg);

    BOARD_EnableSaiMclkOutput(true);

    /* Clock setting for USB */
    USB_DeviceClockInit();
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
