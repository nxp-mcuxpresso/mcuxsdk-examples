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
 *   DEMO_DMA_CHANNEL_0              : eDMA channel used for the transfer (edma_channel_t)
 *   DEMO_DMA_CHANNEL_0_IRQn         : eDMA channel IRQ number
 *   DEMO_DMA_IRQ_HANDLER            : eDMA channel IRQ handler function name
 *   DEMO_DMA_IRQ_PRIO               : eDMA channel IRQ priority level
 *   DEMO_DMA_REQUEST                : DualADC eDMA mux request source (CH_MUX SRC)
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
void DEMO_DMA_IRQ_HANDLER(void);

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
#pragma interrupt alignsp saveall
void DEMO_DMA_IRQ_HANDLER(void)
{
    if (0U != (EDMA_GetChannelStatusFlags(DEMO_DMA_BASEADDR, DEMO_DMA_CHANNEL_0) &
               kEDMA_ChannelStatusMajorLoopCompleteFlag))
    {
        EDMA_ClearChannelStatusFlags(DEMO_DMA_BASEADDR, DEMO_DMA_CHANNEL_0,
                                     kEDMA_ChannelStatusMajorLoopCompleteFlag);

        /* Re-arm the channel request for the next software trigger. */
        EDMA_EnableChannelRequest(DEMO_DMA_BASEADDR, DEMO_DMA_CHANNEL_0, true);

        g_DualAdcDmaTransferDoneFlag = true;
    }

    SDK_ISR_EXIT_BARRIER;
}
#pragma interrupt off

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
    }
}

/*!
 * @brief DualADC configuration.
 */
static void DEMO_ConfigureADC(void)
{
    dualadc_config_t adcConfig;

    /*
     * Populate the whole DualADC configuration then bring the module up with a single
     * DUALADC_Init call: the conversion command sequence, the DMA request enable and the
     * final ADC enable are all applied from this structure by the dsc_dual_adc driver.
     */
    DUALADC_GetDefaultConfig(&adcConfig);

    adcConfig.eConversionMode        = kDUALADC_CooperationMode;
    adcConfig.bEnableAnalogPreEnable = true; /* Pre-enable analog for minimum trigger latency. */

    /* Enable the ADCA DMA request, triggered when a whole scan sequence finishes. */
    adcConfig.sDmaConfig.bEnableDmaA = true;
    adcConfig.sDmaConfig.eDmaSourceA = kDUALADC_DmaSrcEndOfScan;

    /*
     * Configure the command buffers. Two A-side channels and two B-side channels,
     * each occupying one command buffer. All use 12-bit standard resolution and a
     * single hardware conversion.
     */

    /* CMD1: A-side single-ended. */
    adcConfig.sConvCmdConfig[0].u32ChannelNumber    = DEMO_DUALADC_USER_CHANNEL_0;
    adcConfig.sConvCmdConfig[0].u32ResolutionMode   = kDUALADC_ResolutionStandard;
    adcConfig.sConvCmdConfig[0].u32ConversionType   = kDUALADC_ConversionSingleEndedA;
    adcConfig.sConvCmdConfig[0].u32SampleTimeMode   = kDUALADC_SampleTime131_5;
    adcConfig.sConvCmdConfig[0].u32HardwareAverages = kDUALADC_HardwareAverage32;

    /* CMD2: B-side single-ended. */
    adcConfig.sConvCmdConfig[1].u32ChannelNumber    = DEMO_DUALADC_USER_CHANNEL_1;
    adcConfig.sConvCmdConfig[1].u32ResolutionMode   = kDUALADC_ResolutionStandard;
    adcConfig.sConvCmdConfig[1].u32ConversionType   = kDUALADC_ConversionSingleEndedB;
    adcConfig.sConvCmdConfig[1].u32SampleTimeMode   = kDUALADC_SampleTime131_5;
    adcConfig.sConvCmdConfig[1].u32HardwareAverages = kDUALADC_HardwareAverage32;

    /* CMD3: A-side single-ended. */
    adcConfig.sConvCmdConfig[2].u32ChannelNumber    = DEMO_DUALADC_USER_CHANNEL_2;
    adcConfig.sConvCmdConfig[2].u32ResolutionMode   = kDUALADC_ResolutionStandard;
    adcConfig.sConvCmdConfig[2].u32ConversionType   = kDUALADC_ConversionSingleEndedA;
    adcConfig.sConvCmdConfig[2].u32SampleTimeMode   = kDUALADC_SampleTime131_5;
    adcConfig.sConvCmdConfig[2].u32HardwareAverages = kDUALADC_HardwareAverage32;

    /* CMD4: B-side single-ended. */
    adcConfig.sConvCmdConfig[3].u32ChannelNumber    = DEMO_DUALADC_USER_CHANNEL_3;
    adcConfig.sConvCmdConfig[3].u32ResolutionMode   = kDUALADC_ResolutionStandard;
    adcConfig.sConvCmdConfig[3].u32ConversionType   = kDUALADC_ConversionSingleEndedB;
    adcConfig.sConvCmdConfig[3].u32SampleTimeMode   = kDUALADC_SampleTime131_5;
    adcConfig.sConvCmdConfig[3].u32HardwareAverages = kDUALADC_HardwareAverage32;

    /* Mark CMD5 as the sequence terminator so the scan ends after CMD4. */
    adcConfig.sConvCmdConfig[4].u32DisableCommand = true;

    (void)DUALADC_Init(DEMO_DUALADC_BASE, &adcConfig);

    /* Run full calibration: offset, optional high-speed, and gain. The ADC is already
     * enabled by DUALADC_Init (config.bEnableAdc defaults to true). */
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
    edma_channel_transfer_config_t transferConfig;
    edma_config_t dmaConfig;

    /*
     * Configure a peripheral-to-memory transfer. The DualADC conversion results sit in
     * consecutive 32-bit RSLT registers (RSLT[0]..RSLT[DEMO_DUALADC_RESULT_COUNT-1]). On
     * each end-of-scan request the eDMA moves all DEMO_DUALADC_RESULT_COUNT words into the
     * destination buffer. One request carries the whole buffer (minor loop = total bytes).
     */
    EDMA_GetChannelDefaultTransferConfig(&transferConfig,
                                         (uint32_t)(uint8_t *)(&DEMO_DUALADC_BASE->RSLT[0]),
                                         (uint32_t)(uint8_t *)g_DualAdcResult, sizeof(g_DualAdcResult),
                                         sizeof(g_DualAdcResult), kEDMA_ChannelTransferWidth32Bits,
                                         kEDMA_ChannelTransferMemoryToMemory);

    /* After each transfer, reset source and destination back to the first entry. */
    transferConfig.i32SrcMajorLoopOffset = -(int32_t)sizeof(g_DualAdcResult);
    transferConfig.i32DstMajorLoopOffset = -(int32_t)sizeof(g_DualAdcResult);

    /* Interrupt once the major loop (all results) is complete. */
    transferConfig.u16EnabledInterruptMask = kEDMA_ChannelMajorLoopCompleteInterruptEnable;

    /* Initialize the eDMA with the default configuration and the channel transfer config. */
    EDMA_GetDefaultConfig(&dmaConfig);
    dmaConfig.psChannelTransferConfig[DEMO_DMA_CHANNEL_0] = &transferConfig;
    EDMA_Init(DEMO_DMA_BASEADDR, &dmaConfig);

    /* Route the DualADC end-of-scan request to the eDMA channel through the integrated mux. */
    EDMA_SetChannelMux(DEMO_DMA_BASEADDR, DEMO_DMA_CHANNEL_0, DEMO_DMA_REQUEST);

    (void)EnableIRQWithPriority(DEMO_DMA_IRQ, DEMO_DMA_IRQ_PRIO);

    /*
     * Enable hardware service requests for this channel. Once enabled, the DualADC
     * end-of-scan peripheral request can initiate an eDMA channel service request.
     */
    EDMA_EnableChannelRequest(DEMO_DMA_BASEADDR, DEMO_DMA_CHANNEL_0, true);
}
