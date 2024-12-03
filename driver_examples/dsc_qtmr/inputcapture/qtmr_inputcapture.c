/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#if defined(FSL_FEATURE_SOC_DMAMUX_COUNT) && FSL_FEATURE_SOC_DMAMUX_COUNT
#include "fsl_dmamux.h"
#endif
#if defined(FSL_FEATURE_SOC_EDMA_COUNT) && FSL_FEATURE_SOC_EDMA_COUNT
#include "fsl_edma.h"
#endif
#if defined(FSL_FEATURE_SOC_DMA_COUNT) && FSL_FEATURE_SOC_DMA_COUNT
#include "fsl_dma.h"
#endif
#include "fsl_qtmr.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#ifndef EDMA_IRQ_PRIO
#define EDMA_IRQ_PRIO 0
#endif
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool g_bTransferDone = false;
uint16_t g_u16CaptureValue;

/*******************************************************************************
 * Code
 ******************************************************************************/
/* EDMA transfer channel IRQ handler */
void DMA_Channel_IRQHandler(void)
{
#if defined(FSL_FEATURE_SOC_EDMA_COUNT) && FSL_FEATURE_SOC_EDMA_COUNT
    if ((EDMA_GetChannelStatusFlags(DEMO_EDMA_BASEADDR, DEMO_EDMA_CHANNEL) &
         kEDMA_ChannelStatusMajorLoopCompleteFlag) != 0U)
    {
        EDMA_ClearChannelStatusFlags(DEMO_EDMA_BASEADDR, DEMO_EDMA_CHANNEL, kEDMA_ChannelStatusMajorLoopCompleteFlag);
        g_bTransferDone = true;
    }
#else
    if ((DMA_GetChannelStatusFlags(DEMO_DMA_BASEADDR, DEMO_DMA_CHANNEL) & kDMA_ChannelTransactionsDoneFlag) != 0U)
    {
        DMA_ClearChannelDoneStatus(DEMO_DMA_BASEADDR, DEMO_DMA_CHANNEL);
        g_bTransferDone = true;
    }
#endif
}

/*!
 * @brief Main function
 */
int main(void)
{
    uint32_t count = 0;
    qtmr_channel_config_t sChannelConfig;
    qtmr_config_t sQtmrConfig = {0};

    /* Fill QTMR configure structure */
    sQtmrConfig.psChannelConfig[BOARD_QTMR_CHANNEL] = &sChannelConfig;

    /* Board pin, clock, debug console init */
    BOARD_InitHardware();
#if defined(FSL_FEATURE_SOC_EDMA_COUNT) && FSL_FEATURE_SOC_EDMA_COUNT
    edma_config_t sEdmaConfig;
    edma_channel_transfer_config_t sTransferConfig;

    /* Configure DMAMUX */
    DMAMUX_ConnectChannelToTriggerSource(EXAMPLE_QTMR_DMA_MUX, DEMO_DMAMUX_CHANNEL, QTMR_EDMA_REQUEST_CMPLD_SOURCE);
    EDMA_GetChannelDefaultTransferConfig(&sTransferConfig,
                                         TMR_GetCaptureRegAddr(BOARD_QTMR_BASEADDR, BOARD_QTMR_CHANNEL),
                                         (uint32_t)(uint8_t *)&g_u16CaptureValue, 2, 2,
                                         kEDMA_ChannelTransferWidth16Bits, kEDMA_ChannelTransferPeripheralToMemory);
    sTransferConfig.u16EnabledInterruptMask    = kEDMA_ChannelMajorLoopCompleteInterruptEnable;
    sTransferConfig.i16DstOffsetOfEachTransfer = 0U;
    EDMA_GetDefaultConfig(&sEdmaConfig);
    sEdmaConfig.psChannelTransferConfig[DEMO_EDMA_CHANNEL] = &sTransferConfig;
    EDMA_Init(DEMO_EDMA_BASEADDR, &sEdmaConfig);

    /* Enable EDMA channel IRQ */
    EnableIRQWithPriority(DEMO_EDMA_CHANNEL_IRQn, EDMA_IRQ_PRIO);
#else
    dma_config_t sDmaConfig;
    dma_channel_transfer_config_t sTransferConfig;
    memset(&sDmaConfig, 0, sizeof(sDmaConfig));

    /* DMA channel Transfer Configure */
    DMA_GetChannelDefaultTransferConfig(&sTransferConfig,
                                        TMR_GetCaptureRegAddr(BOARD_QTMR_BASEADDR, BOARD_QTMR_CHANNEL),
                                        (uint32_t)(uint8_t *)&g_u16CaptureValue, 2, kDMA_ChannelTransferWidth16Bits,
                                        kDMA_ChannelTransferPeripheralToMemory);
    sTransferConfig.bEnableInterrupt                = true;
    sTransferConfig.bEnablAutoStopPeripheralRequest = true;
    sTransferConfig.eEnableDestIncrement            = kDMA_ChannelAddressFix;

    sDmaConfig.psChannelTransferConfig[DEMO_DMA_CHANNEL] = &sTransferConfig;
    DMA_Init(DEMO_DMA_BASEADDR, &sDmaConfig);
    DMA_SetChannelPeripheralRequest(DEMO_DMA_BASEADDR, DEMO_DMA_CHANNEL, DEMO_DMA_CHANNEL_REQUEST_SOURCE);

    /* Enable DMA channel IRQ */
    EnableIRQWithPriority(DEMO_DMA_CHANNEL_IRQn, EDMA_IRQ_PRIO);
#endif
    PRINTF("\r\n****Input capture example.****\n");
    PRINTF("\r\n****Provide a PWM signal input to the QTMR pin****\n");

    /*
     * psConfig->sInputConfig.ePrimarySource = kQTMR_PrimarySrcIPBusClockDivide2;
     * psConfig->sCountConfig.eCountMode = kQTMR_CountPrimarySrcRiseEdge;
     * psConfig->sCountConfig.eCountLength = kQTMR_CountLengthUntilRollOver;
     * psConfig->sCountConfig.eCountDir = kQTMR_CountDirectionUp;
     * psConfig->sCountConfig.eCountTimes = kQTMR_CountTimesRepeat;
     * psConfig->sCountConfig.eCountLoadMode = kQTMR_CountLoadNormal;
     * psConfig->sOutputConfig.eOutputMode = kQTMR_OutputAssertWhenCountActive;
     * psConfig->sCooperationConfig.bEnableMasterReInit = false;
     * psConfig->sCooperationConfig.bEnableMasterForceOFLAG = false;
     * psConfig->sCooperationConfig.bEnableMasterMode = false;
     * psConfig->eDebugMode = kQTMR_DebugRunNormal;
     * psConfig->u16Comp1 = 0x0U;
     * psConfig->u16EnabledInterruptMask = 0x0U;
     * psConfig->bEnableChannel = false;
     */
    QTMR_GetChannelDefaultConfig(&sChannelConfig);
    /* Setup the primary count source */
    sChannelConfig.sInputConfig.ePrimarySource = QTMR_PRIMARY_SOURCE;
    /* Setup the count mode to trigger mode with re-initialized */
    sChannelConfig.sCountConfig.eCountMode = kQTMR_CountPrimarySrcRiseEdgeSecondarySrcRiseEdgeTrigWithReInit;
    /* Setup the input capture source and mode*/
    sChannelConfig.sInputConfig.eSecondarySource            = QTMR_INPUT_CAPTURE_SOURCE;
    sChannelConfig.sInputConfig.eSecondarySourceCaptureMode = kQTMR_SecondarySrcCaptureFallingEdge;
    /* Enable channel input capture DMA request */
    sChannelConfig.u16EnabledDMAMask = kQTMR_InputEdgeFlagDmaEnable;

    QTMR_Init(BOARD_QTMR_BASEADDR, &sQtmrConfig);

    /* Start the channel to count */
    QTMR_EnableChannels(BOARD_QTMR_BASEADDR, (uint16_t)(1UL << (uint16_t)BOARD_QTMR_CHANNEL));

    while (count < 5 || g_u16CaptureValue == 0)
    {
        while (0 == (QTMR_GetStatusFlags(BOARD_QTMR_BASEADDR, BOARD_QTMR_CHANNEL) & kQTMR_EdgeFlag))
        {
        }
        QTMR_ClearStatusFlags(BOARD_QTMR_BASEADDR, BOARD_QTMR_CHANNEL, kQTMR_EdgeFlag);
        count++;
        g_u16CaptureValue = QTMR_ReadCaptureValue(BOARD_QTMR_BASEADDR, BOARD_QTMR_CHANNEL);
    }

    while (1)
    {
#if defined(FSL_FEATURE_SOC_DMA_COUNT) && FSL_FEATURE_SOC_DMA_COUNT
        DMA_SetChannelTransferSize(DEMO_DMA_BASEADDR, DEMO_DMA_CHANNEL, 2UL);
        DMA_EnableChannelPeripheralRequest(DEMO_DMA_BASEADDR, DEMO_DMA_CHANNEL, true);
#else
        EDMA_EnableChannelRequest(DEMO_EDMA_BASEADDR, DEMO_EDMA_CHANNEL, true);
#endif
        /* Wait for EDMA transfer finish */
        while (g_bTransferDone != true)
        {
        }
        g_bTransferDone = false;

        PRINTF("\r\n The pulse width of the captured PWM is %ld us !",
               COUNT_TO_USEC(g_u16CaptureValue, QTMR_SOURCE_CLOCK));
        PRINTF("\r\n Press any key to trigger the next capture !");
        GETCHAR();
    }
}
