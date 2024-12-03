/*
 * Copyright  2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "fsl_debug_console.h"
#include "fsl_device_registers.h"
#include "fsl_spi.h"
#include "fsl_spi_gpdma.h"
#include "fsl_gpdma.h"
#include "board.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define TRANSFER_SIZE 64U /*! Transfer dataSize */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void SPI_MasterUserCallback(SPI_Type *base, spi_gpdma_handle_t *handle, status_t status, void *userData);
static void EXAMPLE_MasterInit(void);
static void EXAMPLE_MasterGPDMASetup(void);
static void EXAMPLE_MasterStartGPDMATransfer(void);
static void EXAMPLE_TransferDataCheck(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
uint8_t masterRxData[TRANSFER_SIZE] = {0};
uint8_t masterTxData[TRANSFER_SIZE] = {0};

gpdma_handle_t masterTxHandle;
gpdma_handle_t masterRxHandle;

spi_gpdma_handle_t masterHandle;

volatile bool isTransferCompleted = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
static void SPI_MasterUserCallback(SPI_Type *base, spi_gpdma_handle_t *handle, status_t status, void *userData)
{
    if (status == kStatus_Success)
    {
        isTransferCompleted = true;
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Initialize board setting. */
    BOARD_InitHardware();

    /* Print project information. */
    PRINTF("This is SPI GPDMA transfer master example.\r\n");
    PRINTF("This example will communicate with another slave SPI on the other board.\r\n");
    PRINTF("Please start the slave board first!\r\n");

    /* Initialize SPI master with configuration. */
    EXAMPLE_MasterInit();

    /* Set up GPDMA for SPI master TX and RX channel. */
    EXAMPLE_MasterGPDMASetup();

    /* Prepare GPDMA and start SPI master transfer in GPDMA way. */
    EXAMPLE_MasterStartGPDMATransfer();

    /* Waiting for transmission complete and check if all data matched. */
    EXAMPLE_TransferDataCheck();

    /* De-initialzie the GPDMA instance. */
    GPDMA_Deinit(EXAMPLE_GPDMA);

    /* De-initialize the SPI instance. */
    SPI_Deinit(EXAMPLE_SPI_MASTER);

    while (1)
    {
    }
}

static void EXAMPLE_MasterInit(void)
{
    /* SPI init */
    uint32_t srcClock_Hz = 0U;
    spi_master_config_t masterConfig;
    srcClock_Hz = EXAMPLE_SPI_MASTER_CLK_FREQ;

    SPI_MasterGetDefaultConfig(&masterConfig);
    masterConfig.sselNum = (spi_ssel_t)EXAMPLE_SPI_SSEL;
    masterConfig.sselPol = (spi_spol_t)EXAMPLE_MASTER_SPI_SPOL;
    SPI_MasterInit(EXAMPLE_SPI_MASTER, &masterConfig, srcClock_Hz);
}

static void EXAMPLE_MasterGPDMASetup(void)
{
    /* GPDMA init */
    GPDMA_Init(EXAMPLE_GPDMA);
    /* Configure the GPDMA handle and request source peripheral. */
    GPDMA_CreateHandle(&masterTxHandle, EXAMPLE_GPDMA, EXAMPLE_SPI_MASTER_TX_CHANNEL);
    GPDMA_CreateHandle(&masterRxHandle, EXAMPLE_GPDMA, EXAMPLE_SPI_MASTER_RX_CHANNEL);
    GPDMA_SetChannelSourcePeripheral(EXAMPLE_GPDMA, EXAMPLE_SPI_MASTER_RX_CHANNEL, kGpdmaRequestMuxSpiMasterRx);
    GPDMA_SetChannelDestinationPeripheral(EXAMPLE_GPDMA, EXAMPLE_SPI_MASTER_TX_CHANNEL, kGpdmaRequestMuxSpiMasterTx);
}

static void EXAMPLE_MasterStartGPDMATransfer(void)
{
    spi_transfer_t masterXfer;
    uint32_t i = 0U;

    /* Set up the transfer data */
    for (i = 0U; i < TRANSFER_SIZE; i++)
    {
        /* SPI is configured for 8 bits transfer - set only lower 8 bits of buffers */
        masterTxData[i] = i % 256U;
        masterRxData[i] = 0U;
    }

    /* Set up handle for spi master */
    SPI_MasterTransferCreateHandleGPDMA(EXAMPLE_SPI_MASTER, &masterHandle, SPI_MasterUserCallback, NULL,
                                        &masterTxHandle, &masterRxHandle);

    /* Start master transfer */
    masterXfer.txData      = (uint8_t *)&masterTxData;
    masterXfer.rxData      = (uint8_t *)&masterRxData;
    masterXfer.dataSize    = TRANSFER_SIZE * sizeof(masterTxData[0]);
    masterXfer.configFlags = kSPI_FrameAssert;

    if (kStatus_Success != SPI_MasterTransferGPDMA(EXAMPLE_SPI_MASTER, &masterHandle, &masterXfer))
    {
        PRINTF("There is an error when start SPI_MasterTransferGPDMA \r\n ");
    }
}

static void EXAMPLE_TransferDataCheck(void)
{
    uint32_t i = 0U, errorCount = 0U;

    /* Wait until transfer completed */
    while (!isTransferCompleted)
    {
    }

    PRINTF("\r\nThe received data are:");
    /*Check if the data is right*/
    for (i = 0; i < TRANSFER_SIZE; i++)
    {
        /* Print 16 numbers in a line */
        if ((i & 0x0FU) == 0U)
        {
            PRINTF("\r\n  ");
        }
        PRINTF("  0x%02X", masterRxData[i]);
        /* Check if data matched. */
        if (masterTxData[i] != masterRxData[i])
        {
            errorCount++;
        }
    }
    if (errorCount == 0)
    {
        PRINTF("\r\nSPI transfer all data matched! \r\n");
    }
    else
    {
        PRINTF("\r\nError occurred in SPI transfer ! \r\n");
    }
}
