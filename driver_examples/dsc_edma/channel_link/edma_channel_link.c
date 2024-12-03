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
#define BUFF_LENGTH 4U
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
AT_NONCACHEABLE_SECTION_ALIGN(uint32_t g_u32SrcAddr[BUFF_LENGTH], 4U);
AT_NONCACHEABLE_SECTION_ALIGN(uint32_t g_u32DstAddr0[BUFF_LENGTH], 4U);
AT_NONCACHEABLE_SECTION_ALIGN(uint32_t g_u32DstAddr1[BUFF_LENGTH], 4U);
AT_NONCACHEABLE_SECTION_ALIGN(uint32_t g_u32DstAddr2[BUFF_LENGTH], 4U);

volatile bool g_bTransferDone0 = false;
/*******************************************************************************
 * Code
 ******************************************************************************/
void DMA_Channel0_IRQHandler(void)
{
    if ((EDMA_GetChannelStatusFlags(DEMO_EDMA_BASEADDR, DEMO_EDMA_CHANNEL_0) &
         kEDMA_ChannelStatusMajorLoopCompleteFlag) != 0U)
    {
        EDMA_ClearChannelStatusFlags(DEMO_EDMA_BASEADDR, DEMO_EDMA_CHANNEL_0, kEDMA_ChannelStatusMajorLoopCompleteFlag);
        g_bTransferDone0 = true;
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    uint32_t i = 0;
    edma_channel_transfer_config_t sTransferConfig0;
    edma_channel_transfer_config_t sTransferConfig1;
    edma_channel_transfer_config_t sTransferConfig2;

    edma_config_t sConfig;

    BOARD_InitHardware();

    /* source buffer initialization */
    g_u32SrcAddr[0] = 1U;
    g_u32SrcAddr[1] = 2U;
    g_u32SrcAddr[2] = 3U;
    g_u32SrcAddr[3] = 4U;

    /* Print source buffer */
    PRINTF("EDMA channel link example begin.\r\n\r\n");
    PRINTF("Destination Buffer 0:\r\n");
    for (i = 0; i < BUFF_LENGTH; i++)
    {
        PRINTF("%ld\t", g_u32DstAddr0[i]);
    }
    PRINTF("\r\nDestination Buffer 1:\r\n");
    for (i = 0; i < BUFF_LENGTH; i++)
    {
        PRINTF("%ld\t", g_u32DstAddr1[i]);
    }
    PRINTF("\r\nDestination Buffer 2:\r\n");
    for (i = 0; i < BUFF_LENGTH; i++)
    {
        PRINTF("%ld\t", g_u32DstAddr2[i]);
    }

    /* Configure DMAMUX */
    DMAMUX_ConnectChannelToTriggerSource(DEMO_DMAMUX_BASEADDR, DEMO_DMAMUX_CHANNEL_0, DEMO_DMAMUX_ALWAYS_ON_CHANNEL);

    /* Configure EDMA channel 0 */
    EDMA_GetChannelDefaultTransferConfig(&sTransferConfig0, (uint32_t)(uint8_t *)g_u32SrcAddr,
                                         (uint32_t)(uint8_t *)g_u32DstAddr0, sizeof(uint32_t), sizeof(g_u32SrcAddr),
                                         kEDMA_ChannelTransferWidth32Bits, kEDMA_ChannelTransferMemoryToMemory);
    /* minor loop link channel 1 */
    sTransferConfig0.bEnableChannelMinorLoopLink = true;
    sTransferConfig0.bEnableChannelMajorLoopLink = true;
    sTransferConfig0.eMinorLoopLinkChannel       = DEMO_EDMA_CHANNEL_1;
    sTransferConfig0.eMajorLoopLinkChannel       = DEMO_EDMA_CHANNEL_2;
    sTransferConfig0.u16EnabledInterruptMask     = kEDMA_ChannelMajorLoopCompleteInterruptEnable;

    /* Configure EDMA channel 1 */
    EDMA_GetChannelDefaultTransferConfig(&sTransferConfig1, (uint32_t)(uint8_t *)g_u32SrcAddr,
                                         (uint32_t)(uint8_t *)g_u32DstAddr1, sizeof(uint32_t), sizeof(g_u32DstAddr1),
                                         kEDMA_ChannelTransferWidth32Bits, kEDMA_ChannelTransferMemoryToMemory);
    /* Configure EDMA channel 2 */
    EDMA_GetChannelDefaultTransferConfig(&sTransferConfig2, (uint32_t)(uint8_t *)g_u32SrcAddr,
                                         (uint32_t)(uint8_t *)g_u32DstAddr2, sizeof(uint32_t), sizeof(g_u32DstAddr2),
                                         kEDMA_ChannelTransferWidth32Bits, kEDMA_ChannelTransferMemoryToMemory);

    EDMA_GetDefaultConfig(&sConfig);
    sConfig.psChannelTransferConfig[DEMO_EDMA_CHANNEL_0] = &sTransferConfig0;
    sConfig.psChannelTransferConfig[DEMO_EDMA_CHANNEL_1] = &sTransferConfig1;
    sConfig.psChannelTransferConfig[DEMO_EDMA_CHANNEL_2] = &sTransferConfig2;

    EDMA_Init(DEMO_EDMA_BASEADDR, &sConfig);

    /* enable EDMA channel IRQ */
    EnableIRQWithPriority(DEMO_EDMA_CHANNEL_0_IRQn, 1);

    EDMA_EnableChannelRequest(DEMO_EDMA_BASEADDR, DEMO_EDMA_CHANNEL_0, true);
    /* Wait for EDMA transfer finish */
    while (g_bTransferDone0 != true)
    {
    }

    /* Print destination buffer */
    PRINTF("\r\n\r\nEDMA channel link example finish.\r\n\r\n");
    PRINTF("Destination Buffer 0 :\r\n");
    for (i = 0; i < BUFF_LENGTH; i++)
    {
        PRINTF("%ld\t", g_u32DstAddr0[i]);
    }

    PRINTF("\r\nDestination Buffer 1 :\r\n");
    for (i = 0; i < BUFF_LENGTH; i++)
    {
        PRINTF("%ld\t", g_u32DstAddr1[i]);
    }

    PRINTF("\r\nDestination Buffer 2 :\r\n");
    for (i = 0; i < BUFF_LENGTH; i++)
    {
        PRINTF("%ld\t", g_u32DstAddr2[i]);
    }

    while (1)
    {
    }
}
