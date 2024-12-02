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
#define SRC_BUFF_LENGTH 4
#define DST_BUFF_LENGTH 4 * 2 * 2
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
AT_NONCACHEABLE_SECTION_ALIGN(uint32_t g_u32SrcAddr[SRC_BUFF_LENGTH], 16U);
AT_NONCACHEABLE_SECTION_ALIGN(uint32_t g_u32DstAddr[DST_BUFF_LENGTH], 16U);
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
    PRINTF("EDMA address offset example begin.\r\n\r\n");
    PRINTF("Destination Buffer:\r\n");
    for (i = 0; i < DST_BUFF_LENGTH; i++)
    {
        PRINTF("%ld\t", g_u32DstAddr[i]);
    }

    g_u32SrcAddr[0] = 1;
    g_u32SrcAddr[1] = 2;
    g_u32SrcAddr[2] = 3;
    g_u32SrcAddr[3] = 4;

    /* Configure DMAMUX */
    DMAMUX_ConnectChannelToTriggerSource(DEMO_DMAMUX_BASEADDR, DEMO_DMAMUX_CHANNEL_0, DEMO_DMAMUX_ALWAYS_ON_CHANNEL);

    /* Configure EDMA channel for one shot transfer */
    EDMA_GetChannelDefaultTransferConfig(&sTransferConfig, (uint32_t)(uint8_t *)g_u32SrcAddr,
                                         (uint32_t)(uint8_t *)g_u32DstAddr, sizeof(uint32_t), sizeof(g_u32SrcAddr),
                                         kEDMA_ChannelTransferWidth32Bits, kEDMA_ChannelTransferMemoryToMemory);

    sTransferConfig.bEnableDstMinorLoopOffset = true;
    sTransferConfig.i32MinorLoopOffset        = 4U; /* minor loop offset 4byte */

    sTransferConfig.i32DstMajorLoopOffset = 8U;  /* major loop offset 8 byte */
    sTransferConfig.i32SrcMajorLoopOffset = -16; /* wrap back when major loop done */

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

    g_bTransferDone = false;

    EDMA_EnableChannelRequest(DEMO_EDMA_BASEADDR, DEMO_EDMA_CHANNEL_0, true);

    /* Wait for EDMA transfer finish */
    while (g_bTransferDone != true)
    {
    }

    /* Print destination buffer */
    PRINTF("\r\n\r\nEDMA address offset example finish.\r\n\r\n");
    PRINTF("Destination Buffer:\r\n");
    for (i = 0; i < DST_BUFF_LENGTH; i++)
    {
        PRINTF("%ld\t", g_u32DstAddr[i]);
    }
    while (1)
    {
    }
}
