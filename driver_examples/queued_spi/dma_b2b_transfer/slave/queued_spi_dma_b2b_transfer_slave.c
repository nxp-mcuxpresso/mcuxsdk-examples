/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_queued_spi.h"
#include "board.h"
#include "app.h"
#include "fsl_queued_spi_dma.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define TRANSFER_SIZE 64U /* Transfer dataSize */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* QUEUEDSPI user callback */
void QSPI_SlaveUserCallback(qspi_slave_dma_handle_t *handle, status_t status, void *pUserData);

/*******************************************************************************
 * Variables
 ******************************************************************************/
AT_NONCACHEABLE_SECTION_INIT(uint8_t u8SlaveRxDatas[TRANSFER_SIZE]) = {0};
AT_NONCACHEABLE_SECTION_INIT(uint8_t u8SlaveTxDatas[TRANSFER_SIZE]) = {0};

AT_NONCACHEABLE_SECTION(qspi_slave_dma_handle_t g_sQspiHandle);

volatile bool bIsTransferCompleted = false;

/*******************************************************************************
 * Code
 ******************************************************************************/

void QSPI_SlaveUserCallback(qspi_slave_dma_handle_t *psHandle, status_t eStatus, void *pUserData)
{
    if (eStatus == kStatus_Success)
    {
        PRINTF("This is QUEUEDSPI slave dma transfer completed callback. \r\n\r\n");
    }

    bIsTransferCompleted = true;
}

/*!
 * @brief Main function
 */
int main(void)
{
    uint32_t i;
    uint32_t u32ErrorCount;
    uint32_t u32LoopCount = 1U;
    qspi_transfer_t sSlaveXfer;
    qspi_slave_config_t sSlaveConfig;
    dma_config_t dma_config;

    BOARD_InitHardware();
    PRINTF("\r\nQUEUEDSPI board to board dma example -- Slave transfer.\r\n");

    QSPI_SlaveGetDefaultConfig(&sSlaveConfig);
    QSPI_SlaveInit(EXAMPLE_QSPI_SLAVE_BASEADDR, &sSlaveConfig);

    memset(&dma_config, 0U, sizeof(dma_config));

    DMA_Init(EXAMPLE_QSPI_DMA_BASE, &dma_config);
    DMA_SetChannelPeripheralRequest(EXAMPLE_QSPI_DMA_BASE, EXAMPLE_QSPI_DMA_RX_CHANNEL,
                                    EXAMPLE_QSPI_DMA_RX_REQUEST_SOURCE);
    DMA_SetChannelPeripheralRequest(EXAMPLE_QSPI_DMA_BASE, EXAMPLE_QSPI_DMA_TX_CHANNEL,
                                    EXAMPLE_QSPI_DMA_TX_REQUEST_SOURCE);
    QSPI_SlaveTransferCreateHandleDMA(EXAMPLE_QSPI_SLAVE_BASEADDR, &g_sQspiHandle, QSPI_SlaveUserCallback, NULL,
                                      EXAMPLE_QSPI_DMA_BASE, EXAMPLE_QSPI_DMA_TX_CHANNEL, EXAMPLE_QSPI_DMA_RX_CHANNEL);

    while (1)
    {
        PRINTF("\r\n Slave example is running...\r\n");

        /* Set up the transfer data. */
        for (i = 0U; i < TRANSFER_SIZE; i++)
        {
            u8SlaveTxDatas[i] = (i + u32LoopCount) % 256U;
            u8SlaveRxDatas[i] = 0U;
        }

        /* Set slave transfer ready to receive data */
        bIsTransferCompleted   = false;
        sSlaveXfer.pTxData     = u8SlaveTxDatas;
        sSlaveXfer.pRxData     = u8SlaveRxDatas;
        sSlaveXfer.u16DataSize = TRANSFER_SIZE;

        QSPI_SlaveTransferDMA(&g_sQspiHandle, &sSlaveXfer);

        while (!bIsTransferCompleted)
        {
        }

        u32ErrorCount = 0U;
        for (i = 0U; i < TRANSFER_SIZE; i++)
        {
            if (u8SlaveTxDatas[i] != u8SlaveRxDatas[i])
            {
                u32ErrorCount++;
            }
        }

        if (u32ErrorCount == 0U)
        {
            PRINTF(" \r\nQUEUEDSPI transfer all data matched! \r\n");
        }
        else
        {
            PRINTF("\r\nError occurred in QUEUEDSPI transfer ! \r\n");
        }

        /* Print out received data. */
        PRINTF("\r\n Slave received:");
        for (i = 0U; i < TRANSFER_SIZE; i++)
        {
            /* Print 16 numbers in a line. */
            if ((i & 0x0FU) == 0U)
            {
                PRINTF("\r\n    ");
            }
            PRINTF(" %02X", u8SlaveRxDatas[i]);
        }
        PRINTF("\r\n");

        u32LoopCount++;
    }
}
