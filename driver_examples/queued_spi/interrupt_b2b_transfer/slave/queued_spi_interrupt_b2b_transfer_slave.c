/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_queued_spi.h"
#include "board.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define TRANSFER_SIZE 64U /*! Transfer dataSize */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* QUEUEDSPI user callback */
void QSPI_SlaveUserCallback(qspi_slave_transfer_handle_t *handle, status_t status, void *pUserData);

/*******************************************************************************
 * Variables
 ******************************************************************************/
uint8_t u8SlaveRxDatas[TRANSFER_SIZE] = {0U};
uint8_t u8SlaveTxDatas[TRANSFER_SIZE] = {0U};

qspi_slave_transfer_handle_t g_sSlaveHandle;
volatile bool bIsTransferCompleted = false;

/*******************************************************************************
 * Code
 ******************************************************************************/

void QSPI_SlaveUserCallback(qspi_slave_transfer_handle_t *handle, status_t status, void *pUserData)
{
    if (status == kStatus_Success)
    {
        PRINTF("This is QUEUEDSPI slave transfer completed callback. \r\n");
        PRINTF("It's a successful transfer. \r\n\r\n");
    }

    if (status == kStatus_QSPI_Error)
    {
        PRINTF("This is QUEUEDSPI slave transfer completed callback. \r\n");
        PRINTF("Error occurred in this transfer. \r\n\r\n");
    }

    bIsTransferCompleted = true;
}

/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_InitHardware();

    PRINTF("QUEUEDSPI board to board interrupt example.\r\n");

    uint32_t i;
    uint32_t errorCount;
    uint32_t loopCount = 1U;
    qspi_slave_config_t sSlaveConfig;
    qspi_transfer_t sSlaveXfer;

    /* Slave configuration. */
    QSPI_SlaveGetDefaultConfig(&sSlaveConfig);
    QSPI_SlaveInit(EXAMPLE_QSPI_SLAVE_BASEADDR, &sSlaveConfig);

    QSPI_SlaveTransferCreateHandle(EXAMPLE_QSPI_SLAVE_BASEADDR, &g_sSlaveHandle, QSPI_SlaveUserCallback, NULL);

    while (1)
    {
        PRINTF("\r\n Slave example is running...\r\n");

        /* Set up the transfer data. */
        for (i = 0U; i < TRANSFER_SIZE; i++)
        {
            u8SlaveTxDatas[i] = (i + loopCount) % 256U;
            u8SlaveRxDatas[i] = 0U;
        }

        /* Set slave transfer ready to receive data */
        bIsTransferCompleted   = false;
        sSlaveXfer.pTxData     = u8SlaveTxDatas;
        sSlaveXfer.pRxData     = u8SlaveRxDatas;
        sSlaveXfer.u16DataSize = TRANSFER_SIZE;

        /* Slave starts receive */
        QSPI_SlaveTransferNonBlocking(&g_sSlaveHandle, &sSlaveXfer);

        while (!bIsTransferCompleted)
        {
        }

        errorCount = 0U;
        for (i = 0U; i < TRANSFER_SIZE; i++)
        {
            if (u8SlaveTxDatas[i] != u8SlaveRxDatas[i])
            {
                errorCount++;
            }
        }

        if (errorCount == 0U)
        {
            PRINTF(" \r\nQUEUEDSPI transfer all data matched! \r\n");
        }
        else
        {
            PRINTF("\r\nError occurred in QUEUEDSPI transfer ! \r\n");
        }

        /* Print out received data. */
        PRINTF("\r\n Slave received:");
        for (i = 0U; i < TRANSFER_SIZE; i++)
        {
            /* Print 16 numbers in a line. */
            if ((i & 0x0FU) == 0U)
            {
                PRINTF("\r\n    ");
            }
            PRINTF(" %02X", u8SlaveRxDatas[i]);
        }
        PRINTF("\r\n");

        loopCount++;
    }
}
