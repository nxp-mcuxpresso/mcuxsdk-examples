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
#define TRANSFER_SIZE     64U     /* Transfer dataSize */
#define TRANSFER_BAUDRATE 500000U /* Transfer baudrate - 500k */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void QSPI_MasterUserCallback(qspi_master_dma_handle_t *psHandle, status_t status, void *pUserData);
void SysTick_Handler(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
AT_NONCACHEABLE_SECTION_INIT(uint8_t u8MasterRxDatas[TRANSFER_SIZE]) = {0};
AT_NONCACHEABLE_SECTION_INIT(uint8_t u8MasterTxDatas[TRANSFER_SIZE]) = {0};

AT_NONCACHEABLE_SECTION_INIT(qspi_master_dma_handle_t g_sQspiHandle) = {0};
dma_handle_t g_sQspiDmaRxHandle;
dma_handle_t g_sQspiDmaTxHandle;

volatile bool bIsTransferCompleted    = false;
volatile uint32_t g_u32SystickCounter = 20U;
/*******************************************************************************
 * Code
 ******************************************************************************/
void SysTick_Handler(void)
{
    if (g_u32SystickCounter != 0U)
    {
        g_u32SystickCounter--;
    }
}

void QSPI_MasterUserCallback(qspi_master_dma_handle_t *psHandle, status_t eStatus, void *pUserData)
{
    if (eStatus == kStatus_Success)
    {
        PRINTF("This is QUEUEDSPI master dma transfer completed callback. \r\n\r\n");
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
    qspi_transfer_t sMasterXfer;
    qspi_master_config_t sMasterConfig;
    dma_config_t dma_config;

    BOARD_InitHardware();
    PRINTF("\r\nQUEUEDSPI board to board dma example -- Master transfer.\r\n");

    QSPI_MasterGetDefaultConfig(&sMasterConfig, EXAMPLE_QSPI_CLK_FREQ);
    sMasterConfig.u32BaudRateBps = TRANSFER_BAUDRATE;
    QSPI_MasterInit(EXAMPLE_QSPI_MASTER_BASEADDR, &sMasterConfig);

    memset(&dma_config, 0U, sizeof(dma_config));

    DMA_Init(EXAMPLE_QSPI_DMA_BASE, &dma_config);
    DMA_SetChannelPeripheralRequest(EXAMPLE_QSPI_DMA_BASE, EXAMPLE_QSPI_DMA_RX_CHANNEL,
                                    EXAMPLE_QSPI_DMA_RX_REQUEST_SOURCE);
    DMA_SetChannelPeripheralRequest(EXAMPLE_QSPI_DMA_BASE, EXAMPLE_QSPI_DMA_TX_CHANNEL,
                                    EXAMPLE_QSPI_DMA_TX_REQUEST_SOURCE);
    QSPI_MasterTransferCreateHandleDMA(EXAMPLE_QSPI_MASTER_BASEADDR, &g_sQspiHandle, QSPI_MasterUserCallback, NULL,
                                       EXAMPLE_QSPI_DMA_BASE, EXAMPLE_QSPI_DMA_TX_CHANNEL, EXAMPLE_QSPI_DMA_RX_CHANNEL);

    while (1)
    {
        /* Set up the transfer data. */
        for (i = 0U; i < TRANSFER_SIZE; i++)
        {
            u8MasterTxDatas[i] = (i + u32LoopCount) % 256U;
            u8MasterRxDatas[i] = 0U;
        }

        /* Print out transmit buffer. */
        PRINTF("\r\n Master transmit:\r\n");
        for (i = 0; i < TRANSFER_SIZE; i++)
        {
            /* Print 16 numbers in a line. */
            if ((i & 0x0FU) == 0U)
            {
                PRINTF("\r\n");
            }
            PRINTF(" %02X", u8MasterTxDatas[i]);
        }
        PRINTF("\r\n");

        /* Start master transfer. */
        bIsTransferCompleted      = false;
        sMasterXfer.pTxData       = u8MasterTxDatas;
        sMasterXfer.pRxData       = u8MasterRxDatas;
        sMasterXfer.u16DataSize   = TRANSFER_SIZE;
        sMasterXfer.u8ConfigFlags = 0U;

        QSPI_MasterTransferDMA(&g_sQspiHandle, &sMasterXfer);

        /* Wait until transfer is completed. */
        while (!bIsTransferCompleted)
        {
        }

        u32ErrorCount = 0U;
        for (i = 0U; i < TRANSFER_SIZE; i++)
        {
            if (u8MasterTxDatas[i] != u8MasterRxDatas[i])
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
            PRINTF(" \r\nError occurred in QUEUEDSPI transfer ! \r\n");
        }
        /* Print out receive buffer. */
        PRINTF("\r\n Master received:\r\n");
        for (i = 0U; i < TRANSFER_SIZE; i++)
        {
            /* Print 16 numbers in a line. */
            if ((i & 0x0FU) == 0U)
            {
                PRINTF("\r\n");
            }
            PRINTF(" %02X", u8MasterRxDatas[i]);
        }
        PRINTF("\r\n");

        /* Wait for any key to be pressed. */
        PRINTF("\r\n Press any key to run again\r\n");
        GETCHAR();

        /* Increase loop count to change transmit buffer. */
        u32LoopCount++;
    }
}
