/*
 * Copyright  2017 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_spi.h"
#include "board.h"
#include "app.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
#define BUFFER_SIZE (64)
static uint8_t buff[BUFFER_SIZE];
static uint8_t sendBuff[BUFFER_SIZE];
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
    uint8_t err         = 0;
    spi_transfer_t xfer = {0};
    spi_slave_config_t userConfig;

    BOARD_InitHardware();
    PRINTF("\n\rSlave is working....\n\r");

    /*
     * userConfig.enableSlave = true;
     * userConfig.polarity = kSPI_ClockPolarityActiveHigh;
     * userConfig.phase = kSPI_ClockPhaseFirstEdge;
     * userConfig.direction = kSPI_MsbFirst;
     */
    SPI_SlaveGetDefaultConfig(&userConfig);
    userConfig.sselPol                 = (spi_spol_t)EXAMPLE_SPI_SPOL;
    userConfig.fifoConfig.enableRxFifo = true;
    userConfig.fifoConfig.enableTxFifo = true;
    userConfig.fifoConfig.rxFifoSize   = 8U;
    userConfig.fifoConfig.txFifoSize   = 8U;
    SPI_SlaveInit(EXAMPLE_SPI_SLAVE, &userConfig);
    SPI_SlaveTransferCreateHandle(EXAMPLE_SPI_SLAVE, &handle, slaveCallback, NULL);
    for (i = 0; i < 64; i++)
    {
        sendBuff[i] = i;
    }

    /* receive data from master */
    xfer.txData   = sendBuff;
    xfer.rxData   = buff;
    xfer.dataSize = sizeof(sendBuff);
    SPI_SlaveTransferNonBlocking(EXAMPLE_SPI_SLAVE, &handle, &xfer);

    while (slaveFinished != true)
    {
    }

    for (i = 0; i < BUFFER_SIZE; i++)
    {
        if (buff[i] != i)
        {
            PRINTF("\n\rThe %d number is wrong! It is %d\n\r", i, buff[i]);
            err++;
        }
    }
    PRINTF("\r\n");
    if (err == 0)
    {
        PRINTF("Succeed!\n\r");
    }

    while (1)
    {
    }
}
