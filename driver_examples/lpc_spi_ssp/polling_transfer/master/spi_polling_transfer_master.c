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
static void EXAMPLE_SPIMasterInit(void);
static void EXAMPLE_MasterStartTransfer(void);
static void EXAMPLE_TransferDataCheck(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static uint8_t txBuffer[BUFFER_SIZE];
static uint8_t rxBuffer[BUFFER_SIZE];

/*******************************************************************************
 * Code
 ******************************************************************************/

int main(void)
{
    /* Initizlize the hardware. */
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
    spi_master_config_t userConfig = {0};
    uint32_t srcFreq               = 0U;

    SPI_MasterGetDefaultConfig(&userConfig);
    userConfig.baudRate_Bps = EXAMPLE_SPI_MASTER_BAUDRATE;
    srcFreq                 = EXAMPLE_SPI_MASTER_CLK_FREQ;
    SPI_MasterInit(EXAMPLE_SPI_MASTER, &userConfig, srcFreq);
}

static void EXAMPLE_MasterStartTransfer(void)
{
    uint32_t i          = 0U;
    spi_transfer_t xfer = {0};

    /* Init Buffer*/
    for (i = 0; i < BUFFER_SIZE; i++)
    {
        txBuffer[i] = i % 256;
        rxBuffer[i] = 0U;
    }

    /*Start Transfer*/
    xfer.txData   = txBuffer;
    xfer.rxData   = rxBuffer;
    xfer.dataSize = sizeof(txBuffer);
    /* Transfer data in polling mode. */
    SPI_MasterTransferBlocking(EXAMPLE_SPI_MASTER, &xfer);
}

static void EXAMPLE_TransferDataCheck(void)
{
    uint32_t i = 0U, err = 0U;

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
