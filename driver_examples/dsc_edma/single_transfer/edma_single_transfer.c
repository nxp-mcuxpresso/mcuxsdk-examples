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
AT_NONCACHEABLE_SECTION_ALIGN(uint32_t g_u32DstAddr[BUFF_LENGTH], 4U);
volatile bool g_bTransferDone = false;
AT_NONCACHEABLE_SECTION_ALIGN(static edma_channel_tcd_t s_edma_tcd[1], sizeof(edma_channel_tcd_t));
static edma_handle_t s_edma_handle;
/*******************************************************************************
 * Code
 ******************************************************************************/
/* EDMA transfer channel 0 IRQ handler */
void DMA_Channel0_Callback(edma_handle_t *psHandle, void *pUserData, bool bTransferDone, uint32_t u32Tcds);
void DMA_Channel0_Callback(edma_handle_t *psHandle, void *pUserData, bool bTransferDone, uint32_t u32Tcds)
{
    if (bTransferDone)
    {
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
    PRINTF("EDMA single transfer example begin.\r\n\r\n");
    PRINTF("Destination Buffer:\r\n");
    for (i = 0; i < BUFF_LENGTH; i++)
    {
        PRINTF("%ld\t", g_u32DstAddr[i]);
        g_u32SrcAddr[i] = i;
    }

    /* Configure DMAMUX */
    DMAMUX_ConnectChannelToTriggerSource(DEMO_DMAMUX_BASEADDR, DEMO_DMAMUX_CHANNEL_0, DEMO_DMAMUX_ALWAYS_ON_CHANNEL);

    EDMA_GetDefaultConfig(&sConfig);
    EDMA_Init(DEMO_EDMA_BASEADDR, &sConfig);

    EDMA_TransferCreateHandle(DEMO_EDMA_BASEADDR, &s_edma_handle, DEMO_EDMA_CHANNEL_0, s_edma_tcd, 1U,
                              DMA_Channel0_Callback, NULL);

    EDMA_GetChannelDefaultTransferConfig(&sTransferConfig, (uint32_t)(uint8_t *)g_u32SrcAddr,
                                         (uint32_t)(uint8_t *)g_u32DstAddr, sizeof(uint32_t), sizeof(g_u32SrcAddr),
                                         kEDMA_ChannelTransferWidth32Bits, kEDMA_ChannelTransferMemoryToMemory);
    sTransferConfig.u16EnabledInterruptMask = kEDMA_ChannelMajorLoopCompleteInterruptEnable;

    EDMA_TransferSubmitSingleTransfer(&s_edma_handle, &sTransferConfig);

    /* start channel */
    EDMA_TransferStart(&s_edma_handle);

    /* Wait for EDMA transfer finish */
    while (g_bTransferDone != true)
    {
    }

    /* Print destination buffer */
    PRINTF("\r\n\r\nEDMA single transfer example finish.\r\n\r\n");
    PRINTF("Destination Buffer:\r\n");
    for (i = 0; i < BUFF_LENGTH; i++)
    {
        PRINTF("%ld\t", g_u32DstAddr[i]);
    }
    while (1)
    {
    }
}
