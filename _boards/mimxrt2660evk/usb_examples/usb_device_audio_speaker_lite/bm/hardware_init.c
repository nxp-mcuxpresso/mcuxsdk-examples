/*
 * Copyright 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"
#include "usb_device_ch9.h"
#include "usb_device_descriptor.h"
#include "usb_device_audio.h"
#include "audio_speaker.h"
#include "pin_mux.h"
#include "usb_phy.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "fsl_sai.h"
#include "fsl_sai_edma.h"
#include "fsl_codec_common.h"
#include "fsl_wm8962.h"
#include "fsl_adapter_audio.h"
#include "fsl_codec_adapter.h"
/*${header:end}*/

/*${prototype:start}*/
extern uint32_t USB_AudioSpeakerBufferSpaceUsed(void);
extern void USB_DeviceCalculateFeedback(void);
extern usb_audio_speaker_struct_t g_UsbDeviceAudioSpeaker;
extern uint8_t audioPlayDataBuff[AUDIO_SPEAKER_DATA_WHOLE_BUFFER_COUNT_NORMAL * AUDIO_PLAY_BUFFER_SIZE_ONE_FRAME];
/*${prototype:end}*/

/*${variable:start}*/
HAL_AUDIO_HANDLE_DEFINE(audioTxHandle);
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) static uint8_t audioPlayDMATempBuff[AUDIO_PLAY_BUFFER_SIZE_ONE_FRAME];
codec_handle_t codecHandle;

hal_audio_dma_channel_mux_config_t dmaTxChannelSource = {
    .dmaChannelMuxConfig.dmaRequestSource = DEMO_SAI_DMA_TX_CHANNEL,
};

hal_audio_dma_config_t dmaTxConfig = {
    .instance            = DEMO_DMA_INDEX,
    .channel             = DEMO_DMA_TX_CHANNEL,
    .priority            = kHAL_AudioDmaChannelPriorityDefault,
    .dmaChannelMuxConfig = (void *)&dmaTxChannelSource,
    .dmaChannelConfig    = NULL,
    .dmaExtraConfig      = NULL,
};

hal_audio_ip_config_t ipTxConfig = {
    .sai.lineMask = 1U << DEMO_SAI_CHANNEL,
    .sai.syncMode = kHAL_AudioSaiModeAsync,
};

hal_audio_config_t audioTxConfig = {
    .dmaConfig         = &dmaTxConfig,
    .ipConfig          = &ipTxConfig,
    .instance          = DEMO_SAI_INDEX,
    .sampleRate_Hz     = (uint32_t)kHAL_AudioSampleRate48KHz,
    .masterSlave       = kHAL_AudioMaster,
    .bclkPolarity      = kHAL_AudioSampleOnRisingEdge,
    .frameSyncWidth    = kHAL_AudioFrameSyncWidthHalfFrame,
    .frameSyncPolarity = kHAL_AudioBeginAtFallingEdge,
    .dataFormat        = kHAL_AudioDataFormatI2sClassic,
    .fifoWatermark     = (uint8_t)(FSL_FEATURE_SAI_FIFO_COUNTn(DEMO_SAI) - 1),
    .bitWidth          = (uint8_t)kHAL_AudioWordWidth16bits,
    .lineChannels      = kHAL_AudioStereo,
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
void BOARD_MASTER_CLOCK_CONFIG(void)
{
    sai_master_clock_t mclkConfig = {0};

    mclkConfig.mclkOutputEnable = true;
    mclkConfig.mclkHz           = DEMO_AUDIO_MASTER_CLOCK;
    mclkConfig.mclkSourceClkHz  = DEMO_SAI_CLK_FREQ;

    SAI_SetMasterClockConfig(DEMO_SAI, &mclkConfig);
}

void BOARD_Codec_Init()
{
    BOARD_MASTER_CLOCK_CONFIG();

    if (CODEC_Init(&codecHandle, &boardCodecConfig) != kStatus_Success)
    {
        assert(false);
    }
    if (CODEC_SetVolume(&codecHandle, kCODEC_PlayChannelHeadphoneLeft | kCODEC_PlayChannelHeadphoneRight, 50U) !=
        kStatus_Success)
    {
        assert(false);
    }

    SDK_DelayAtLeastUs(5000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
}

static void txCallback(hal_audio_handle_t handle, hal_audio_status_t completionStatus, void *callbackParam)
{
    uint32_t audioSpeakerPreReadDataCount = 0U;
    uint32_t preAudioSendCount            = 0U;
    hal_audio_transfer_t xfer             = {0};

    if ((USB_AudioSpeakerBufferSpaceUsed() < (g_UsbDeviceAudioSpeaker.audioPlayTransferSize)) &&
        (g_UsbDeviceAudioSpeaker.startPlayFlag == 1U))
    {
        g_UsbDeviceAudioSpeaker.startPlayFlag          = 0;
        g_UsbDeviceAudioSpeaker.speakerDetachOrNoInput = 1;
    }
    if (0U != g_UsbDeviceAudioSpeaker.startPlayFlag)
    {
        USB_DeviceCalculateFeedback();

        xfer.dataSize     = g_UsbDeviceAudioSpeaker.audioPlayTransferSize;
        xfer.data         = audioPlayDataBuff + g_UsbDeviceAudioSpeaker.tdReadNumberPlay;
        preAudioSendCount = g_UsbDeviceAudioSpeaker.audioSendCount[0];
        g_UsbDeviceAudioSpeaker.audioSendCount[0] += g_UsbDeviceAudioSpeaker.audioPlayTransferSize;
        if (preAudioSendCount > g_UsbDeviceAudioSpeaker.audioSendCount[0])
        {
            g_UsbDeviceAudioSpeaker.audioSendCount[1] += 1U;
        }
        g_UsbDeviceAudioSpeaker.audioSendTimes++;
        g_UsbDeviceAudioSpeaker.tdReadNumberPlay += g_UsbDeviceAudioSpeaker.audioPlayTransferSize;
        if (g_UsbDeviceAudioSpeaker.tdReadNumberPlay >= g_UsbDeviceAudioSpeaker.audioPlayBufferSize)
        {
            g_UsbDeviceAudioSpeaker.tdReadNumberPlay = 0;
        }
        audioSpeakerPreReadDataCount = g_UsbDeviceAudioSpeaker.audioSpeakerReadDataCount[0];
        g_UsbDeviceAudioSpeaker.audioSpeakerReadDataCount[0] += g_UsbDeviceAudioSpeaker.audioPlayTransferSize;
        if (audioSpeakerPreReadDataCount > g_UsbDeviceAudioSpeaker.audioSpeakerReadDataCount[0])
        {
            g_UsbDeviceAudioSpeaker.audioSpeakerReadDataCount[1] += 1U;
        }
    }
    else
    {
        if (0U != g_UsbDeviceAudioSpeaker.audioPlayTransferSize)
        {
            xfer.dataSize = g_UsbDeviceAudioSpeaker.audioPlayTransferSize;
        }
        else
        {
            xfer.dataSize = AUDIO_PLAY_BUFFER_SIZE_ONE_FRAME / 8U;
        }
        xfer.data = audioPlayDMATempBuff;
    }
    HAL_AudioTransferSendNonBlocking(handle, &xfer);
}

void AUDIO_DMA_EDMA_Start()
{
    usb_echo("Init Audio SAI and CODEC\r\n");
    hal_audio_transfer_t xfer = {0};
    memset(audioPlayDMATempBuff, 0, AUDIO_PLAY_BUFFER_SIZE_ONE_FRAME);
    xfer.dataSize = AUDIO_PLAY_BUFFER_SIZE_ONE_FRAME / 8U;
    xfer.data     = audioPlayDMATempBuff;
    HAL_AudioTxInstallCallback((hal_audio_handle_t)&audioTxHandle[0], txCallback, NULL);
    HAL_AudioTransferSendNonBlocking((hal_audio_handle_t)&audioTxHandle[0], &xfer);
}

void BOARD_EnableSaiMclkOutput(bool enable)
{
    /* On RT2660 the SAI0 MCLK output to the pad is dynamically clock-gated by
     * default (MODCON SAI0 CFG[OVERRIDE_IPG_MCLK_EN] resets to 0). An external
     * audio codec needs a free-running MCLK, so disable the dynamic gate to
     * keep MCLK toggling continuously. Without this the WM8962 gets no SYSCLK
     * and CODEC_Init's power-up register writes are silently dropped (control
     * interface needs SYSCLK when SYSCLK_ENA=1), leaving the DAC and headphone
     * path powered down -> no audio. */
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

void BOARD_InitHardware(void)
{
    /* Board common setting: MPU, Power and Clock Tree, TRDC, and Debug Console init. */
    BOARD_CommonSetting();
    BOARD_InitSAIPins();
    BOARD_InitI2CPins();

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

    audioTxConfig.srcClock_Hz = DEMO_SAI_CLK_FREQ;
}

#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
void USBHS_IRQHandler(void)
{
    USB_DeviceEhciIsrFunction(g_UsbDeviceAudioSpeaker.deviceHandle);
}
#endif
#if (defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U))
void USBFS_IRQHandler(void)
{
    USB_DeviceKhciIsrFunction(g_UsbDeviceAudioSpeaker.deviceHandle);
}
#endif

void USB_DeviceClockInit(void)
{
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
    uint32_t usbClockFreq             = 24000000;
    usb_phy_config_struct_t phyConfig = {
        BOARD_USB_PHY_D_CAL,
        BOARD_USB_PHY_TXCAL45DP,
        BOARD_USB_PHY_TXCAL45DM,
    };

    CLOCK_EnableUsbhsPhyPllClock(kCLOCK_Usbphy480M, usbClockFreq);
    CLOCK_EnableUsbhsClock(kCLOCK_UsbSrcUnused, usbClockFreq);
    USB_EhciPhyInit(CONTROLLER_ID, usbClockFreq, &phyConfig);
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
    CLOCK_EnableUsbfsClock();
#endif
}

void USB_DeviceIsrEnable(void)
{
    IRQn_Type irqNumber;

#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
    IRQn_Type usbDeviceEhciIrq[] = USBHS_IRQS;
    irqNumber                    = usbDeviceEhciIrq[CONTROLLER_ID - kUSB_ControllerEhci0];

    /* Install isr, set priority, and enable IRQ. */
    NVIC_SetPriority(irqNumber, USB_DEVICE_INTERRUPT_PRIORITY);
    EnableIRQ(irqNumber);
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
    IRQn_Type usbDeviceKhciIrq[] = USBFS_IRQS;
    irqNumber                    = usbDeviceKhciIrq[CONTROLLER_ID - kUSB_ControllerKhci0];

    /* Install isr, set priority, and enable IRQ. */
    NVIC_SetPriority(irqNumber, USB_DEVICE_INTERRUPT_PRIORITY);
    EnableIRQ(irqNumber);
#endif
}

#if USB_DEVICE_CONFIG_USE_TASK
void USB_DeviceTaskFn(void *deviceHandle)
{
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
    USB_DeviceEhciTaskFunction(deviceHandle);
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
    USB_DeviceKhciTaskFunction(deviceHandle);
#endif
}
#endif
/*${function:end}*/
