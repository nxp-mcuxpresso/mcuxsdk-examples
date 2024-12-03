/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "fsl_spi.h"
#include "board.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BUFFER_SIZE (64)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* Callback function for interrupt transfer. */
static void SPI_MasterInterruptCallback(SPI_Type *base,
                                        spi_master_handle_t *masterHandle,
                                        status_t status,
                                        void *userData);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static uint8_t txData[BUFFER_SIZE];
static uint8_t rxData[BUFFER_SIZE];

spi_master_handle_t handle;
static volatile bool masterFinished = false;
/*******************************************************************************
 * Code
 ******************************************************************************/
static void SPI_MasterInterruptCallback(SPI_Type *base,
                                        spi_master_handle_t *masterHandle,
                                        status_t status,
                                        void *userData)
{
    masterFinished = true;
}

int main(void)
{
    uint32_t srcFreq = 0;
    uint32_t i       = 0;
    uint32_t err     = 0U;
    /* Transfer structure for half-duplex. */
    spi_half_duplex_transfer_t xfer = {0};
    spi_master_config_t masterConfig;

    BOARD_InitHardware();

    PRINTF("This is SPI half-duplex interrupt transfer example!\r\n");
    PRINTF("SPI master board will transmit data to slave board first, then receive data from slave board.\r\n");
    PRINTF("To make sure the transfer work successfully, please start the slave board first!\r\n");
    PRINTF("Master will use interrupt way, and slave will use interrupt way too.\r\n");
    PRINTF("\r\nMaster start to tansfer data...\r\n");

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

    xfer.txData                = txData;
    xfer.rxData                = rxData;
    xfer.txDataSize            = sizeof(txData);
    xfer.rxDataSize            = sizeof(rxData);
    xfer.isTransmitFirst       = true;
    xfer.isPcsAssertInTransfer = true;
    xfer.configFlags           = kSPI_FrameAssert;

    /* Half-duplex transfer in interrupt way. */
    SPI_MasterTransferCreateHandle(EXAMPLE_SPI_MASTER_BASEADDR, &handle, SPI_MasterInterruptCallback, NULL);
    SPI_MasterHalfDuplexTransferNonBlocking(EXAMPLE_SPI_MASTER_BASEADDR, &handle, &xfer);

    while (!masterFinished)
    {
    }
    PRINTF("The half-duplex transfer in interrupt way is end!\r\n");
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
        PRINTF("\r\nMaster half-duplex interrupt transfer succeed!\r\n");
    }
    else
    {
        PRINTF("\r\nMaster half-duplex interrupt transfer faild!\r\n");
    }

    SPI_Deinit(EXAMPLE_SPI_MASTER_BASEADDR);

    while (1)
    {
    }
}
