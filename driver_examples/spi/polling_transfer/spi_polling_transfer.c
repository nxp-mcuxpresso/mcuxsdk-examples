/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
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
#define BUFFER_SIZE 64U
static uint8_t srcBuff[BUFFER_SIZE];
static uint8_t destBuff[BUFFER_SIZE];
static spi_slave_handle_t slaveHandle;
static volatile bool slaveFinished = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
static void slaveCallback(SPI_Type *base, spi_slave_handle_t *handle, status_t status, void *userData)
{
    slaveFinished = true;
}

int main(void)
{
    spi_master_config_t masterConfig = {0};
    spi_slave_config_t slaveConfig   = {0};
    spi_transfer_t xfer              = {0};
    uint32_t sourceClock             = 0U;
    uint32_t i                       = 0U;
    uint32_t err                     = 0U;

    /* Init the boards */
    BOARD_InitHardware();

    PRINTF("\r\nSPI one board polling example started!\r\n");

    /* Init SPI master */
    /*
     * masterConfig.enableStopInWaitMode = false;
     * masterConfig.polarity = kSPI_ClockPolarityActiveHigh;
     * masterConfig.phase = kSPI_ClockPhaseFirstEdge;
     * masterConfig.direction = kSPI_MsbFirst;
     * masterConfig.dataMode = kSPI_8BitMode;
     * masterConfig.txWatermark = kSPI_TxFifoOneHalfEmpty;
     * masterConfig.rxWatermark = kSPI_RxFifoOneHalfFull;
     * masterConfig.pinMode = kSPI_PinModeNormal;
     * masterConfig.outputMode = kSPI_SlaveSelectAutomaticOutput;
     * masterConfig.baudRate_Bps = 500000U;
     */
    SPI_MasterGetDefaultConfig(&masterConfig);
    sourceClock = EXAMPLE_SPI_MASTER_CLK_FREQ;
    SPI_MasterInit(EXAMPLE_SPI_MASTER, &masterConfig, sourceClock);

    /* Init SPI slave, use interrupt mode, as no slave polling interface */
    /*
     * slaveConfig.polarity = kSPI_ClockPolarityActiveHigh;
     * slaveConfig.phase = kSPI_ClockPhaseFirstEdge;
     * slaveConfig.direction = kSPI_MsbFirst;
     * slaveConfig.enableStopInWaitMode = false;
     * slaveConfig.dataMode = kSPI_8BitMode;
     * slaveConfig.txWatermark = kSPI_TxFifoOneHalfEmpty;
     * slaveConfig.rxWatermark = kSPI_RxFifoOneHalfFull;
     */
    SPI_SlaveGetDefaultConfig(&slaveConfig);
    SPI_SlaveInit(EXAMPLE_SPI_SLAVE, &slaveConfig);
    SPI_SlaveTransferCreateHandle(EXAMPLE_SPI_SLAVE, &slaveHandle, slaveCallback, NULL);

    /* Init source buffer */
    for (i = 0U; i < BUFFER_SIZE; i++)
    {
        srcBuff[i] = i;
    }

    /* SPI slave transfer */
    xfer.rxData   = destBuff;
    xfer.dataSize = BUFFER_SIZE;
    xfer.txData   = NULL;
    SPI_SlaveTransferNonBlocking(EXAMPLE_SPI_SLAVE, &slaveHandle, &xfer);

    /* SPI master start transfer */
    xfer.txData   = srcBuff;
    xfer.rxData   = NULL;
    xfer.dataSize = BUFFER_SIZE;
    SPI_MasterTransferBlocking(EXAMPLE_SPI_MASTER, &xfer);

    while (slaveFinished != true)
    {
    }

    /* Check the data received */
    for (i = 0U; i < BUFFER_SIZE; i++)
    {
        if (destBuff[i] != srcBuff[i])
        {
            PRINTF("\r\nThe %d data is wrong, the data received is %d \r\n", i, destBuff[i]);
            err++;
        }
    }
    if (err == 0U)
    {
        PRINTF("\r\nSPI transfer finished!\r\n");
    }

    while (1)
    {
    }
}
