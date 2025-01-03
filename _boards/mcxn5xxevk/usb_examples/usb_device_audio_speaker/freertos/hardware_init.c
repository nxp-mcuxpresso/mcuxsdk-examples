/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "fsl_common.h"
#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"
#include "usb_device_class.h"
#include "usb_device_ch9.h"
#include "usb_audio_config.h"
#include "usb_device_descriptor.h"
#include "usb_device_audio.h"
#include "audio_speaker.h"
#include "pin_mux.h"
#include "usb_phy.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "fsl_gpio.h"
#include "fsl_port.h"
#include "fsl_sai.h"
#include "fsl_sai_edma.h"
#include "fsl_adapter_audio.h"
#include "fsl_codec_adapter.h"
#include "fsl_codec_common.h"
#include "fsl_dialog7212.h"
#if defined(USB_DEVICE_AUDIO_USE_SYNC_MODE) && (USB_DEVICE_AUDIO_USE_SYNC_MODE > 0U)
#include "fsl_ctimer.h"
#endif
/*${header:end}*/
/*${prototype:start}*/
extern uint32_t USB_AudioSpeakerBufferSpaceUsed(void);
#if defined(USB_DEVICE_AUDIO_USE_SYNC_MODE) && (USB_DEVICE_AUDIO_USE_SYNC_MODE > 0U)
#if ((defined USB_DEVICE_CONFIG_LPCIP3511FS) && (USB_DEVICE_CONFIG_LPCIP3511FS) || \
     (defined USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI))
void CTIMER_SOF_TOGGLE_HANDLER_FRO(uint32_t i);
#endif
void CTIMER_SOF_TOGGLE_HANDLER_PLL(uint32_t i);
#endif
/*${prototype:end}*/

/*${variable:start}*/
extern usb_audio_speaker_struct_t g_UsbDeviceAudioSpeaker;
extern uint8_t audioPlayDataBuff[AUDIO_SPEAKER_DATA_WHOLE_BUFFER_COUNT_NORMAL * AUDIO_PLAY_BUFFER_SIZE_ONE_FRAME];
HAL_AUDIO_HANDLE_DEFINE(audioTxHandle);
hal_audio_config_t audioTxConfig;
hal_audio_dma_config_t dmaTxConfig;
hal_audio_ip_config_t ipTxConfig;
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
static uint8_t audioPlayDMATempBuff[AUDIO_PLAY_BUFFER_SIZE_ONE_FRAME];
codec_handle_t codecHandle;
#if (defined(FSL_FEATURE_SAI_HAS_MCR) && (FSL_FEATURE_SAI_HAS_MCR)) || \
    (defined(FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER) && (FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER))
sai_master_clock_t mclkConfig;
#endif
hal_audio_dma_channel_mux_config_t dmaChannelSource = {
    .dmaChannelMuxConfig.dmaRequestSource = DEMO_SAI_TX_SOURCE,
};
da7212_pll_config_t pllconfig = {
    .outputClock_HZ = kDA7212_PLLOutputClk12288000,
    .refClock_HZ    = 12288000U,
    .source         = kDA7212_PLLClkSourceMCLK,
};

da7212_config_t da7212Config = {
    .i2cConfig    = {.codecI2CInstance = BOARD_CODEC_I2C_INSTANCE, .codecI2CSourceClock = 12000000U},
    .dacSource    = kDA7212_DACSourceInputStream,
    .slaveAddress = DA7212_ADDRESS,
    .protocol     = kDA7212_BusI2S,
    .format       = {.mclk_HZ = 12288000U, .sampleRate = 48000U, .bitWidth = 16U},
    .isMaster     = true,
    .pll          = &pllconfig,
    .sysClkSource = kDA7212_SysClkSourcePLL,
};
codec_config_t boardCodecConfig = {.codecDevType = kCODEC_DA7212, .codecDevConfig = &da7212Config};

#if defined(USB_DEVICE_AUDIO_USE_SYNC_MODE) && (USB_DEVICE_AUDIO_USE_SYNC_MODE > 0U)
#if (defined USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI)
static ctimer_config_t ctimerInfoFro;
ctimer_callback_t *cb_func_fro[] = {(ctimer_callback_t *)CTIMER_SOF_TOGGLE_HANDLER_FRO};
#endif
ctimer_callback_t *cb_func_pll[] = {(ctimer_callback_t *)CTIMER_SOF_TOGGLE_HANDLER_PLL};
static ctimer_config_t ctimerInfoPll;
#endif
/*${variable:end}*/
/*${function:start}*/
extern void DA7212_USB_Audio_Init(void *I2CBase, void *i2cHandle);
extern void DA7212_Config_Audio_Formats(uint32_t samplingRate);
extern uint32_t USB_AudioSpeakerBufferSpaceUsed(void);
extern void USB_DeviceCalculateFeedback(void);

/*******************************************************************************
 * Code
 ******************************************************************************/

void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_BootClockFROHF144M();
    BOARD_InitDebugConsole();

    CLOCK_EnableClock(kCLOCK_InputMux);

    /* attach FRO 12M to LPFLEXCOMM4 (debug console) */
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom4Clk, 1u);
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    /* attach FRO 12M to LPFLEXCOMM2 */
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom2Clk, 1u);
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM2);

#if (defined USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI)
    /*!< Set up PLL0 */
    const pll_setup_t pll0Setup = {
        .pllctrl = SCG_APLLCTRL_SOURCE(1U) | SCG_APLLCTRL_LIMUPOFF_MASK | SCG_APLLCTRL_SELI(4U) |
                   SCG_APLLCTRL_SELP(3U) | SCG_APLLCTRL_SELR(4U),
        .pllndiv = SCG_APLLNDIV_NDIV(10U),
        .pllpdiv = SCG_APLLPDIV_PDIV(10U),
        .pllsscg = {(SCG_APLLSSCG0_SS_MDIV_LSB(3435973837U)),
                    ((SCG0->APLLSSCG1 & ~SCG_APLLSSCG1_SS_PD_MASK) | SCG_APLLSSCG1_SS_MDIV_MSB(0U) |
                     (uint32_t)(kSS_MF_512) | (uint32_t)(kSS_MR_K0) | (uint32_t)(kSS_MC_NOC) |
                     SCG_APLLSSCG1_SEL_SS_MDIV_MASK)},
        .pllRate = 24576000U};
    CLOCK_SetPLL0Freq(&pll0Setup); /*!< Configure PLL0 to the desired values */
#elif (defined USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI)
    CLOCK_SetupExtClocking(24000000U);
    CLOCK_SetSysOscMonitorMode(kSCG_SysOscMonitorDisable); /* System OSC Clock Monitor is disabled */
    /*!< Set up PLL0 */
    const pll_setup_t pll0Setup = {
        .pllctrl = SCG_APLLCTRL_SOURCE(0U) | SCG_APLLCTRL_LIMUPOFF_MASK | SCG_APLLCTRL_SELI(4U) |
                   SCG_APLLCTRL_SELP(3U) | SCG_APLLCTRL_SELR(4U),
        .pllndiv = SCG_APLLNDIV_NDIV(6U),
        .pllpdiv = SCG_APLLPDIV_PDIV(7U),
        .pllsscg = {(SCG_APLLSSCG0_SS_MDIV_LSB(2886218023U)),
                    ((SCG0->APLLSSCG1 & ~SCG_APLLSSCG1_SS_PD_MASK) | SCG_APLLSSCG1_SS_MDIV_MSB(0U) |
                     (uint32_t)(kSS_MF_512) | (uint32_t)(kSS_MR_K0) | (uint32_t)(kSS_MC_NOC) |
                     SCG_APLLSSCG1_SEL_SS_MDIV_MASK)},
        .pllRate = 24576000U};
    CLOCK_SetPLL0Freq(&pll0Setup); /*!< Configure PLL0 to the desired values */
#endif
    /* force the APLL power on and gate on */
    SCG0->APLL_OVRD |= SCG_APLL_OVRD_APLLPWREN_OVRD_MASK | SCG_APLL_OVRD_APLLCLKEN_OVRD_MASK;
    SCG0->APLL_OVRD |= SCG_APLL_OVRD_APLL_OVRD_EN_MASK;
    CLOCK_SetClkDiv(kCLOCK_DivPllClk, 1U);

    /* attach PLL0 to SAI1 */
    CLOCK_SetClkDiv(kCLOCK_DivSai1Clk, 1u);
    CLOCK_AttachClk(kPLL0_to_SAI1);

#if (defined USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI)
    CLOCK_SetClkDiv(kCLOCK_DivCtimer0Clk, 1u);
    CLOCK_AttachClk(kPLL0_to_CTIMER0);
#elif (defined USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI)
    CLOCK_SetClkDiv(kCLOCK_DivCtimer0Clk, 1u);
    CLOCK_AttachClk(kPLL0_to_CTIMER0);
    CLOCK_SetClkDiv(kCLOCK_DivCtimer1Clk, 1u);
    CLOCK_AttachClk(kFRO_HF_to_CTIMER1);
#endif

    dmaTxConfig.instance            = DEMO_DMA_INDEX;
    dmaTxConfig.channel             = DEMO_DMA_TX_CHANNEL;
    dmaTxConfig.priority            = kHAL_AudioDmaChannelPriorityDefault;
    dmaTxConfig.dmaChannelMuxConfig = (void *)&dmaChannelSource;
    ipTxConfig.sai.lineMask         = 1U << 0U;
    ipTxConfig.sai.syncMode         = kHAL_AudioSaiModeAsync;
    audioTxConfig.dmaConfig         = &dmaTxConfig;
    audioTxConfig.ipConfig          = &ipTxConfig;
    audioTxConfig.instance          = DEMO_SAI_INSTANCE_INDEX;
    audioTxConfig.srcClock_Hz       = DEMO_SAI_CLK_FREQ;
    audioTxConfig.sampleRate_Hz     = (uint32_t)kHAL_AudioSampleRate48KHz;
    audioTxConfig.masterSlave       = kHAL_AudioSlave;
    audioTxConfig.bclkPolarity      = kHAL_AudioSampleOnRisingEdge;
    audioTxConfig.frameSyncWidth    = kHAL_AudioFrameSyncWidthHalfFrame;
    audioTxConfig.frameSyncPolarity = kHAL_AudioBeginAtFallingEdge;
    audioTxConfig.dataFormat        = kHAL_AudioDataFormatI2sClassic;
    audioTxConfig.fifoWatermark     = (uint8_t)((uint32_t)FSL_FEATURE_SAI_FIFO_COUNTn(DEMO_SAI) / 2U);
    audioTxConfig.bitWidth          = (uint8_t)kHAL_AudioWordWidth16bits;
    audioTxConfig.lineChannels      = kHAL_AudioStereo;
}

#if defined(USB_DEVICE_AUDIO_USE_SYNC_MODE) && (USB_DEVICE_AUDIO_USE_SYNC_MODE > 0U)
void audio_fro_trim_up(void)
{
    volatile uint32_t trim_value = 0;
    trim_value                   = (uint32_t)(SCG0->FIRCTRIM & SCG_FIRCSTAT_TRIMFINE_MASK);
    trim_value++;
    SCG0->FIRCTRIM = ((SCG0->FIRCTRIM & 0xFFFFFF00U) | (trim_value & 0x000000FFU));
}

void audio_fro_trim_down(void)
{
    volatile uint32_t trim_value = 0;
    trim_value                   = (uint32_t)(SCG0->FIRCTRIM & SCG_FIRCSTAT_TRIMFINE_MASK);
    trim_value--;
    SCG0->FIRCTRIM = ((SCG0->FIRCTRIM & 0xFFFFFF00U) | (trim_value & 0x000000FFU));
}
#endif

void BOARD_Codec_Init()
{
    if (CODEC_Init(&codecHandle, &boardCodecConfig) != kStatus_Success)
    {
        assert(false);
    }
    if (CODEC_SetVolume(&codecHandle, kCODEC_PlayChannelHeadphoneLeft | kCODEC_PlayChannelHeadphoneRight,
                        DEMO_CODEC_VOLUME) != kStatus_Success)
    {
        assert(false);
    }
    /* delay for codec output stable */
    SDK_DelayAtLeastUs(1000000U, SystemCoreClock);
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
#if defined(USB_DEVICE_AUDIO_USE_SYNC_MODE) && (USB_DEVICE_AUDIO_USE_SYNC_MODE > 0U)
#else
        USB_DeviceCalculateFeedback();
#endif
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
    HAL_AudioTransferSendNonBlocking((hal_audio_handle_t)&audioTxHandle[0], &xfer);
}

void AUDIO_DMA_EDMA_Start()
{
    usb_echo("Init Audio SAI and CODEC\r\n");
    hal_audio_transfer_t xfer   = {0};
    mclkConfig.mclkOutputEnable = true;
    mclkConfig.mclkHz           = 12288000U;
    mclkConfig.mclkSourceClkHz  = 24576000U;
    SAI_SetMasterClockConfig(DEMO_SAI, &mclkConfig);
    memset(audioPlayDMATempBuff, 0, AUDIO_PLAY_BUFFER_SIZE_ONE_FRAME);
    xfer.dataSize = AUDIO_PLAY_BUFFER_SIZE_ONE_FRAME / 8U;
    xfer.data     = audioPlayDMATempBuff;
    HAL_AudioTxInstallCallback((hal_audio_handle_t)&audioTxHandle[0], txCallback, NULL);
    HAL_AudioTransferSendNonBlocking((hal_audio_handle_t)&audioTxHandle[0], &xfer);
}
#if defined(USB_DEVICE_AUDIO_USE_SYNC_MODE) && (USB_DEVICE_AUDIO_USE_SYNC_MODE > 0U)
void USB_AudioPllChange(void)
{
    SCG0->APLLSSCG0 = g_UsbDeviceAudioSpeaker.curAudioPllFrac;
    SCG0->APLLSSCG1 |= SCG_APLLSSCG1_SS_MDIV_REQ_MASK;
}

void CTIMER_CaptureInit(void)
{
#if (defined USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI)
    INPUTMUX->CTIMER0CAP0 = 0x14U; /* 0x15U for USB1 and 0x14U for USB0. */
#elif (defined USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI)
    INPUTMUX->CTIMER0CAP0 = 0x15U; /* 0x15U for USB1 and 0x14U for USB0. */
#endif
    CTIMER_GetDefaultConfig(&ctimerInfoPll);

    /* Initialize CTimer module */
    CTIMER_Init(CTIMER0, &ctimerInfoPll);

    CTIMER_SetupCapture(CTIMER0, kCTIMER_Capture_0, kCTIMER_Capture_RiseEdge, true);

    CTIMER_RegisterCallBack(CTIMER0, (ctimer_callback_t *)&cb_func_pll[0], kCTIMER_SingleCallback);

    /* Start the L counter */
    CTIMER_StartTimer(CTIMER0);

    /* if full speed controller, use another ctimer */
#if (defined USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI)
    INPUTMUX->CTIMER1CAP0 = 0x14U; /* 0x15U for USB1 and 0x14U for USB0. */

    CTIMER_GetDefaultConfig(&ctimerInfoFro);

    CTIMER_Init(CTIMER1, &ctimerInfoFro);

    CTIMER_SetupCapture(CTIMER1, kCTIMER_Capture_0, kCTIMER_Capture_RiseEdge, true);

    CTIMER_RegisterCallBack(CTIMER1, (ctimer_callback_t *)&cb_func_fro[0], kCTIMER_SingleCallback);

    /* Start the L counter */
    CTIMER_StartTimer(CTIMER1);
#endif
}
#endif

#if (defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U))
void USB1_HS_IRQHandler(void)
{
    USB_DeviceEhciIsrFunction(g_UsbDeviceAudioSpeaker.deviceHandle);
}
#endif
#if (defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U))
void USB0_FS_IRQHandler(void)
{
    USB_DeviceKhciIsrFunction(g_UsbDeviceAudioSpeaker.deviceHandle);
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
#endif
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
    SPC0->ACTIVE_VDELAY = 0x0500;
    /* Change the power DCDC to 1.8v (By deafult, DCDC is 1.8V), CORELDO to 1.1v (By deafult, CORELDO is 1.0V) */
    SPC0->ACTIVE_CFG &= ~SPC_ACTIVE_CFG_CORELDO_VDD_DS_MASK;
    SPC0->ACTIVE_CFG |= SPC_ACTIVE_CFG_DCDC_VDD_LVL(0x3) | SPC_ACTIVE_CFG_CORELDO_VDD_LVL(0x3) |
                        SPC_ACTIVE_CFG_SYSLDO_VDD_DS_MASK | SPC_ACTIVE_CFG_DCDC_VDD_DS(0x2u);
    /* Wait until it is done */
    while (SPC0->SC & SPC_SC_BUSY_MASK)
        ;
    if (0u == (SCG0->LDOCSR & SCG_LDOCSR_LDOEN_MASK))
    {
        SCG0->TRIM_LOCK = 0x5a5a0001U;
        SCG0->LDOCSR |= SCG_LDOCSR_LDOEN_MASK;
        /* wait LDO ready */
        while (0U == (SCG0->LDOCSR & SCG_LDOCSR_VOUT_OK_MASK))
            ;
    }
    SYSCON->AHBCLKCTRLSET[2] |= SYSCON_AHBCLKCTRL2_USB_HS_MASK | SYSCON_AHBCLKCTRL2_USB_HS_PHY_MASK;
    SYSCON->CLOCK_CTRL |= SYSCON_CLOCK_CTRL_CLKIN_ENA_MASK | SYSCON_CLOCK_CTRL_CLKIN_ENA_FM_USBH_LPT_MASK;
    CLOCK_EnableClock(kCLOCK_UsbHs);
    CLOCK_EnableClock(kCLOCK_UsbHsPhy);
    CLOCK_EnableUsbhsPhyPllClock(kCLOCK_Usbphy480M, 24000000U);
    CLOCK_EnableUsbhsClock();
    USB_EhciPhyInit(CONTROLLER_ID, BOARD_XTAL0_CLK_HZ, &phyConfig);
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
    CLOCK_AttachClk(kCLK_48M_to_USB0);
    CLOCK_EnableClock(kCLOCK_Usb0Ram);
    CLOCK_EnableClock(kCLOCK_Usb0Fs);
    CLOCK_EnableUsbfsClock();
#endif
}

void USB_DeviceIsrEnable(void)
{
    uint8_t irqNumber;
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
    uint8_t usbDeviceEhciIrq[] = USBHS_IRQS;
    irqNumber                  = usbDeviceEhciIrq[CONTROLLER_ID - kUSB_ControllerEhci0];
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
    uint8_t usbDeviceKhciIrq[] = USBFS_IRQS;
    irqNumber                  = usbDeviceKhciIrq[CONTROLLER_ID - kUSB_ControllerKhci0];
#endif
    /* Install isr, set priority, and enable IRQ. */
    NVIC_SetPriority((IRQn_Type)irqNumber, USB_DEVICE_INTERRUPT_PRIORITY);
    EnableIRQ((IRQn_Type)irqNumber);
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
