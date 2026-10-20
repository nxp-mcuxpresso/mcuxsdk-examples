/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_dual_adc.h"
#include "fsl_edma.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* ADC and eDMA configuration - defined in board-specific app.h:
 *   DEMO_DUALADC_BASE               : DADC peripheral base pointer (e.g. ADC0)
 *   DEMO_DUALADC_USER_CHANNEL_0..3  : analog channels to convert (0~31)
 *   DEMO_DMA_BASEADDR               : eDMA peripheral base pointer
 *   DEMO_DMA_CHANNEL_0              : eDMA channel used for the transfer
 *   DEMO_DMA_IRQ / DEMO_DMA_IRQ_HANDLER : eDMA channel IRQ number and handler
 *   DEMO_DMA_REQUEST                : DualADC eDMA hardware request source
 *   DEMO_DUALADC_RESULT_COUNT       : number of command results per sequence
 */

/* Number of command results transferred by one DMA request (CMD1~CMD4). */
#define DEMO_DUALADC_RESULT_COUNT (4U)

/* 12-bit standard resolution, JLEFT=0: result is at bit[14:3]; right-shift by 3. */
#define DEMO_DUALADC_RESULT_MASK  (0xFFFFU)
#define DEMO_DUALADC_RESULT_SHIFT (3U)
#define DEMO_DUALADC_FULL_RANGE   (4096U)

#define LOG_INFO (void)PRINTF

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void DEMO_ConfigureADC(void);
static void DEMO_ConfigureEDMA(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool g_DualAdcDmaTransferDoneFlag = false;

/*
 * eDMA destination buffer; one slot per command result (CMD1~CMD4). Placed in a
 * non-cacheable, 32-bit-aligned section so the eDMA sees coherent data.
 */
AT_NONCACHEABLE_SECTION_ALIGN_INIT(uint32_t g_DualAdcResult[DEMO_DUALADC_RESULT_COUNT], 4U) = {0U};

/*******************************************************************************
 * Code
 ******************************************************************************/
void DEMO_DMA_IRQ_HANDLER(void)
{
    if (0U != (EDMA_GetChannelStatusFlags(DEMO_DMA_BASEADDR, DEMO_DMA_CHANNEL_0) & (uint32_t)kEDMA_InterruptFlag))
    {
        EDMA_ClearChannelStatusFlags(DEMO_DMA_BASEADDR, DEMO_DMA_CHANNEL_0, (uint32_t)kEDMA_InterruptFlag);
        EDMA_EnableChannelRequest(DEMO_DMA_BASEADDR, DEMO_DMA_CHANNEL_0);
        g_DualAdcDmaTransferDoneFlag = true;
    }

    SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief Main function
 */
int main(void)
{
    uint32_t i;
    uint16_t value;

    BOARD_InitHardware();

    LOG_INFO("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);
    LOG_INFO("DualADC eDMA Example\r\n");

    /* Configure the DualADC command sequence and enable its DMA request. */
    DEMO_ConfigureADC();

    /* Configure the eDMA channel to move the conversion results to memory. */
    DEMO_ConfigureEDMA();

    LOG_INFO("ADC Full Range: %d\r\n", DEMO_DUALADC_FULL_RANGE);
    LOG_INFO("Please press any key to get user channel's ADC value.\r\n");

    while (true)
    {
        (void)GETCHAR();

        g_DualAdcDmaTransferDoneFlag = false;

        /* Issue a software trigger to start conversion (Trigger0 starts ADCA). */
        DUALADC_SetSoftwareTrigger(DEMO_DUALADC_BASE, (uint32_t)kDUALADC_SoftwareTrigger0);

        /* Wait for the eDMA to move all command results into the buffer. */
        while (!g_DualAdcDmaTransferDoneFlag)
        {
        }

        /* resultLow holds bit[14:3] of the 12-bit value; right-shift by 3 to recover. */
        LOG_INFO("ADC value:");
        for (i = 0U; i < DEMO_DUALADC_RESULT_COUNT; i++)
        {
            value = (uint16_t)((g_DualAdcResult[i] & DEMO_DUALADC_RESULT_MASK) >> DEMO_DUALADC_RESULT_SHIFT);
            LOG_INFO(" %d", value);
        }
        LOG_INFO("\r\n");

        /* Re-arm the channel for the next software trigger. */
        EDMA_EnableChannelRequest(DEMO_DMA_BASEADDR, DEMO_DMA_CHANNEL_0);
    }
}

/*!
 * @brief DualADC configuration.
 */
static void DEMO_ConfigureADC(void)
{
    dualadc_config_t adcConfig;
    dualadc_cmd_config_t cmdConfig;

    /* Initialize ADC with default configuration, cooperation mode. */
    DUALADC_GetDefaultConfig(&adcConfig);

    adcConfig.conversionMode        = kDUALADC_CooperationMode;
    adcConfig.enableAnalogPreEnable = true; /* Pre-enable analog for minimum trigger latency. */

    /* Enable the ADCA DMA request, triggered when a whole scan sequence finishes. */
    adcConfig.enableDmaA = true;
    adcConfig.dmaSourceA = kDUALADC_DmaSrcEndOfScan;
    (void)DUALADC_Init(DEMO_DUALADC_BASE, &adcConfig);

    /*
     * Configure the command buffers. Two A-side channels and two B-side channels,
     * each occupying one command buffer. All use 12-bit standard resolution and a
     * single hardware conversion.
     */
    DUALADC_GetDefaultConversionCommandConfig(&cmdConfig);

    cmdConfig.resolutionMode   = kDUALADC_ResolutionStandard;
    cmdConfig.sampleTimeMode   = kDUALADC_SampleTime131_5;
    cmdConfig.hardwareAverages = kDUALADC_HardwareAverage32;

    /* CMD1: A-side single-ended. */
    cmdConfig.channelNumber  = DEMO_DUALADC_USER_CHANNEL_0;
    cmdConfig.conversionType = kDUALADC_ConversionSingleEndedA;
    DUALADC_ConfigConversionCommand(DEMO_DUALADC_BASE, 1U, &cmdConfig);

    /* CMD2: B-side single-ended. */
    cmdConfig.channelNumber  = DEMO_DUALADC_USER_CHANNEL_1;
    cmdConfig.conversionType = kDUALADC_ConversionSingleEndedB;
    DUALADC_ConfigConversionCommand(DEMO_DUALADC_BASE, 2U, &cmdConfig);

    /* CMD3: A-side single-ended. */
    cmdConfig.channelNumber  = DEMO_DUALADC_USER_CHANNEL_2;
    cmdConfig.conversionType = kDUALADC_ConversionSingleEndedA;
    DUALADC_ConfigConversionCommand(DEMO_DUALADC_BASE, 3U, &cmdConfig);

    /* CMD4: B-side single-ended. */
    cmdConfig.channelNumber  = DEMO_DUALADC_USER_CHANNEL_3;
    cmdConfig.conversionType = kDUALADC_ConversionSingleEndedB;
    DUALADC_ConfigConversionCommand(DEMO_DUALADC_BASE, 4U, &cmdConfig);

    /* Mark CMD5 as sequence terminator so the sequence ends after CMD4. */
    DUALADC_SetConversionSequenceEndCommand(DEMO_DUALADC_BASE, 5U);

    /* Enable DualADC before calibration. */
    DUALADC_Enable(DEMO_DUALADC_BASE);

    /* Run full calibration: offset, optional high-speed, and gain. */
    if (kStatus_Success != DUALADC_DoCalibration(DEMO_DUALADC_BASE))
    {
        LOG_INFO("Calibration failed.\r\n");
    }
}

/*!
 * @brief eDMA configuration.
 */
static void DEMO_ConfigureEDMA(void)
{
    edma_transfer_config_t transferConfig;
    edma_channel_config_t channelConfig;
    edma_config_t dmaConfig;
    void *srcAddr;

    /* Initialize the eDMA with the default configuration. */
    EDMA_GetDefaultConfig(&dmaConfig);
    EDMA_Init(DEMO_DMA_BASEADDR, &dmaConfig);

    (void)memset(&channelConfig, 0, sizeof(channelConfig));
    channelConfig.channelPreemptionConfig.enableChannelPreemption = false;
    channelConfig.channelPreemptionConfig.enablePreemptAbility    = true;
    channelConfig.channelRequestSource                            = DEMO_DMA_REQUEST;
    channelConfig.protectionLevel                                 = kEDMA_ChannelProtectionLevelUser;
#if !(defined(FSL_FEATURE_EDMA_HAS_NO_CH_SBR_SEC) && FSL_FEATURE_EDMA_HAS_NO_CH_SBR_SEC)
    channelConfig.securityLevel = kEDMA_ChannelSecurityLevelNonSecure;
#endif

    /*
     * The DualADC conversion results sit in consecutive 32-bit RSLT registers
     * (RSLT[0]..RSLT[DEMO_DUALADC_RESULT_COUNT-1]). On each end-of-scan request the
     * eDMA copies all DEMO_DUALADC_RESULT_COUNT words into the destination buffer.
     */
    srcAddr = (void *)(uintptr_t)(&DEMO_DUALADC_BASE->RSLT[0]);

    EDMA_PrepareTransferConfig(&transferConfig, srcAddr, sizeof(uint32_t), (int16_t)sizeof(uint32_t),
                               g_DualAdcResult, sizeof(uint32_t), (int16_t)sizeof(uint32_t),
                               sizeof(g_DualAdcResult), sizeof(g_DualAdcResult));

    /* After the transfer, reset source and destination back to the first entry. */
    transferConfig.srcMajorLoopOffset = -(int32_t)sizeof(g_DualAdcResult);
    transferConfig.dstMajorLoopOffset = -(int32_t)sizeof(g_DualAdcResult);

    /* Interrupt once the major loop (all results) is complete. */
    transferConfig.enabledInterruptMask = (uint16_t)kEDMA_MajorInterruptEnable;

    EDMA_SetTransferConfig(DEMO_DMA_BASEADDR, DEMO_DMA_CHANNEL_0, &transferConfig, NULL);

    EDMA_InitChannel(DEMO_DMA_BASEADDR, DEMO_DMA_CHANNEL_0, &channelConfig);

    (void)EnableIRQ(DEMO_DMA_IRQ);
    EDMA_EnableChannelRequest(DEMO_DMA_BASEADDR, DEMO_DMA_CHANNEL_0);
}
