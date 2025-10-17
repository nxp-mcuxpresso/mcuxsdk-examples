/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "app.h"
#include "fsl_debug_console.h"
#include "fsl_ctimer.h"

#if defined(USE_EDMA)
#include "fsl_edma.h"
#elif defined(USE_LPC_DMA)
#include "fsl_dma.h"
#endif
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BUFFER_LENGTH 4U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
AT_NONCACHEABLE_SECTION_INIT(uint32_t srcAddr[BUFFER_LENGTH]) = {0x01, 0x02, 0x03, 0x04};
AT_NONCACHEABLE_SECTION_INIT(uint32_t destAddr[BUFFER_LENGTH]) = {0};
volatile uint32_t g_Transfer_Count = 0U;

#if defined(USE_EDMA)
edma_handle_t g_DMA_Handle;
edma_transfer_config_t g_transferConfig;
edma_config_t g_userConfig;
#elif defined(USE_LPC_DMA)
dma_handle_t g_DMA_Handle;
dma_channel_config_t g_transferConfig;
dma_channel_trigger_t g_triggerConfig;
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/
#if defined(USE_EDMA)
void DMA_Callback(edma_handle_t *handle, void *userData, bool transferDone, uint32_t tcds)
{
    uint32_t i;

    /* Update source buffer when first transfer finish. */
    for (i = 0; i < BUFFER_LENGTH; i++)
    {
        srcAddr[i] += 4U;
    }

    g_Transfer_Count++;
}

void DEMO_DMAConfig(void)
{
    /* Configure EDMA channel for one shot transfer */
    EDMA_GetDefaultConfig(&g_userConfig);
    EDMA_Init(EXAMPLE_DMA_BASEADDR, &g_userConfig);

    EDMA_CreateHandle(&g_DMA_Handle, EXAMPLE_DMA_BASEADDR, DEMO_DMA_CHANNEL);
    EDMA_SetCallback(&g_DMA_Handle, DMA_Callback, NULL);

    /* sizeof(uint32_t): 4 major loop counts */
    EDMA_PrepareTransfer(&g_transferConfig, srcAddr, sizeof(srcAddr[0]), destAddr, sizeof(destAddr[0]),
                         sizeof(uint32_t) * BUFFER_LENGTH, sizeof(srcAddr), kEDMA_MemoryToMemory);

    g_transferConfig.srcMajorLoopOffset = -(int32_t)sizeof(srcAddr);
    g_transferConfig.dstMajorLoopOffset = -(int32_t)sizeof(destAddr);

    EDMA_SubmitTransfer(&g_DMA_Handle, &g_transferConfig);

    EDMA_EnableAutoStopRequest(EXAMPLE_DMA_BASEADDR, DEMO_DMA_CHANNEL, false);

#if defined(FSL_FEATURE_EDMA_HAS_CHANNEL_MUX) && FSL_FEATURE_EDMA_HAS_CHANNEL_MUX && defined(DEMO_DMA_REQUEST_SOURCE)
    EDMA_SetChannelMux(EXAMPLE_DMA_BASEADDR, DEMO_DMA_CHANNEL, DEMO_DMA_REQUEST_SOURCE);
#endif

    EDMA_StartTransfer(&g_DMA_Handle);
}

#elif defined(USE_LPC_DMA)
void DMA_Callback(dma_handle_t *handle, void *userData, bool transferDone, uint32_t tcds)
{
    uint32_t i;

    /* Update source buffer when first transfer finish. */
    for (i = 0; i < BUFFER_LENGTH; i++)
    {
        srcAddr[i] += 4U;
    }

    DMA_SubmitChannelTransfer(&g_DMA_Handle, &g_transferConfig);
    DMA_StartTransfer(&g_DMA_Handle);

    g_Transfer_Count++;
}

void DEMO_DMAConfig(void)
{
    DMA_Init(EXAMPLE_DMA_BASEADDR);

    DMA_CreateHandle(&g_DMA_Handle, EXAMPLE_DMA_BASEADDR, DEMO_DMA_CHANNEL);
    DMA_EnableChannel(EXAMPLE_DMA_BASEADDR, DEMO_DMA_CHANNEL);
    DMA_SetCallback(&g_DMA_Handle, DMA_Callback, NULL);

    g_triggerConfig.type = kDMA_RisingEdgeTrigger;
    g_triggerConfig.wrap = kDMA_NoWrap;
    g_triggerConfig.burst = kDMA_SingleTransfer;

    DMA_PrepareChannelTransfer(&g_transferConfig, srcAddr, destAddr,
                               DMA_CHANNEL_XFER(false, false, true, false, 4, kDMA_AddressInterleave1xWidth,
                                                kDMA_AddressInterleave1xWidth, 16),
                               kDMA_MemoryToMemory, &g_triggerConfig, NULL);
    DMA_SubmitChannelTransfer(&g_DMA_Handle, &g_transferConfig);

    DMA_StartTransfer(&g_DMA_Handle);
}
#endif

/*!
 * @brief Main function
 */
int main(void)
{
    uint32_t i;
    ctimer_config_t ctimerConfig;
    ctimer_match_config_t matchConfig;

    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

    /* Print destination buffer */
    PRINTF("\r\nCTimer trigger DMA memory to memory transfer example begin.\r\n");
    PRINTF("Destination Buffer:\r\n");
    for (i = 0; i < BUFFER_LENGTH; i++)
    {
        destAddr[i] = 0;
        PRINTF("%d\t", destAddr[i]);
    }

    CTIMER_GetDefaultConfig(&ctimerConfig);
    CTIMER_Init(CTIMER, &ctimerConfig);

    matchConfig.enableCounterReset = true;
    matchConfig.enableCounterStop  = false;
    matchConfig.outControl         = kCTIMER_Output_NoAction;
    matchConfig.outPinInitState    = false;
    matchConfig.enableInterrupt    = false;
    matchConfig.matchValue         = CTIMER_CLK_FREQ;
    CTIMER_SetupMatch(CTIMER, kCTIMER_Match_0, &matchConfig);

    DEMO_DMAConfig();

    CTIMER_StartTimer(CTIMER);

    /* Wait for DMA first transfer finish. */
    while (g_Transfer_Count != 1U)
    {
    }
    /* Print destination buffer */
    PRINTF("\r\nCTimer trigger DMA memory to memory transfer example finish.\r\n");
    PRINTF("Destination Buffer:\r\n");
    for (i = 0; i < BUFFER_LENGTH; i++)
    {
        PRINTF("%d\t", destAddr[i]);
    }
    PRINTF("\r\n");

    /* Wait for DMA second transfer finish. */
    while (g_Transfer_Count != 2U)
    {
    }
    for (i = 0; i < BUFFER_LENGTH; i++)
    {
        PRINTF("%d\t", destAddr[i]);
    }

    CTIMER_StopTimer(CTIMER);

    PRINTF("\r\n\r\n");

    while (1)
    {
    }
}