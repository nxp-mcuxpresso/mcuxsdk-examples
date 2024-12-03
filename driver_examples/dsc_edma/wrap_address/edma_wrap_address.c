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
#define BUFF_LENGTH 4 * 2U
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
AT_NONCACHEABLE_SECTION_ALIGN(uint32_t g_u32SrcAddr[BUFF_LENGTH], 16U);
AT_NONCACHEABLE_SECTION_ALIGN(uint32_t g_u32DstAddr[BUFF_LENGTH], 16U);
volatile bool g_bTransferDone = false;
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
    PRINTF("EDMA wrap address example begin.\r\n\r\n");
    PRINTF("Destination Buffer:\r\n");
    for (i = 0; i < BUFF_LENGTH; i++)
    {
        PRINTF("%ld\t", g_u32DstAddr[i]);
        g_u32SrcAddr[i] = i;
    }

    /* Configure DMAMUX */
    DMAMUX_ConnectChannelToTriggerSource(DEMO_DMAMUX_BASEADDR, DEMO_DMAMUX_CHANNEL_0, DEMO_DMAMUX_ALWAYS_ON_CHANNEL);

    /* Configure EDMA channel for one shot transfer */
    EDMA_GetChannelDefaultTransferConfig(&sTransferConfig, (uint32_t)(uint8_t *)g_u32SrcAddr,
                                         (uint32_t)(uint8_t *)g_u32DstAddr, sizeof(uint32_t), sizeof(g_u32SrcAddr),
                                         kEDMA_ChannelTransferWidth32Bits, kEDMA_ChannelTransferMemoryToMemory);

    sTransferConfig.eSrcAddrModulo          = kEDMA_ChannelModulo16bytes;
    sTransferConfig.u16EnabledInterruptMask = kEDMA_ChannelMajorLoopCompleteInterruptEnable;

    EDMA_GetDefaultConfig(&sConfig);
    sConfig.psChannelTransferConfig[DEMO_EDMA_CHANNEL_0] = &sTransferConfig;
    EDMA_Init(DEMO_EDMA_BASEADDR, &sConfig);

    /* enable EDMA channel IRQ */
    EnableIRQWithPriority(DEMO_EDMA_CHANNEL_0_IRQn, 1);

    EDMA_EnableChannelRequest(DEMO_EDMA_BASEADDR, DEMO_EDMA_CHANNEL_0, true);

    /* Wait for EDMA transfer finish */
    while (g_bTransferDone != true)
    {
    }

    /* Print destination buffer */
    PRINTF("\r\n\r\nEDMA wrap address example finish.\r\n\r\n");
    PRINTF("Destination Buffer:\r\n");
    for (i = 0; i < BUFF_LENGTH; i++)
    {
        PRINTF("%ld\t", g_u32DstAddr[i]);
    }
    while (1)
    {
    }
}
