/*
 * Copyright  2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_spi.h"
#include "board.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void EXAMPLE_MasterCallback(SPI0_Type *base, spi_master_handle_t *handle, status_t status, void *userData);
static void EXAMPLE_SPIMasterInit(void);
static void EXAMPLE_MasterStartTransfer(void);
static void EXAMPLE_TransferDataCheck(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static uint8_t txBuffer[BUFFER_SIZE];
static uint8_t rxBuffer[BUFFER_SIZE];
spi_master_handle_t masterHandle;
static volatile bool masterFinished = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
static void EXAMPLE_MasterCallback(SPI0_Type *base, spi_master_handle_t *handle, status_t status, void *userData)
{
    masterFinished = true;
}

int main(void)
{
    BOARD_InitHardware();

    /* Initialize the SPI master with configuration. */
    EXAMPLE_SPIMasterInit();

    /* Start transfer with slave board. */
    EXAMPLE_MasterStartTransfer();

    /* Check the received data. */
    EXAMPLE_TransferDataCheck();

    /* De-initialize the SPI. */
    SPI_Deinit(EXAMPLE_SPI_MASTER);

    while (1)
    {
    }
}

static void EXAMPLE_SPIMasterInit(void)
{
    spi_master_config_t userConfig;
    uint32_t srcFreq = 0U;

    /* configuration from using SPI_MasterGetDefaultConfig():
     * userConfig->enableLoopback = false;
     * userConfig->enableMaster = true;
     * userConfig->polarity = kSPI_ClockPolarityActiveHigh;
     * userConfig->phase = kSPI_ClockPhaseFirstEdge;
     * userConfig->baudRate_Bps = 500000U;
     * userConfig->dataWidth = 8u;
     */
    SPI_MasterGetDefaultConfig(&userConfig);
    userConfig.baudRate_Bps = EXAMPLE_SPI_MASTER_BAUDRATE;
    srcFreq                 = EXAMPLE_SPI_MASTER_CLK_FREQ;
    SPI_MasterInit(EXAMPLE_SPI_MASTER, &userConfig, srcFreq);
}

static void EXAMPLE_MasterStartTransfer(void)
{
    spi_transfer_t xfer = {0};
    uint32_t i          = 0U;

    /* Initialize txBuffer and rxBuffer. */
    for (i = 0; i < BUFFER_SIZE; i++)
    {
        txBuffer[i] = i;
        rxBuffer[i] = 0U;
    }

    /* Set transfer parameters for master. */
    xfer.txData   = txBuffer;
    xfer.rxData   = rxBuffer;
    xfer.dataSize = sizeof(txBuffer);

    /* Create handle for master. */
    SPI_MasterTransferCreateHandle(EXAMPLE_SPI_MASTER, &masterHandle, EXAMPLE_MasterCallback, NULL);

    /* Start transfer data. */
    SPI_MasterTransferNonBlocking(EXAMPLE_SPI_MASTER, &masterHandle, &xfer);
}
static void EXAMPLE_TransferDataCheck(void)
{
    uint32_t i = 0U, err = 0U;
    /* Waiting for transmission complete. */
    while (masterFinished != true)
    {
    }

    for (i = 0; i < BUFFER_SIZE; i++)
    {
        /* Check if data matched. */
        if (txBuffer[i] != rxBuffer[i])
        {
            err++;
        }
    }

    if (0U == err)
    {
        BOARD_LedMatrixShowNumber(0, 0, false);
    }
    else
    {
        BOARD_LedMatrixShowNumber(1, 0, false);
    }
}
