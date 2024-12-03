/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "fsl_gpdma.h"
#include "board.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BUFFER1_LENGTH 16U
#define BUFFER2_LENGTH 8U
#define BUFFER3_LENGTH 4U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
AT_NONCACHEABLE_SECTION(static uint32_t s_srcBuf1[BUFFER1_LENGTH]);
AT_NONCACHEABLE_SECTION(static uint32_t s_destBuf1[BUFFER1_LENGTH]);
AT_NONCACHEABLE_SECTION(static uint32_t s_srcBuf2[BUFFER2_LENGTH]);
AT_NONCACHEABLE_SECTION(static uint32_t s_destBuf2[BUFFER2_LENGTH]);
AT_NONCACHEABLE_SECTION(static uint32_t s_srcBuf3[BUFFER3_LENGTH]);
AT_NONCACHEABLE_SECTION(static uint32_t s_destBuf3[BUFFER3_LENGTH]);

/*
 * Define two descriptors, one for buffer2, one for buffer3.
 */
AT_NONCACHEABLE_SECTION_ALIGN(static gpdma_descriptor_t s_descriptor[2], 16);

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

void DEMO_InitBuf(void)
{
    uint32_t i;

    memset(s_destBuf1, 0, sizeof(s_destBuf1));
    memset(s_destBuf2, 0, sizeof(s_destBuf2));
    memset(s_destBuf3, 0, sizeof(s_destBuf3));

    for (i = 0U; i < BUFFER1_LENGTH; i++)
    {
        s_srcBuf1[i] = i;
    }

    for (i = 0U; i < BUFFER2_LENGTH; i++)
    {
        s_srcBuf2[i] = i;
    }

    for (i = 0U; i < BUFFER3_LENGTH; i++)
    {
        s_srcBuf3[i] = i;
    }
}

bool DEMO_ValidateResult(void)
{
    return ((0 == memcmp(s_srcBuf1, s_destBuf1, sizeof(s_srcBuf1))) &&
            (0 == memcmp(s_srcBuf2, s_destBuf2, sizeof(s_srcBuf2))) &&
            (0 == memcmp(s_srcBuf3, s_destBuf3, sizeof(s_srcBuf3))));
}

/*!
 * @brief Main function
 */
int main(void)
{
    gpdma_transfer_config_t transferConfig;

    BOARD_InitHardware();

    /* Initializes data. */
    DEMO_InitBuf();

    /* Print destination buffer */
    PRINTF("GPDMA memory to memory link list example begin.\r\n\r\n");

    /* Configure GPDMA channel for one shot transfer */
    GPDMA_Init(DEMO_DMA);

    /* Create the descriptors. */
    GPDMA_PrepareDescriptor(&s_descriptor[0], s_srcBuf2,     /* srcAddr */
                            kGPDMA_BurstSize1,               /* srcBurstSize */
                            kGPDMA_TransferWidth4Bytes,      /* srcTransferWidth */
                            s_destBuf2,                      /* destAddr */
                            kGPDMA_BurstSize1,               /* destBurstSize */
                            kGPDMA_TransferWidth4Bytes,      /* destTransferWidth */
                            sizeof(s_srcBuf2),               /* totalBytes */
                            kGPDMA_TransferM2MControllerDma, /* transferType */
                            (uint32_t)&s_descriptor[1]       /* Next descriptor. */
    );

    GPDMA_PrepareDescriptor(&s_descriptor[1], s_srcBuf3,     /* srcAddr */
                            kGPDMA_BurstSize1,               /* srcBurstSize */
                            kGPDMA_TransferWidth4Bytes,      /* srcTransferWidth */
                            s_destBuf3,                      /* destAddr */
                            kGPDMA_BurstSize1,               /* destBurstSize */
                            kGPDMA_TransferWidth4Bytes,      /* destTransferWidth */
                            sizeof(s_srcBuf3),               /* totalBytes */
                            kGPDMA_TransferM2MControllerDma, /* transferType */
                            0UL                              /* Next descriptor. */
    );

    GPDMA_PrepareTransferConfig(&transferConfig,                 /* config. */
                                s_srcBuf1,                       /* srcAddr */
                                kGPDMA_BurstSize1,               /* srcBurstSize */
                                kGPDMA_TransferWidth4Bytes,      /* srcTransferWidth */
                                s_destBuf1,                      /* destAddr */
                                kGPDMA_BurstSize1,               /* destBurstSize */
                                kGPDMA_TransferWidth4Bytes,      /* destTransferWidth */
                                sizeof(s_srcBuf1),               /* totalBytes */
                                kGPDMA_TransferM2MControllerDma, /* transferType */
                                (uint32_t)&s_descriptor[0]       /* next descriptor */
    );

    GPDMA_SetTransferConfig(DEMO_DMA, DEMO_DMA_CHANNEL, &transferConfig);
    GPDMA_EnableChannel(DEMO_DMA, DEMO_DMA_CHANNEL, true);

    /* Wait for GPDMA transfer finish */
    while (g_Transfer_Done != true)
    {
    }

    if (false == DEMO_ValidateResult())
    {
        PRINTF("\r\n\r\nGPDMA memory to memory link list example failed.\r\n\r\n");

        DEMO_DumpArray("s_srcBuf1", (uint8_t *)s_srcBuf1, sizeof(s_srcBuf1));
        DEMO_DumpArray("s_destBuf1", (uint8_t *)s_destBuf1, sizeof(s_destBuf1));

        DEMO_DumpArray("s_srcBuf2", (uint8_t *)s_srcBuf2, sizeof(s_srcBuf2));
        DEMO_DumpArray("s_destBuf2", (uint8_t *)s_destBuf2, sizeof(s_destBuf2));

        DEMO_DumpArray("s_srcBuf3", (uint8_t *)s_srcBuf3, sizeof(s_srcBuf3));
        DEMO_DumpArray("s_destBuf3", (uint8_t *)s_destBuf3, sizeof(s_destBuf3));
    }
    else
    {
        PRINTF("\r\n\r\nGPDMA memory to memory link list example succeed.\r\n\r\n");
    }

    while (1)
    {
    }
}
