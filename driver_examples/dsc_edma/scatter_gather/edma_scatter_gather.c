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
#define SRC_BUFF_LENGTH 4U
#define DST_BUFF_LENGTH 8U
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
AT_NONCACHEABLE_SECTION_ALIGN(uint32_t g_u32SrcAddr[SRC_BUFF_LENGTH], 4U);
AT_NONCACHEABLE_SECTION_ALIGN(uint32_t g_u32DstAddr[DST_BUFF_LENGTH], 4U);
volatile bool g_bTransferDone = false;
AT_NONCACHEABLE_SECTION_ALIGN(static edma_channel_tcd_t s_edma_tcd[2], sizeof(edma_channel_tcd_t));
/*******************************************************************************
 * Code
 ******************************************************************************/
/* EDMA transfer channel 0 IRQ handler */
void DMA_Channel0_IRQHandler(void)
{
    if ((EDMA_GetChannelStatusFlags(DEMO_EDMA_BASEADDR, DEMO_EDMA_CHANNEL_0) &
         kEDMA_ChannelStatusMajorLoopCompleteFlag) != 0U)
    {
        EDMA_ClearChannelStatusFlags(DEMO_EDMA_BASEADDR, DEMO_EDMA_CHANNEL_0, kEDMA_ChannelStatusMajorLoopCompleteFlag);
        g_bTransferDone = true;
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    uint32_t i = 0;
    edma_channel_transfer_config_t sTransferConfig;
    edma_config_t sConfig;

    BOARD_InitHardware();

    /* Print source buffer */
    PRINTF("EDMA scatter gather example begin.\r\n\r\n");
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

    /* Configure EDMA channel for one shot transfer */
    EDMA_GetChannelDefaultTransferConfig(&sTransferConfig, (uint32_t)(uint8_t *)g_u32SrcAddr,
                                         (uint32_t)(uint8_t *)&g_u32DstAddr[0], sizeof(uint32_t), sizeof(g_u32SrcAddr),
                                         kEDMA_ChannelTransferWidth32Bits, kEDMA_ChannelTransferMemoryToMemory);
    /* Do not disable the channel request after tcd[0] complete, so that tcd[1] will start immediately */
    sTransferConfig.bDisableRequestAfterMajorLoopComplete = false;
    sTransferConfig.psLinkTCD                             = &s_edma_tcd[1];
    sTransferConfig.u16EnabledInterruptMask               = kEDMA_ChannelMajorLoopCompleteInterruptEnable;

    EDMA_ConfigChannelSoftwareTCD(&s_edma_tcd[0], &sTransferConfig);

    EDMA_GetChannelDefaultTransferConfig(&sTransferConfig, (uint32_t)(uint8_t *)g_u32SrcAddr,
                                         (uint32_t)(uint8_t *)&g_u32DstAddr[4], sizeof(uint32_t), sizeof(g_u32SrcAddr),
                                         kEDMA_ChannelTransferWidth32Bits, kEDMA_ChannelTransferMemoryToMemory);
    EDMA_ConfigChannelSoftwareTCD(&s_edma_tcd[1], &sTransferConfig);

    EDMA_InstallChannelSoftwareTCD(DEMO_EDMA_BASEADDR, DEMO_EDMA_CHANNEL_0, &s_edma_tcd[0]);

    /* enable EDMA channel IRQ */
    EnableIRQWithPriority(DEMO_EDMA_CHANNEL_0_IRQn, 1);

    /* start channel */
    EDMA_EnableChannelRequest(DEMO_EDMA_BASEADDR, DEMO_EDMA_CHANNEL_0, true);

    /* Wait for EDMA transfer finish */
    while (g_bTransferDone != true)
    {
    }

    /* Print destination buffer */
    PRINTF("\r\n\r\nEDMA scatter gather example finish.\r\n\r\n");
    PRINTF("Destination Buffer:\r\n");
    for (i = 0; i < DST_BUFF_LENGTH; i++)
    {
        PRINTF("%ld\t", g_u32DstAddr[i]);
    }
    while (1)
    {
    }
}
