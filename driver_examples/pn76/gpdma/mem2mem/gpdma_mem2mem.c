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

volatile bool g_Transfer_Done = false;

/*******************************************************************************
 * Code
 ******************************************************************************/

/* GPDMA IRQ handler */
void GPDMA_IRQHandler(void)
{
    if (GPDMA_CheckChannelInterrupts(DEMO_DMA, DEMO_DMA_CHANNEL) == kStatus_Success)
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

    PRINTF("GPDMA memory to memory example begin.\r\n\r\n");

    /* Configure GPDMA channel for one shot transfer */
    GPDMA_Init(DEMO_DMA);
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

    GPDMA_SetTransferConfig(DEMO_DMA, DEMO_DMA_CHANNEL, &transferConfig);
    GPDMA_EnableChannel(DEMO_DMA, DEMO_DMA_CHANNEL, true);

    /* Wait for GPDMA transfer finish */
    while (g_Transfer_Done != true)
    {
    }

    if (0 == memcmp(s_srcBuf, s_destBuf, sizeof(s_srcBuf)))
    {
        PRINTF("\r\n\r\nGPDMA memory to memory example succeed.\r\n\r\n");
    }
    else
    {
        PRINTF("\r\n\r\nGPDMA memory to memory example failed.\r\n\r\n");

        DEMO_DumpArray("s_srcBuf", (uint8_t *)s_srcBuf, sizeof(s_srcBuf));
        DEMO_DumpArray("s_destBuf", (uint8_t *)s_destBuf, sizeof(s_destBuf));
    }

    while (1)
    {
    }
}
