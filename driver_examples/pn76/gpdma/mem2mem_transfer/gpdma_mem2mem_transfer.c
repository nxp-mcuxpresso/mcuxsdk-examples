/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_gpdma.h"
#include "board.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BUFFER_LENGTH 16U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
AT_NONCACHEABLE_SECTION(uint32_t s_srcBuf[BUFFER_LENGTH]);
AT_NONCACHEABLE_SECTION(uint32_t s_destBuf[BUFFER_LENGTH]);
gpdma_handle_t g_DMA_Handle;
volatile bool g_Transfer_Done = false;

/*******************************************************************************
 * Code
 ******************************************************************************/

/* User callback function for EDMA transfer. */
void DMA_Callback(gpdma_handle_t *handle, void *userData, uint32_t status)
{
    if (0UL != (status & kGPDMA_StatusInterruptTerminalCountRequest))
    {
        g_Transfer_Done = true;
    }
}

void DEMO_DumpArray(const char *arrayName, const uint8_t *data, uint32_t len)
{
    PRINTF("\r\n%s:\r\n", arrayName);

    for (uint32_t i = 0U; i < len;)
    {
        PRINTF("0x%02X ", data[i]);

        if ((++i) % 16U == 0)
        {
            PRINTF("\r\n");
        }
    }

    PRINTF("\r\n");
}

/*!
 * @brief Main function
 */
int main(void)
{
    uint32_t i;
    gpdma_transfer_config_t transferConfig;

    BOARD_InitHardware();

    /* Initializes data. */
    memset(s_destBuf, 0, sizeof(s_destBuf));
    for (i = 0U; i < BUFFER_LENGTH; i++)
    {
        s_srcBuf[i] = i;
    }

    PRINTF("GPDMA memory to memory transfer example begin.\r\n\r\n");

    /* Configure GPDMA channel for one shot transfer */
    GPDMA_Init(DEMO_DMA);
    GPDMA_CreateHandle(&g_DMA_Handle, DEMO_DMA, DEMO_DMA_CHANNEL);
    GPDMA_SetCallback(&g_DMA_Handle, DMA_Callback, NULL);

    GPDMA_PrepareTransferConfig(&transferConfig,                 /* config. */
                                s_srcBuf,                        /* srcAddr */
                                kGPDMA_BurstSize1,               /* srcBurstSize */
                                kGPDMA_TransferWidth4Bytes,      /* srcTransferWidth */
                                s_destBuf,                       /* destAddr */
                                kGPDMA_BurstSize1,               /* destBurstSize */
                                kGPDMA_TransferWidth4Bytes,      /* destTransferWidth */
                                sizeof(s_srcBuf),                /* totalBytes */
                                kGPDMA_TransferM2MControllerDma, /* transferType */
                                0UL                              /* linkListItem */
    );
    GPDMA_SubmitTransfer(&g_DMA_Handle, &transferConfig);
    GPDMA_StartTransfer(&g_DMA_Handle);

    /* Wait for GPDMA transfer finish */
    while (g_Transfer_Done != true)
    {
    }

    if (0 == memcmp(s_srcBuf, s_destBuf, sizeof(s_srcBuf)))
    {
        PRINTF("\r\n\r\nGPDMA memory to memory transfer example succeed.\r\n\r\n");
    }
    else
    {
        PRINTF("\r\n\r\nGPDMA memory to memory transfer example failed.\r\n\r\n");

        DEMO_DumpArray("s_srcBuf", (uint8_t *)s_srcBuf, sizeof(s_srcBuf));
        DEMO_DumpArray("s_destBuf", (uint8_t *)s_destBuf, sizeof(s_destBuf));
    }

    while (1)
    {
    }
}
