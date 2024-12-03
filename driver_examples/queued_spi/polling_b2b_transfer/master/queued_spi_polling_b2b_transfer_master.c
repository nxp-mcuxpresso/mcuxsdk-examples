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
#define TRANSFER_SIZE     64U     /*! Transfer dataSize */
#define TRANSFER_BAUDRATE 500000U /*! Transfer baudrate - 500k */

/*******************************************************************************
 * Variables
 ******************************************************************************/
uint8_t u8MasterRxDatas[TRANSFER_SIZE] = {0U};
uint8_t u8MasterTxDatas[TRANSFER_SIZE] = {0U};

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

int main(void)
{
    BOARD_InitHardware();

    PRINTF("QUEUEDSPI board to board polling example.\r\n");
    PRINTF("This example use one board as master and another as slave.\r\n");
    PRINTF("Master uses polling way and slave uses interrupt way. \r\n");
    PRINTF("Please make sure you make the correct line connection. Basically, the connection is: \r\n");
    PRINTF("QSPI_master -- QSPI_slave   \r\n");
    PRINTF("   CLK      --    CLK  \r\n");
    PRINTF("   PCS      --    PCS \r\n");
    PRINTF("   SOUT     --    SIN  \r\n");
    PRINTF("   SIN      --    SOUT \r\n");
    PRINTF("   GND      --    GND \r\n");

    uint32_t i;
    uint32_t u32ErrorCount;
    uint32_t u32LoopCount = 1U;
    qspi_transfer_t sMasterXfer;

    /* Master configuration. */
    qspi_master_config_t sMasterConfig;
    QSPI_MasterGetDefaultConfig(&sMasterConfig, EXAMPLE_QSPI_CLK_FREQ);
    sMasterConfig.u32BaudRateBps = TRANSFER_BAUDRATE;
    QSPI_MasterInit(EXAMPLE_QSPI_MASTER_BASEADDR, &sMasterConfig);

    while (1)
    {
        /* Set up the transfer data. */
        for (i = 0U; i < TRANSFER_SIZE; i++)
        {
            u8MasterTxDatas[i] = (i + u32LoopCount) % 256U;
            u8MasterRxDatas[i] = 0U;
        }

        /* Print out transmit data. */
        PRINTF("\r\n Master transmit:\r\n");
        for (i = 0U; i < TRANSFER_SIZE; i++)
        {
            /* Print 16 numbers in a line. */
            if ((i & 0x0FU) == 0U)
            {
                PRINTF("\r\n");
            }
            PRINTF(" %02X", u8MasterTxDatas[i]);
        }
        PRINTF("\r\n");

        /* Configure master transfer. */
        sMasterXfer.pTxData       = u8MasterTxDatas;
        sMasterXfer.pRxData       = u8MasterRxDatas;
        sMasterXfer.u16DataSize   = TRANSFER_SIZE;
        sMasterXfer.u8ConfigFlags = 0U;

        /* Start transfer. */
        QSPI_MasterTransferBlocking(EXAMPLE_QSPI_MASTER_BASEADDR, &sMasterXfer);

        u32ErrorCount = 0U;
        for (i = 0U; i < TRANSFER_SIZE; i++)
        {
            if (u8MasterTxDatas[i] != u8MasterRxDatas[i])
            {
                u32ErrorCount++;
            }
        }

        if (u32ErrorCount == 0U)
        {
            PRINTF(" \r\nQUEUEDSPI transfer all data matched! \r\n");
        }
        else
        {
            PRINTF("\r\nError occurred in QUEUEDSPI transfer ! \r\n");
        }
        /* Print out receive buffer */
        PRINTF("\r\n Master received:\r\n");
        for (i = 0U; i < TRANSFER_SIZE; i++)
        {
            /* Print 16 numbers in a line */
            if ((i & 0x0FU) == 0U)
            {
                PRINTF("\r\n");
            }
            PRINTF(" %02X", u8MasterRxDatas[i]);
        }
        PRINTF("\r\n");

        /* Wait for serial input. */
        PRINTF("\r\n Press any key to run again\r\n");
        GETCHAR();

        /* Increase loop count to change transmit buffer */
        u32LoopCount++;
    }
}
