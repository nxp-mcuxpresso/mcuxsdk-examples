/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
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
edma_handle_t g_EDMA_Handle;
volatile bool g_Transfer_Done = false;

/*******************************************************************************
 * Code
 ******************************************************************************/

/* User callback function for EDMA transfer. */
void EDMA_Callback(edma_handle_t *handle, void *param, bool transferDone, uint32_t tcds)
{
    if (transferDone)
    {
        g_Transfer_Done = true;
    }
}

AT_NONCACHEABLE_SECTION_INIT(uint32_t srcAddr[BUFF_LENGTH])  = {0x01, 0x02, 0x03, 0x04};
AT_NONCACHEABLE_SECTION_INIT(uint32_t destAddr[BUFF_LENGTH]) = {0x00, 0x00, 0x00, 0x00};

/*!
 * @brief Main function
 */
int main(void)
{
    uint32_t i;
    edma_transfer_config_t transferConfig;
    edma_config_t userConfig;

    BOARD_InitHardware();
    /* Print destination buffer */
    PRINTF("EDMA memory to memory transfer example begin.\r\n\r\n");
    PRINTF("Destination Buffer:\r\n");
    for (i = 0; i < BUFF_LENGTH; i++)
    {
        PRINTF("%d\t", destAddr[i]);
    }
    /* Configure EDMA one shot transfer */
    /*
     * userConfig.enableMasterIdReplication = true;
     * userConfig.enableHaltOnError = true;
     * userConfig.enableRoundRobinArbitration = false;
     * userConfig.enableDebugMode = false;
     * userConfig.enableBufferedWrites = false;
     */
    EDMA_GetDefaultConfig(&userConfig);
    EDMA_Init(EXAMPLE_DMA_BASEADDR, &userConfig);
    EDMA_CreateHandle(&g_EDMA_Handle, EXAMPLE_DMA_BASEADDR, EXAMPLE_DMA_CHANNEL);
    EDMA_SetCallback(&g_EDMA_Handle, EDMA_Callback, NULL);
    EDMA_PrepareTransfer(&transferConfig, srcAddr, sizeof(srcAddr[0]), destAddr, sizeof(destAddr[0]), sizeof(srcAddr),
                         sizeof(srcAddr), kEDMA_MemoryToMemory);
    EDMA_SubmitTransfer(&g_EDMA_Handle, &transferConfig);

    /* Trigger transfer start */
    EDMA_TriggerChannelStart(EXAMPLE_DMA_BASEADDR, EXAMPLE_DMA_CHANNEL);

    /* Wait for EDMA transfer finish */
    while (g_Transfer_Done != true)
    {
    }
    /* Print destination buffer */
    PRINTF("\r\n\r\nEDMA memory to memory transfer example finish.\r\n\r\n");
    PRINTF("Destination Buffer:\r\n");
    for (i = 0; i < BUFF_LENGTH; i++)
    {
        PRINTF("%d\t", destAddr[i]);
    }
    while (1)
    {
    }
}
