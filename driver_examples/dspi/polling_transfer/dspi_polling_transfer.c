/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
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

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define TRANSFER_SIZE     256U    /*! Transfer dataSize */
#define TRANSFER_BAUDRATE 500000U /*! Transfer baudrate - 500k */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* DSPI user callback */
void DSPI_SlaveUserCallback(SPI_Type *base, dspi_slave_handle_t *handle, status_t status, void *userData);

/*******************************************************************************
 * Variables
 ******************************************************************************/
uint8_t masterRxData[TRANSFER_SIZE] = {0U};
uint8_t masterTxData[TRANSFER_SIZE] = {0U};
uint8_t slaveRxData[TRANSFER_SIZE]  = {0U};
uint8_t slaveTxData[TRANSFER_SIZE]  = {0U};

dspi_slave_handle_t g_s_handle;

volatile bool isTransferCompleted = false;

/*******************************************************************************
 * Code
 ******************************************************************************/

void DSPI_SlaveUserCallback(SPI_Type *base, dspi_slave_handle_t *handle, status_t status, void *userData)
{
    if (status == kStatus_Success)
    {
        PRINTF("This is DSPI slave transfer completed callback. \r\n");
        PRINTF("It's a successful transfer. \r\n\r\n");
    }

    if (status == kStatus_DSPI_Error)
    {
        PRINTF("This is DSPI slave transfer completed callback. \r\n");
        PRINTF("Error occurred in this transfer. \r\n\r\n");
    }

    isTransferCompleted = true;
}

/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_InitHardware();

    PRINTF("DSPI polling example start.\r\n");
    PRINTF("This example use one dspi instance as master and another as slave on one board.\r\n");
    PRINTF("Master uses polling way and slave uses interrupt way.\r\n");
    PRINTF("Please make sure you make the correct line connection. Basically, the connection is: \r\n");
    PRINTF("DSPI_master -- DSPI_slave   \r\n");
    PRINTF("   CLK      --    CLK  \r\n");
    PRINTF("   PCS      --    PCS \r\n");
    PRINTF("   SOUT     --    SIN  \r\n");
    PRINTF("   SIN      --    SOUT \r\n");

    uint32_t srcClock_Hz;
    uint32_t errorCount;
    uint32_t i;
    dspi_master_config_t masterConfig;
    dspi_slave_config_t slaveConfig;
    dspi_transfer_t masterXfer;
    dspi_transfer_t slaveXfer;

    /* Master config */
    masterConfig.whichCtar                                = kDSPI_Ctar0;
    masterConfig.ctarConfig.baudRate                      = TRANSFER_BAUDRATE;
    masterConfig.ctarConfig.bitsPerFrame                  = 8U;
    masterConfig.ctarConfig.cpol                          = kDSPI_ClockPolarityActiveHigh;
    masterConfig.ctarConfig.cpha                          = kDSPI_ClockPhaseFirstEdge;
    masterConfig.ctarConfig.direction                     = kDSPI_MsbFirst;
    masterConfig.ctarConfig.pcsToSckDelayInNanoSec        = 1000000000U / TRANSFER_BAUDRATE;
    masterConfig.ctarConfig.lastSckToPcsDelayInNanoSec    = 1000000000U / TRANSFER_BAUDRATE;
    masterConfig.ctarConfig.betweenTransferDelayInNanoSec = 1000000000U / TRANSFER_BAUDRATE;

    masterConfig.whichPcs           = EXAMPLE_DSPI_MASTER_PCS_FOR_INIT;
    masterConfig.pcsActiveHighOrLow = kDSPI_PcsActiveLow;

    masterConfig.enableContinuousSCK        = false;
    masterConfig.enableRxFifoOverWrite      = false;
    masterConfig.enableModifiedTimingFormat = false;
    masterConfig.samplePoint                = kDSPI_SckToSin0Clock;

    srcClock_Hz = DSPI_MASTER_CLK_FREQ;
    DSPI_MasterInit(EXAMPLE_DSPI_MASTER_BASEADDR, &masterConfig, srcClock_Hz);

    /* Slave config */
    slaveConfig.whichCtar                  = kDSPI_Ctar0;
    slaveConfig.ctarConfig.bitsPerFrame    = masterConfig.ctarConfig.bitsPerFrame;
    slaveConfig.ctarConfig.cpol            = masterConfig.ctarConfig.cpol;
    slaveConfig.ctarConfig.cpha            = masterConfig.ctarConfig.cpha;
    slaveConfig.enableContinuousSCK        = masterConfig.enableContinuousSCK;
    slaveConfig.enableRxFifoOverWrite      = masterConfig.enableRxFifoOverWrite;
    slaveConfig.enableModifiedTimingFormat = masterConfig.enableModifiedTimingFormat;
    slaveConfig.samplePoint                = masterConfig.samplePoint;

    DSPI_SlaveInit(EXAMPLE_DSPI_SLAVE_BASEADDR, &slaveConfig);

    /* Set up the transfer data */
    for (i = 0U; i < TRANSFER_SIZE; i++)
    {
        masterTxData[i] = i % 256U;
        masterRxData[i] = 0U;

        slaveTxData[i] = ~masterTxData[i];
        slaveRxData[i] = 0U;
    }

    isTransferCompleted = false;
    /* Set up slave first */
    DSPI_SlaveTransferCreateHandle(EXAMPLE_DSPI_SLAVE_BASEADDR, &g_s_handle, DSPI_SlaveUserCallback, NULL);

    /* Set slave transfer ready to receive/send data */
    slaveXfer.txData      = slaveTxData;
    slaveXfer.rxData      = slaveRxData;
    slaveXfer.dataSize    = TRANSFER_SIZE;
    slaveXfer.configFlags = kDSPI_SlaveCtar0;

    DSPI_SlaveTransferNonBlocking(EXAMPLE_DSPI_SLAVE_BASEADDR, &g_s_handle, &slaveXfer);

    /* Start master transfer */
    masterXfer.txData      = masterTxData;
    masterXfer.rxData      = masterRxData;
    masterXfer.dataSize    = TRANSFER_SIZE;
    masterXfer.configFlags = kDSPI_MasterCtar0 | EXAMPLE_DSPI_MASTER_PCS_FOR_TRANSFER | kDSPI_MasterPcsContinuous;

    DSPI_MasterTransferBlocking(EXAMPLE_DSPI_MASTER_BASEADDR, &masterXfer);

    /* Wait slave received all data . Even though the DSPI_MasterTransferBlocking is a blocking transfer , but slave may
     * need a little bit of time to read the data of last frames from buffer. */
    while (!isTransferCompleted)
    {
    }

    errorCount = 0U;
    for (i = 0U; i < TRANSFER_SIZE; i++)
    {
        if (masterTxData[i] != slaveRxData[i])
        {
            errorCount++;
        }

        if (slaveTxData[i] != masterRxData[i])
        {
            errorCount++;
        }
    }
    if (errorCount == 0U)
    {
        PRINTF("DSPI transfer all data matched! \r\n");
    }
    else
    {
        PRINTF("Error occurred in DSPI transfer ! \r\n");
    }

    DSPI_Deinit(EXAMPLE_DSPI_MASTER_BASEADDR);
    DSPI_Deinit(EXAMPLE_DSPI_SLAVE_BASEADDR);

    while (1)
    {
    }
}
