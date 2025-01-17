/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "fsl_spi_dma.h"
#include "fsl_spi.h"
#include "board.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Callback function for DMA transfer. */
static void SPI_MasterDMACallback(SPI_Type *base, spi_dma_handle_t *handle, status_t status, void *userData);

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
#define BUFFER_SIZE (64)
static uint8_t txData[BUFFER_SIZE];
static uint8_t rxData[BUFFER_SIZE];

dma_handle_t masterTxHandle;
dma_handle_t masterRxHandle;
spi_dma_handle_t masterHandle;

static volatile bool masterFinished = false;
/*******************************************************************************
 * Code
 ******************************************************************************/
static void SPI_MasterDMACallback(SPI_Type *base, spi_dma_handle_t *handle, status_t status, void *userData)
{
    masterFinished = true;
}

int main(void)
{
    uint32_t srcFreq                = 0;
    uint32_t i                      = 0;
    uint32_t err                    = 0U;
    spi_half_duplex_transfer_t xfer = {0};
    spi_master_config_t masterConfig;

    BOARD_InitHardware();

    PRINTF("This is SPI half-duplex dma transfer example!\r\n");
    PRINTF("SPI master board will transmit data to slave board first, then receive data from slave board.\r\n");
    PRINTF("To make sure the transfer work successfully, please start the slave board first!\r\n");
    PRINTF("Master will use dma way, and slave will use dma way too.\r\n");
    PRINTF("\r\nMaster start to tansfer data...\r\n");

    /* DMA init */
    DMA_Init(EXAMPLE_SPI_MASTER_DMA_BASEADDR);

    /* Master config */
    SPI_MasterGetDefaultConfig(&masterConfig);
    masterConfig.sselNum                 = (spi_ssel_t)EXAMPLE_SPI_MASTER_SSEL;
    masterConfig.sselPol                 = (spi_spol_t)EXAMPLE_SPI_MASTER_SPOL;
    masterConfig.fifoConfig.enableRxFifo = true;
    masterConfig.fifoConfig.enableTxFifo = true;
    masterConfig.fifoConfig.rxFifoSize   = 8U;
    masterConfig.fifoConfig.txFifoSize   = 8U;
    srcFreq                              = EXAMPLE_SPI_MASTER_CLK_FREQ;
    SPI_MasterInit(EXAMPLE_SPI_MASTER_BASEADDR, &masterConfig, srcFreq);

    /* Init Buffer*/
    for (i = 0; i < BUFFER_SIZE; i++)
    {
        txData[i] = i;
    }

    DMA_EnableChannel(EXAMPLE_SPI_MASTER_DMA_BASEADDR, EXAMPLE_SPI_MASTER_TX_CHANNEL);
    DMA_EnableChannel(EXAMPLE_SPI_MASTER_DMA_BASEADDR, EXAMPLE_SPI_MASTER_RX_CHANNEL);
    DMA_SetChannelPriority(EXAMPLE_SPI_MASTER_DMA_BASEADDR, EXAMPLE_SPI_MASTER_TX_CHANNEL, kDMA_ChannelPriority3);
    DMA_SetChannelPriority(EXAMPLE_SPI_MASTER_DMA_BASEADDR, EXAMPLE_SPI_MASTER_RX_CHANNEL, kDMA_ChannelPriority2);
    DMA_CreateHandle(&masterTxHandle, EXAMPLE_SPI_MASTER_DMA_BASEADDR, EXAMPLE_SPI_MASTER_TX_CHANNEL);
    DMA_CreateHandle(&masterRxHandle, EXAMPLE_SPI_MASTER_DMA_BASEADDR, EXAMPLE_SPI_MASTER_RX_CHANNEL);

    xfer.txData                = txData;
    xfer.rxData                = rxData;
    xfer.txDataSize            = sizeof(txData);
    xfer.rxDataSize            = sizeof(rxData);
    xfer.isTransmitFirst       = true;
    xfer.isPcsAssertInTransfer = true;
    xfer.configFlags           = kSPI_FrameAssert;

    SPI_MasterTransferCreateHandleDMA(EXAMPLE_SPI_MASTER_BASEADDR, &masterHandle, SPI_MasterDMACallback, NULL,
                                      &masterTxHandle, &masterRxHandle);

    SPI_MasterHalfDuplexTransferDMA(EXAMPLE_SPI_MASTER_BASEADDR, &masterHandle, &xfer);

    while (!masterFinished)
    {
    }

    PRINTF("The half-duplex transfer in edma way is end!\r\n");
    PRINTF("\r\nThe received data are:");
    /*Check if the data is right*/
    err = 0U;
    for (i = 0; i < BUFFER_SIZE; i++)
    {
        /* Print 16 numbers in a line */
        if ((i & 0x0FU) == 0U)
        {
            PRINTF("\r\n    ");
        }
        PRINTF("  0x%02X", rxData[i]);
        /* Check if data matched. */
        if (txData[i] != rxData[i])
        {
            err++;
        }
    }

    if (err == 0)
    {
        PRINTF("\r\nMaster half-duplex DMA transfer succeed!\r\n");
    }
    else
    {
        PRINTF("\r\nMaster half-duplex DMA transfer faild!\r\n");
    }

    /* Stop the transfer. */
    DMA_Deinit(EXAMPLE_SPI_MASTER_DMA_BASEADDR);
    SPI_Deinit(EXAMPLE_SPI_MASTER_BASEADDR);

    while (1)
    {
    }
}
