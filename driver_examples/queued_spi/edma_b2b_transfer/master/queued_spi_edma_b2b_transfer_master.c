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
#include "fsl_queued_spi_edma.h"
#if defined(FSL_FEATURE_SOC_DMAMUX_COUNT) && FSL_FEATURE_SOC_DMAMUX_COUNT
#include "fsl_dmamux.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define TRANSFER_SIZE     64U     /* Transfer dataSize */
#define TRANSFER_BAUDRATE 500000U /* Transfer baudrate - 500k */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* QUEUEDSPI user callback */
void QSPI_MasterUserCallback(qspi_master_edma_handle_t *psHandle, status_t status, void *pUserData);

/*******************************************************************************
 * Variables
 ******************************************************************************/
AT_NONCACHEABLE_SECTION_INIT(uint8_t u8MasterRxDatas[TRANSFER_SIZE]) = {0};
AT_NONCACHEABLE_SECTION_INIT(uint8_t u8MasterTxDatas[TRANSFER_SIZE]) = {0};

AT_NONCACHEABLE_SECTION_ALIGN(qspi_master_edma_handle_t g_sQspiHandle, sizeof(edma_channel_tcd_t));

volatile bool bIsTransferCompleted    = false;
volatile uint32_t g_u32SystickCounter = 20U;
/*******************************************************************************
 * Code
 ******************************************************************************/
static void SysTick_Handler(void)
{
    if (g_u32SystickCounter != 0U)
    {
        g_u32SystickCounter--;
    }
}

void QSPI_MasterUserCallback(qspi_master_edma_handle_t *psHandle, status_t eStatus, void *pUserData)
{
    if (eStatus == kStatus_Success)
    {
        PRINTF("This is QUEUEDSPI master edma transfer completed callback. \r\n\r\n");
    }

    bIsTransferCompleted = true;
}

/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_InitHardware();

    PRINTF("QUEUEDSPI board to board edma example.\r\n");
    PRINTF("This example use one board as master and another as slave.\r\n");
    PRINTF("Master and slave uses EDMA way. Slave should start first. \r\n");
    PRINTF("Please make sure you make the correct line connection. Basically, the connection is: \r\n");
    PRINTF("QSPI_master --  QSPI_slave   \r\n");
    PRINTF("   CLK       --    CLK  \r\n");
    PRINTF("   PCS       --    PCS \r\n");
    PRINTF("   SOUT      --    SIN  \r\n");
    PRINTF("   SIN       --    SOUT \r\n");
    PRINTF("   GND       --    GND \r\n");

    uint32_t i;
    uint32_t u32ErrorCount;
    uint32_t u32LoopCount = 1U;
    qspi_transfer_t sMasterXfer;

    /* Master configuration. */
    qspi_master_config_t sMasterConfig;
    QSPI_MasterGetDefaultConfig(&sMasterConfig, EXAMPLE_QSPI_CLK_FREQ);
    sMasterConfig.u32BaudRateBps = TRANSFER_BAUDRATE;
    QSPI_MasterInit(EXAMPLE_QSPI_MASTER_BASEADDR, &sMasterConfig);

    /* DMA MUX setting and EDMA init. */
#if defined(FSL_FEATURE_SOC_DMAMUX_COUNT) && FSL_FEATURE_SOC_DMAMUX_COUNT
    /* DMA MUX init. */
    DMAMUX_ConnectChannelToTriggerSource(EXAMPLE_QSPI_DMA_MUX_BASE, (dmamux_dma_channel_t)EXAMPLE_QSPI_DMA_RX_CHANNEL,
                                         EXAMPLE_QSPI_DMA_RX_REQUEST_SOURCE);
    DMAMUX_ConnectChannelToTriggerSource(EXAMPLE_QSPI_DMA_MUX_BASE, (dmamux_dma_channel_t)EXAMPLE_QSPI_DMA_TX_CHANNEL,
                                         EXAMPLE_QSPI_DMA_TX_REQUEST_SOURCE);
#endif
    /* EDMA init. */
    /* Init the EDMA module */
    edma_config_t edma_config;
    EDMA_GetDefaultConfig(&edma_config);
    EDMA_Init(EXAMPLE_QSPI_DMA_BASE, &edma_config);

    QSPI_MasterTransferCreateHandleEDMA(EXAMPLE_QSPI_MASTER_BASEADDR, &g_sQspiHandle, QSPI_MasterUserCallback, NULL,
                                        EXAMPLE_QSPI_DMA_BASE, EXAMPLE_QSPI_DMA_TX_CHANNEL,
                                        EXAMPLE_QSPI_DMA_RX_CHANNEL);

    while (1)
    {
        /* Set up the transfer data. */
        for (i = 0U; i < TRANSFER_SIZE; i++)
        {
            u8MasterTxDatas[i] = (i + u32LoopCount) % 256U;
            u8MasterRxDatas[i] = 0U;
        }

        /* Print out transmit buffer. */
        PRINTF("\r\n Master transmit:\r\n");
        for (i = 0; i < TRANSFER_SIZE; i++)
        {
            /* Print 16 numbers in a line. */
            if ((i & 0x0FU) == 0U)
            {
                PRINTF("\r\n");
            }
            PRINTF(" %02X", u8MasterTxDatas[i]);
        }
        PRINTF("\r\n");

        /* Start master transfer. */
        bIsTransferCompleted      = false;
        sMasterXfer.pTxData       = u8MasterTxDatas;
        sMasterXfer.pRxData       = u8MasterRxDatas;
        sMasterXfer.u16DataSize   = TRANSFER_SIZE;
        sMasterXfer.u8ConfigFlags = 0U;

        QSPI_MasterTransferEDMA(&g_sQspiHandle, &sMasterXfer);

        /* Wait until transfer is completed. */
        while (!bIsTransferCompleted)
        {
        }

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
            PRINTF(" \r\nError occurred in QUEUEDSPI transfer ! \r\n");
        }
        /* Print out receive buffer. */
        PRINTF("\r\n Master received:\r\n");
        for (i = 0U; i < TRANSFER_SIZE; i++)
        {
            /* Print 16 numbers in a line. */
            if ((i & 0x0FU) == 0U)
            {
                PRINTF("\r\n");
            }
            PRINTF(" %02X", u8MasterRxDatas[i]);
        }
        PRINTF("\r\n");

        /* Wait for any key to be pressed. */
        PRINTF("\r\n Press any key to run again\r\n");
        GETCHAR();

        /* Increase loop count to change transmit buffer. */
        u32LoopCount++;
    }
}
