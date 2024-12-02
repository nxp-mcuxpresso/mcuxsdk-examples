/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "app.h"
#include "fsl_debug_console.h"
#include "fsl_dma.h"

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
uint32_t g_u32SrcAddr[BUFF_LENGTH]  = {0x01, 0x02, 0x03, 0x04};
uint32_t g_u32DestAddr[BUFF_LENGTH] = {0x00, 0x00, 0x00, 0x00};
dma_handle_t g_DMA_Handle;
volatile bool g_Transfer_Done = false;
/*******************************************************************************
 * Code
 ******************************************************************************/

/* User callback function for DMA transfer. */
static void DMA_Callback(dma_handle_t *psHandle, void *pUserData)
{
    g_Transfer_Done = true;
}

/* Main function. Excuate DMA transfer with transactional APIs. */
int main(void)
{
    uint32_t i = 0;
    dma_channel_transfer_config_t transferConfig;
    dma_config_t dmaConfig;

    memset(&dmaConfig, 0U, sizeof(dmaConfig));

    BOARD_InitHardware();
    /* Print source buffer */
    PRINTF("DMA memory to memory transfer example begin.\r\n\r\n");
    PRINTF("Destination Buffer:\r\n");
    for (i = 0; i < BUFF_LENGTH; i++)
    {
        PRINTF("%ld\t", g_u32DestAddr[i]);
    }
    /* Configure DMA one shot transfer */
    DMA_Init(DEMO_DMA_BASEADDR, &dmaConfig);
    DMA_TransferChannelCreateHandle(DEMO_DMA_BASEADDR, &g_DMA_Handle, DEMO_DMA_CHANNEL_0, DMA_Callback, NULL);
    DMA_GetChannelDefaultTransferConfig(&transferConfig, (uint32_t)(uint8_t *)g_u32SrcAddr,
                                        (uint32_t)(uint8_t *)g_u32DestAddr, sizeof(g_u32SrcAddr[0]) * BUFF_LENGTH,
                                        kDMA_ChannelTransferWidth32Bits, kDMA_ChannelTransferMemoryToMemory);
    DMA_TransferChannelSubmit(&g_DMA_Handle, &transferConfig);
    DMA_TransferChannelStart(&g_DMA_Handle, false);
    /* Wait for DMA transfer finish */
    while (g_Transfer_Done != true)
    {
    }
    /* Print destination buffer */
    PRINTF("\r\n\r\nDMA memory to memory transfer example finish.\r\n\r\n");
    PRINTF("Destination Buffer:\r\n");
    for (i = 0; i < BUFF_LENGTH; i++)
    {
        PRINTF("%ld\t", g_u32DestAddr[i]);
    }
    while (1)
    {
    }
}
