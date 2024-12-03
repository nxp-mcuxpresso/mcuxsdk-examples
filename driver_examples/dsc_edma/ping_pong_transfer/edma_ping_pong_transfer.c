
/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "app.h"
#include "fsl_debug_console.h"
#include "fsl_edma.h"
#include "fsl_dmamux.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define SRC_BUFF_LENGTH          4U
#define DST_BUFF_LENGTH          8U
#define DEMO_EDMA_TRANSFER_COUNT 4U
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
AT_NONCACHEABLE_SECTION_ALIGN(uint32_t g_u32SrcAddr[SRC_BUFF_LENGTH], 4U);
AT_NONCACHEABLE_SECTION_ALIGN(uint32_t g_u32DstAddr[DST_BUFF_LENGTH], 4U);
volatile bool g_bTransferDone        = false;
volatile uint32_t g_u32TransferCount = 0U;
AT_NONCACHEABLE_SECTION_ALIGN(static edma_channel_tcd_t s_edma_tcd[2], sizeof(edma_channel_tcd_t));
static edma_handle_t s_edma_handle;
/*******************************************************************************
 * Code
 ******************************************************************************/
/* EDMA transfer channel 0 IRQ handler */
void DMA_Channel0_Callback(edma_handle_t *psHandle, void *pUserData, bool bTransferDone, uint32_t u32Tcds);
void DMA_Channel0_Callback(edma_handle_t *psHandle, void *pUserData, bool bTransferDone, uint32_t u32Tcds)
{
    if (g_u32TransferCount++ > DEMO_EDMA_TRANSFER_COUNT)
    {
        g_bTransferDone = true;
        EDMA_TransferStop(psHandle);
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    uint32_t i = 0;
    edma_channel_transfer_config_t sTransferConfig[2U];
    edma_config_t sConfig;

    BOARD_InitHardware();

    /* Print source buffer */
    PRINTF("EDMA ping pong transfer example begin.\r\n\r\n");
    PRINTF("Destination Buffer:\r\n");
    for (i = 0; i < DST_BUFF_LENGTH; i++)
    {
        PRINTF("%ld\t", g_u32DstAddr[i]);
    }
    /* source buffer initialization */
    g_u32SrcAddr[0] = 0U;
    g_u32SrcAddr[1] = 1U;
    g_u32SrcAddr[2] = 2U;
    g_u32SrcAddr[3] = 3U;
    /* Configure DMAMUX */
    DMAMUX_ConnectChannelToTriggerSource(DEMO_DMAMUX_BASEADDR, DEMO_DMAMUX_CHANNEL_0, DEMO_DMAMUX_ALWAYS_ON_CHANNEL);

    EDMA_GetDefaultConfig(&sConfig);
    EDMA_Init(DEMO_EDMA_BASEADDR, &sConfig);

    EDMA_TransferCreateHandle(DEMO_EDMA_BASEADDR, &s_edma_handle, DEMO_EDMA_CHANNEL_0, s_edma_tcd, 2U,
                              DMA_Channel0_Callback, NULL);

    EDMA_GetChannelDefaultTransferConfig(&sTransferConfig[0], (uint32_t)(uint8_t *)g_u32SrcAddr,
                                         (uint32_t)(uint8_t *)&g_u32DstAddr[0], sizeof(uint32_t), sizeof(g_u32SrcAddr),
                                         kEDMA_ChannelTransferWidth32Bits, kEDMA_ChannelTransferMemoryToMemory);
    /* Do not disable the channel request after tcd[0] complete, so that tcd[1] will start immediately */
    sTransferConfig[0].bDisableRequestAfterMajorLoopComplete = false;
    sTransferConfig[0].u16EnabledInterruptMask               = kEDMA_ChannelMajorLoopCompleteInterruptEnable;

    EDMA_GetChannelDefaultTransferConfig(&sTransferConfig[1], (uint32_t)(uint8_t *)g_u32SrcAddr,
                                         (uint32_t)(uint8_t *)&g_u32DstAddr[4], sizeof(uint32_t), sizeof(g_u32SrcAddr),
                                         kEDMA_ChannelTransferWidth32Bits, kEDMA_ChannelTransferMemoryToMemory);
    /* Do not disable the channel request after tcd[1] complete, so that tcd[0] will start immediately */
    sTransferConfig[1].bDisableRequestAfterMajorLoopComplete = false;
    sTransferConfig[1].u16EnabledInterruptMask               = kEDMA_ChannelMajorLoopCompleteInterruptEnable;

    EDMA_TransferSubmitLoopTransfer(&s_edma_handle, sTransferConfig, 2U);

    /* start channel */
    EDMA_TransferStart(&s_edma_handle);

    /* Wait for EDMA transfer finish */
    while (g_bTransferDone != true)
    {
    }

    /* Print destination buffer */
    PRINTF("\r\n\r\nEDMA ping pong transfer example finish.\r\n\r\n");
    PRINTF("Destination Buffer:\r\n");
    for (i = 0; i < DST_BUFF_LENGTH; i++)
    {
        PRINTF("%ld\t", g_u32DstAddr[i]);
    }
    while (1)
    {
    }
}
