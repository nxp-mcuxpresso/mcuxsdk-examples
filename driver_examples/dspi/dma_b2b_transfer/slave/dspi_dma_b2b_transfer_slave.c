/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_dspi.h"
#include "board.h"
#include "app.h"
#include "fsl_dspi_dma.h"
#include "fsl_dma.h"
#include "fsl_dmamux.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define TRANSFER_SIZE 64U /* Transfer dataSize */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* DSPI user callback */
void DSPI_SlaveUserCallback(SPI_Type *base, dspi_slave_dma_handle_t *handle, status_t status, void *userData);

/*******************************************************************************
 * Variables
 ******************************************************************************/
uint8_t slaveRxData[TRANSFER_SIZE] = {0};

dspi_slave_dma_handle_t g_dspi_dma_s_handle;
dma_handle_t dspiDmaSlaveRxRegToRxDataHandle;
dma_handle_t dspiDmaSlaveTxDataToTxRegHandle;

volatile bool isTransferCompleted = false;

/*******************************************************************************
 * Code
 ******************************************************************************/

void DSPI_SlaveUserCallback(SPI_Type *base, dspi_slave_dma_handle_t *handle, status_t status, void *userData)
{
    if (status == kStatus_Success)
    {
        PRINTF("This is DSPI slave dma transfer completed callback. \r\n\r\n");
    }

    isTransferCompleted = true;
}

/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_InitHardware();

    PRINTF("DSPI board to board dma example.\r\n");

    /* DMA Mux setting and DMA init */
    uint32_t slaveRxChannel, slaveTxChannel;

    slaveRxChannel = 0U;
    slaveTxChannel = 1U;

    /* DMA MUX init */
    DMAMUX_Init(EXAMPLE_DSPI_SLAVE_DMA_MUX_BASEADDR);

    DMAMUX_SetSource(EXAMPLE_DSPI_SLAVE_DMA_MUX_BASEADDR, slaveRxChannel,
                     (uint8_t)EXAMPLE_DSPI_SLAVE_DMA_RX_REQUEST_SOURCE);
    DMAMUX_EnableChannel(EXAMPLE_DSPI_SLAVE_DMA_MUX_BASEADDR, slaveRxChannel);

#if (defined EXAMPLE_DSPI_SLAVE_DMA_TX_REQUEST_SOURCE)
    DMAMUX_SetSource(EXAMPLE_DSPI_SLAVE_DMA_MUX_BASEADDR, slaveTxChannel,
                     (uint8_t)EXAMPLE_DSPI_SLAVE_DMA_TX_REQUEST_SOURCE);
    DMAMUX_EnableChannel(EXAMPLE_DSPI_SLAVE_DMA_MUX_BASEADDR, slaveTxChannel);
#endif

    /* DMA init */
    DMA_Init(EXAMPLE_DSPI_SLAVE_DMA_BASEADDR);

    uint32_t i;
    dspi_slave_config_t slaveConfig;
    dspi_transfer_t slaveXfer;

    /* Slave config */
    slaveConfig.whichCtar                  = kDSPI_Ctar0;
    slaveConfig.ctarConfig.bitsPerFrame    = 8U;
    slaveConfig.ctarConfig.cpol            = kDSPI_ClockPolarityActiveHigh;
    slaveConfig.ctarConfig.cpha            = kDSPI_ClockPhaseFirstEdge;
    slaveConfig.enableContinuousSCK        = false;
    slaveConfig.enableRxFifoOverWrite      = false;
    slaveConfig.enableModifiedTimingFormat = false;
    slaveConfig.samplePoint                = kDSPI_SckToSin0Clock;

    DSPI_SlaveInit(EXAMPLE_DSPI_SLAVE_BASEADDR, &slaveConfig);

    /* Set up dspi slave first */
    memset(&(dspiDmaSlaveRxRegToRxDataHandle), 0, sizeof(dspiDmaSlaveRxRegToRxDataHandle));
    memset(&(dspiDmaSlaveTxDataToTxRegHandle), 0, sizeof(dspiDmaSlaveTxDataToTxRegHandle));
    DMA_CreateHandle(&(dspiDmaSlaveRxRegToRxDataHandle), EXAMPLE_DSPI_SLAVE_DMA_BASEADDR, slaveRxChannel);
    DMA_CreateHandle(&(dspiDmaSlaveTxDataToTxRegHandle), EXAMPLE_DSPI_SLAVE_DMA_BASEADDR, slaveTxChannel);
    DSPI_SlaveTransferCreateHandleDMA(EXAMPLE_DSPI_SLAVE_BASEADDR, &g_dspi_dma_s_handle, DSPI_SlaveUserCallback, NULL,
                                      &dspiDmaSlaveRxRegToRxDataHandle, &dspiDmaSlaveTxDataToTxRegHandle);

    while (1)
    {
        PRINTF("\r\n Slave example is running...\r\n");

        /* Reset the receive buffer */
        for (i = 0U; i < TRANSFER_SIZE; i++)
        {
            slaveRxData[i] = 0U;
        }

        /* Set slave transfer ready to receive data */
        isTransferCompleted   = false;
        slaveXfer.txData      = NULL;
        slaveXfer.rxData      = slaveRxData;
        slaveXfer.dataSize    = TRANSFER_SIZE;
        slaveXfer.configFlags = kDSPI_SlaveCtar0;

        /* Slave start receive */
        if (kStatus_Success != DSPI_SlaveTransferDMA(EXAMPLE_DSPI_SLAVE_BASEADDR, &g_dspi_dma_s_handle, &slaveXfer))
        {
            PRINTF("There is error when start DSPI_SlaveTransferDMA \r\n");
        }

        while (!isTransferCompleted)
        {
        }

        /* Set slave transfer ready to send back data */
        isTransferCompleted   = false;
        slaveXfer.txData      = slaveRxData;
        slaveXfer.rxData      = NULL;
        slaveXfer.dataSize    = TRANSFER_SIZE;
        slaveXfer.configFlags = kDSPI_SlaveCtar0;

        /* Slave start send */
        if (kStatus_Success != DSPI_SlaveTransferDMA(EXAMPLE_DSPI_SLAVE_BASEADDR, &g_dspi_dma_s_handle, &slaveXfer))
        {
            PRINTF("There is error when start DSPI_SlaveTransferDMA \r\n");
        }

        while (!isTransferCompleted)
        {
        }

        /* Print out receive buffer */
        PRINTF("\r\n Slave receive:");
        for (i = 0; i < TRANSFER_SIZE; i++)
        {
            /* Print 16 numbers in a line */
            if ((i & 0x0FU) == 0U)
            {
                PRINTF("\r\n    ");
            }
            PRINTF(" %02X", slaveRxData[i]);
        }
        PRINTF("\r\n");
    }
}
