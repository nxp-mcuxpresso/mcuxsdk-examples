/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_spi.h"

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
     * masterConfig.polarity = flexSPI_ClockPolarityActiveHigh;
     * masterConfig.phase = flexSPI_ClockPhaseFirstEdge;
     * masterConfig.direction = flexSPI_MsbFirst;
     * masterConfig.baudRate_Bps = 500000U;
     */
    SPI_MasterGetDefaultConfig(&masterConfig);
    sourceClock          = EXAMPLE_SPI_MASTER_CLK_FREQ;
    masterConfig.sselNum = (spi_ssel_t)EXAMPLE_SPI_SSEL;
    masterConfig.sselPol = (spi_spol_t)EXAMPLE_MASTER_SPI_SPOL;
    SPI_MasterInit(EXAMPLE_SPI_MASTER, &masterConfig, sourceClock);
    /* Init SPI slave, use interrupt mode, as no slave polling interface */
    /*
     * slaveConfig.polarity = flexSPI_ClockPolarityActiveHigh;
     * slaveConfig.phase = flexSPI_ClockPhaseFirstEdge;
     * slaveConfig.direction = flexSPI_MsbFirst;
     */
    SPI_SlaveGetDefaultConfig(&slaveConfig);
    slaveConfig.sselPol = (spi_spol_t)EXAMPLE_SLAVE_SPI_SPOL;
    SPI_SlaveInit(EXAMPLE_SPI_SLAVE, &slaveConfig);
    SPI_SlaveTransferCreateHandle(EXAMPLE_SPI_SLAVE, &slaveHandle, slaveCallback, NULL);

    /* Init source buffer */
    for (i = 0U; i < BUFFER_SIZE; i++)
    {
        srcBuff[i] = i;
    }

    /* SPI slave transfer */
    xfer.rxData   = destBuff;
    xfer.dataSize = sizeof(destBuff);
    xfer.txData   = NULL;
    SPI_SlaveTransferNonBlocking(EXAMPLE_SPI_SLAVE, &slaveHandle, &xfer);

    /* SPI master start transfer */
    xfer.txData   = srcBuff;
    xfer.dataSize = sizeof(srcBuff);
    xfer.rxData   = NULL;
    xfer.configFlags |= kSPI_FrameAssert;
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
