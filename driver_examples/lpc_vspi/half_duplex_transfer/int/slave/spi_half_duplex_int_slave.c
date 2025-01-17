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
/* The slave will receive 64 bytes from master, and then send them back.
 * slave will do this operation in one transimission, so the transfer data size is 128,
 * and slave will store the received data from the address of dataBuff[64] in the
 * first half transimission. In the second half transimission, slave will send them
 * back to the master board.
 */
#define BUFFER_SIZE        (192)
#define RX_BUFFER_INDEX    (64)
#define TRANSFER_DATA_SIZE (128)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
static uint8_t dataBuff[BUFFER_SIZE] = {0};

spi_slave_handle_t handle;
static volatile bool slaveFinished = false;
/*******************************************************************************
 * Code
 ******************************************************************************/
static void slaveCallback(SPI_Type *base, spi_slave_handle_t *slaveHandle, status_t status, void *userData)
{
    slaveFinished = true;
}

int main(void)
{
    uint32_t i          = 0;
    spi_transfer_t xfer = {0};
    spi_slave_config_t slaveConfig;

    BOARD_InitHardware();
    PRINTF("\r\nThis is slave example for SPI half-duplex interrupt transfer.");
    PRINTF("\r\nSlave is working....\r\n");

    /* Slave config */
    SPI_SlaveGetDefaultConfig(&slaveConfig);
    slaveConfig.sselPol                 = (spi_spol_t)EXAMPLE_SPI_SLAVE_SPOL;
    slaveConfig.fifoConfig.enableRxFifo = true;
    slaveConfig.fifoConfig.enableTxFifo = true;
    slaveConfig.fifoConfig.rxFifoSize   = 8U;
    slaveConfig.fifoConfig.txFifoSize   = 8U;
    SPI_SlaveInit(EXAMPLE_SPI_SLAVE_BASEADDR, &slaveConfig);
    SPI_SlaveTransferCreateHandle(EXAMPLE_SPI_SLAVE_BASEADDR, &handle, slaveCallback, NULL);

    /* Receive data from master board.*/
    xfer.txData   = dataBuff;
    xfer.rxData   = &dataBuff[RX_BUFFER_INDEX];
    xfer.dataSize = TRANSFER_DATA_SIZE;
    SPI_SlaveTransferNonBlocking(EXAMPLE_SPI_SLAVE_BASEADDR, &handle, &xfer);

    /* Wait for the recieve complete. */
    while (slaveFinished != true)
    {
    }

    PRINTF("\r\nThe received data are:");
    for (i = 0; i < BUFFER_SIZE / 3; i++)
    {
        /* Print 16 data in a line. */
        if ((i & 0x0F) == 0)
        {
            PRINTF("\r\n");
        }
        PRINTF("  0x%02X", dataBuff[i + RX_BUFFER_INDEX]);
    }

    PRINTF("\r\n\r\nSlave transfer completed.");

    SPI_Deinit(EXAMPLE_SPI_SLAVE_BASEADDR);

    while (1)
    {
    }
}
