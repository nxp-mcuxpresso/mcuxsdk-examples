/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "app.h"
#include "fsl_debug_console.h"
#include "fsl_edma.h"

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
uint32_t g_u32SrcAddr[BUFF_LENGTH] = {0x01, 0x02, 0x03, 0x04};
uint32_t g_u32DstAddr[BUFF_LENGTH] = {0x00, 0x00, 0x00, 0x00};
volatile bool g_Transfer_Done      = false;
/*******************************************************************************
 * Code
 ******************************************************************************/
/* EDMA transfer channel 0 IRQ handler */
#pragma interrupt alignsp saveall
void DEMO_EDMA_CHANNEL_0_IRQ_HANDLER(void)
{
    if ((EDMA_GetChannelStatusFlags(DEMO_EDMA_BASEADDR, DEMO_EDMA_CHANNEL_0) &
         kEDMA_ChannelStatusMajorLoopCompleteFlag) != 0U)
    {
        EDMA_ClearChannelStatusFlags(DEMO_EDMA_BASEADDR, DEMO_EDMA_CHANNEL_0, kEDMA_ChannelStatusMajorLoopCompleteFlag);
        g_Transfer_Done = true;
    }
}
#pragma interrupt off

/*!
 * @brief Main function
 */
int main(void)
{
    uint32_t i = 0;
    edma_channel_transfer_config_t sTransferConfig;
    edma_config_t sConfig;

    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

    /* Print source buffer */
    PRINTF("EDMA memory to memory example begin.\r\n\r\n");
    PRINTF("Destination Buffer:\r\n");
    for (i = 0; i < BUFF_LENGTH; i++)
    {
        PRINTF("%ld\t", g_u32DstAddr[i]);
    }

    /* Configure EDMA channel for one shot transfer */
    EDMA_GetChannelDefaultTransferConfig(&sTransferConfig, (uint32_t)(uint8_t *)g_u32SrcAddr,
                                         (uint32_t)(uint8_t *)g_u32DstAddr, sizeof(g_u32SrcAddr), sizeof(g_u32SrcAddr),
                                         kEDMA_ChannelTransferWidth32Bits, kEDMA_ChannelTransferMemoryToMemory);
    sTransferConfig.u16EnabledInterruptMask = kEDMA_ChannelMajorLoopCompleteInterruptEnable;

    EDMA_GetDefaultConfig(&sConfig);
    sConfig.psChannelTransferConfig[DEMO_EDMA_CHANNEL_0] = &sTransferConfig;
    EDMA_Init(DEMO_EDMA_BASEADDR, &sConfig);
    /* enable EDMA channel IRQ */
    EnableIRQWithPriority(DEMO_EDMA_CHANNEL_0_IRQn, 1);

    EDMA_SoftwareTriggerChannelStart(DEMO_EDMA_BASEADDR, DEMO_EDMA_CHANNEL_0);

    /* Wait for EDMA transfer finish */
    while (g_Transfer_Done != true)
    {
    }

    /* Print destination buffer */
    PRINTF("\r\n\r\nEDMA memory to memory example finish.\r\n\r\n");
    PRINTF("Destination Buffer:\r\n");
    for (i = 0; i < BUFF_LENGTH; i++)
    {
        PRINTF("%ld\t", g_u32DstAddr[i]);
    }
    while (1)
    {
    }
}
