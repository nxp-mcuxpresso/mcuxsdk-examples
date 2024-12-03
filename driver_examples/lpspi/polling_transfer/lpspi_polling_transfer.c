/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_lpspi.h"
#include "board.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define TRANSFER_SIZE     (512U)    /*! Transfer dataSize.*/
#define TRANSFER_BAUDRATE (500000U) /*! Transfer baudrate - 500k */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* LPSPI user callback */
void LPSPI_SlaveUserCallback(LPSPI_Type *base, lpspi_slave_handle_t *handle, status_t status, void *userData);

extern uint32_t LPSPI_GetInstance(LPSPI_Type *base);
/*******************************************************************************
 * Variables
 ******************************************************************************/
uint8_t masterRxData[TRANSFER_SIZE] = {0};
uint8_t masterTxData[TRANSFER_SIZE] = {0};
uint8_t slaveRxData[TRANSFER_SIZE]  = {0};
uint8_t slaveTxData[TRANSFER_SIZE]  = {0};

lpspi_slave_handle_t g_s_handle;

volatile bool isSlaveTransferCompleted = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
void LPSPI_SlaveUserCallback(LPSPI_Type *base, lpspi_slave_handle_t *handle, status_t status, void *userData)
{
    if (status == kStatus_Success)
    {
        PRINTF("This is LPSPI slave transfer completed callback. \r\n");
        PRINTF("It's a successful transfer. \r\n\r\n");
    }

    if (status == kStatus_LPSPI_Error)
    {
        PRINTF("This is LPSPI slave transfer completed callback. \r\n");
        PRINTF("Error occurred in this transfer. \r\n\r\n");
    }

    isSlaveTransferCompleted = true;
}
/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_InitHardware();

    PRINTF("LPSPI polling example start.\r\n");
    PRINTF("This example use one lpspi instance as master and another as slave on one board.\r\n");
    PRINTF("Master uses polling way and slave uses interrupt way.\r\n");
    PRINTF(
        "Note that some LPSPI instances interrupt is in INTMUX ,"
        "you should set the intmux when you porting this example accordingly \r\n");

    PRINTF("Please make sure you make the correct line connection. Basically, the connection is: \r\n");
    PRINTF("LPSPI_master -- LPSPI_slave   \r\n");
    PRINTF("   CLK      --    CLK  \r\n");
    PRINTF("   PCS      --    PCS \r\n");
    PRINTF("   SOUT     --    SIN  \r\n");
    PRINTF("   SIN      --    SOUT \r\n");

    uint32_t errorCount;
    uint32_t i;
    lpspi_master_config_t masterConfig;
    lpspi_slave_config_t slaveConfig;
    lpspi_transfer_t masterXfer;
    lpspi_transfer_t slaveXfer;

    /*Set up the transfer data*/
    for (i = 0; i < TRANSFER_SIZE; i++)
    {
        masterTxData[i] = i % 256;
        masterRxData[i] = 0;

        slaveTxData[i] = ~masterTxData[i];
        slaveRxData[i] = 0;
    }

    /*Master config*/
    LPSPI_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate = TRANSFER_BAUDRATE;
    masterConfig.whichPcs = EXAMPLE_LPSPI_MASTER_PCS_FOR_INIT;

    LPSPI_MasterInit(EXAMPLE_LPSPI_MASTER_BASEADDR, &masterConfig, EXAMPLE_LPSPI_MASTER_CLOCK_FREQ);

    /*Slave config*/
    LPSPI_SlaveGetDefaultConfig(&slaveConfig);
    slaveConfig.whichPcs = EXAMPLE_LPSPI_SLAVE_PCS_FOR_INIT;

    LPSPI_SlaveInit(EXAMPLE_LPSPI_SLAVE_BASEADDR, &slaveConfig);

    /*Set up slave first */
    isSlaveTransferCompleted = false;
    LPSPI_SlaveTransferCreateHandle(EXAMPLE_LPSPI_SLAVE_BASEADDR, &g_s_handle, LPSPI_SlaveUserCallback, NULL);

    /*Set slave transfer ready to receive/send data*/
    slaveXfer.txData      = slaveTxData;
    slaveXfer.rxData      = slaveRxData;
    slaveXfer.dataSize    = TRANSFER_SIZE;
    slaveXfer.configFlags = EXAMPLE_LPSPI_SLAVE_PCS_FOR_TRANSFER | kLPSPI_SlaveByteSwap;

    LPSPI_SlaveTransferNonBlocking(EXAMPLE_LPSPI_SLAVE_BASEADDR, &g_s_handle, &slaveXfer);

    /*Start master transfer*/
    masterXfer.txData      = masterTxData;
    masterXfer.rxData      = masterRxData;
    masterXfer.dataSize    = TRANSFER_SIZE;
    masterXfer.configFlags = EXAMPLE_LPSPI_MASTER_PCS_FOR_TRANSFER | kLPSPI_MasterPcsContinuous | kLPSPI_SlaveByteSwap;

    LPSPI_MasterTransferBlocking(EXAMPLE_LPSPI_MASTER_BASEADDR, &masterXfer);

    /*Wait slave received all data . Even though the LPSPI_MasterTransferBlocking is a blocking transfer , but slave may
     * need a little bit of time to read the data of last frames from buffer.*/
    while (!isSlaveTransferCompleted)
    {
    }

    errorCount = 0;
    for (i = 0; i < TRANSFER_SIZE; i++)
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
    if (errorCount == 0)
    {
        PRINTF("LPSPI transfer all data matched! \r\n");
    }
    else
    {
        PRINTF("Error occurred in LPSPI transfer ! \r\n");
    }

    LPSPI_Deinit(EXAMPLE_LPSPI_MASTER_BASEADDR);
    LPSPI_Deinit(EXAMPLE_LPSPI_SLAVE_BASEADDR);

    PRINTF("End of example. \r\n");

    while (1)
    {
    }
}
