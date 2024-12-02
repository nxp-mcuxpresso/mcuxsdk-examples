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
#include "fsl_lpspi_edma.h"
#include "board.h"
#include "app.h"

#include "fsl_edma.h"
#if defined(FSL_FEATURE_SOC_DMAMUX_COUNT) && FSL_FEATURE_SOC_DMAMUX_COUNT
#include "fsl_dmamux.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_LPSPI_MASTER_DMA_MUX_BASEADDR ((DMAMUX_Type *)(EXAMPLE_LPSPI_MASTER_DMA_MUX_BASE))
#define EXAMPLE_LPSPI_MASTER_DMA_BASEADDR     ((DMA_Type *)(EXAMPLE_LPSPI_MASTER_DMA_BASE))

#define EXAMPLE_LPSPI_SLAVE_DMA_MUX_BASEADDR ((DMAMUX_Type *)(EXAMPLE_LPSPI_SLAVE_DMA_MUX_BASE))
#define EXAMPLE_LPSPI_SLAVE_DMA_BASEADDR     ((DMA_Type *)(EXAMPLE_LPSPI_SLAVE_DMA_BASE))

#define TRANSFER_SIZE     (512U)    /*! Transfer dataSize .*/
#define TRANSFER_BAUDRATE (500000U) /*! Transfer baudrate .*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* LPSPI user callback */
void LPSPI_SlaveUserCallback(LPSPI_Type *base, lpspi_slave_handle_t *handle, status_t status, void *userData);
void LPSPI_SlaveUserEdmaCallback(LPSPI_Type *base, lpspi_slave_edma_handle_t *handle, status_t status, void *userData);

extern uint32_t LPSPI_GetInstance(LPSPI_Type *base);
/*******************************************************************************
 * Variables
 ******************************************************************************/
AT_NONCACHEABLE_SECTION_INIT(uint8_t masterRxData[TRANSFER_SIZE]) = {0};
AT_NONCACHEABLE_SECTION_INIT(uint8_t masterTxData[TRANSFER_SIZE]) = {0};
AT_NONCACHEABLE_SECTION_INIT(uint8_t slaveRxData[TRANSFER_SIZE])  = {0};
AT_NONCACHEABLE_SECTION_INIT(uint8_t slaveTxData[TRANSFER_SIZE])  = {0};

AT_NONCACHEABLE_SECTION(lpspi_master_edma_handle_t g_m_edma_handle);
edma_handle_t lpspiEdmaMasterRxRegToRxDataHandle;
edma_handle_t lpspiEdmaMasterTxDataToTxRegHandle;

lpspi_slave_edma_handle_t g_s_edma_handle;
edma_handle_t lpspiEdmaSlaveRxRegToRxDataHandle;
edma_handle_t lpspiEdmaSlaveTxDataToTxRegHandle;

volatile bool isSlaveTransferCompleted  = false;
volatile bool isMasterTransferCompleted = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
void LPSPI_SlaveUserEdmaCallback(LPSPI_Type *base, lpspi_slave_edma_handle_t *handle, status_t status, void *userData)
{
    if (status == kStatus_Success)
    {
        PRINTF("This is LPSPI slave edma transfer completed callback. \r\n");
        PRINTF("It's a successful transfer. \r\n");
    }

    if (status == kStatus_LPSPI_Error)
    {
        PRINTF("This is LPSPI slave edma transfer completed callback. \r\n");
        PRINTF("Error occurred in this transfer. \r\n");
    }

    isSlaveTransferCompleted = true;
}

void LPSPI_MasterUserEdmaCallback(LPSPI_Type *base, lpspi_master_edma_handle_t *handle, status_t status, void *userData)
{
    isMasterTransferCompleted = true;
}

/*!
 * @brief Main function
 */
int main(void)
{
    uint32_t errorCount;
    uint32_t i;
    lpspi_master_config_t masterConfig;
    lpspi_slave_config_t slaveConfig;
    lpspi_transfer_t masterXfer;
    lpspi_transfer_t slaveXfer;
    edma_config_t userConfig;

    BOARD_InitHardware();

    PRINTF("LPSPI edma example start.\r\n");
    PRINTF("This example use one lpspi instance as master and another as slave on one board.\r\n");
    PRINTF("Both master and slave use edma way.\r\n");

    PRINTF("Please make sure you make the correct line connection. Basically, the connection is: \r\n");
    PRINTF("LPSPI_master -- LPSPI_slave   \r\n");
    PRINTF("   CLK      --    CLK  \r\n");
    PRINTF("   PCS      --    PCS \r\n");
    PRINTF("   SOUT     --    SIN  \r\n");
    PRINTF("   SIN      --    SOUT \r\n");

#if defined(FSL_FEATURE_SOC_DMAMUX_COUNT) && FSL_FEATURE_SOC_DMAMUX_COUNT
    /* DMA MUX init*/
    DMAMUX_Init(EXAMPLE_LPSPI_MASTER_DMA_MUX_BASEADDR);

#if (EXAMPLE_LPSPI_MASTER_DMA_MUX_BASE != EXAMPLE_LPSPI_SLAVE_DMA_MUX_BASE)
    DMAMUX_Init(EXAMPLE_LPSPI_SLAVE_DMA_MUX_BASEADDR);
#endif

    DMAMUX_SetSource(EXAMPLE_LPSPI_MASTER_DMA_MUX_BASEADDR, EXAMPLE_LPSPI_MASTER_DMA_RX_CHANNEL,
                     EXAMPLE_LPSPI_MASTER_DMA_RX_REQUEST_SOURCE);
    DMAMUX_EnableChannel(EXAMPLE_LPSPI_MASTER_DMA_MUX_BASEADDR, EXAMPLE_LPSPI_MASTER_DMA_RX_CHANNEL);

    DMAMUX_SetSource(EXAMPLE_LPSPI_MASTER_DMA_MUX_BASEADDR, EXAMPLE_LPSPI_MASTER_DMA_TX_CHANNEL,
                     EXAMPLE_LPSPI_MASTER_DMA_TX_REQUEST_SOURCE);
    DMAMUX_EnableChannel(EXAMPLE_LPSPI_MASTER_DMA_MUX_BASEADDR, EXAMPLE_LPSPI_MASTER_DMA_TX_CHANNEL);

    DMAMUX_SetSource(EXAMPLE_LPSPI_SLAVE_DMA_MUX_BASEADDR, EXAMPLE_LPSPI_SLAVE_DMA_RX_CHANNEL,
                     EXAMPLE_LPSPI_SLAVE_DMA_RX_REQUEST_SOURCE);
    DMAMUX_EnableChannel(EXAMPLE_LPSPI_SLAVE_DMA_MUX_BASEADDR, EXAMPLE_LPSPI_SLAVE_DMA_RX_CHANNEL);

    DMAMUX_SetSource(EXAMPLE_LPSPI_SLAVE_DMA_MUX_BASEADDR, EXAMPLE_LPSPI_SLAVE_DMA_TX_CHANNEL,
                     EXAMPLE_LPSPI_SLAVE_DMA_TX_REQUEST_SOURCE);
    DMAMUX_EnableChannel(EXAMPLE_LPSPI_SLAVE_DMA_MUX_BASEADDR, EXAMPLE_LPSPI_SLAVE_DMA_TX_CHANNEL);
#endif
    /* EDMA init*/
    /*
     * userConfig.enableRoundRobinArbitration = false;
     * userConfig.enableHaltOnError = true;
     * userConfig.enableContinuousLinkMode = false;
     * userConfig.enableDebugMode = false;
     */
    EDMA_GetDefaultConfig(&userConfig);

    EDMA_Init(EXAMPLE_LPSPI_MASTER_DMA_BASEADDR, &userConfig);

#if (EXAMPLE_LPSPI_SLAVE_DMA_BASE != EXAMPLE_LPSPI_MASTER_DMA_BASE)
    EDMA_Init(EXAMPLE_LPSPI_SLAVE_DMA_BASEADDR, &userConfig);
#endif

    /*Set up the transfer data*/
    for (i = 0; i < TRANSFER_SIZE; i++)
    {
        masterTxData[i] = (i + 1) % 256;
        masterRxData[i] = 0x55;

        slaveTxData[i] = ~masterTxData[i];
        slaveRxData[i] = 0x55;
    }

    /*Master config*/
    LPSPI_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate = TRANSFER_BAUDRATE;
    masterConfig.whichPcs = EXAMPLE_LPSPI_MASTER_PCS_FOR_INIT;
    masterConfig.pcsToSckDelayInNanoSec        = 1000000000U / (masterConfig.baudRate * 2U);
    masterConfig.lastSckToPcsDelayInNanoSec    = 1000000000U / (masterConfig.baudRate * 2U);
    masterConfig.betweenTransferDelayInNanoSec = 1000000000U / (masterConfig.baudRate * 2U);
    
    LPSPI_MasterInit(EXAMPLE_LPSPI_MASTER_BASEADDR, &masterConfig, EXAMPLE_LPSPI_MASTER_CLOCK_FREQ);

    /*Slave config*/
    LPSPI_SlaveGetDefaultConfig(&slaveConfig);
    slaveConfig.whichPcs = EXAMPLE_LPSPI_SLAVE_PCS_FOR_INIT;

    LPSPI_SlaveInit(EXAMPLE_LPSPI_SLAVE_BASEADDR, &slaveConfig);

    /*Set up slave first */
    memset(&(lpspiEdmaSlaveRxRegToRxDataHandle), 0, sizeof(lpspiEdmaSlaveRxRegToRxDataHandle));
    memset(&(lpspiEdmaSlaveTxDataToTxRegHandle), 0, sizeof(lpspiEdmaSlaveTxDataToTxRegHandle));

    EDMA_CreateHandle(&(lpspiEdmaSlaveRxRegToRxDataHandle), EXAMPLE_LPSPI_SLAVE_DMA_BASEADDR,
                      EXAMPLE_LPSPI_SLAVE_DMA_RX_CHANNEL);
    EDMA_CreateHandle(&(lpspiEdmaSlaveTxDataToTxRegHandle), EXAMPLE_LPSPI_SLAVE_DMA_BASEADDR,
                      EXAMPLE_LPSPI_SLAVE_DMA_TX_CHANNEL);

    LPSPI_SlaveTransferCreateHandleEDMA(EXAMPLE_LPSPI_SLAVE_BASEADDR, &g_s_edma_handle, LPSPI_SlaveUserEdmaCallback,
                                        NULL, &lpspiEdmaSlaveRxRegToRxDataHandle, &lpspiEdmaSlaveTxDataToTxRegHandle);

    /*Set slave transfer ready to receive/send data*/
    slaveXfer.txData      = slaveTxData;
    slaveXfer.rxData      = slaveRxData;
    slaveXfer.dataSize    = TRANSFER_SIZE;
    slaveXfer.configFlags = EXAMPLE_LPSPI_SLAVE_PCS_FOR_TRANSFER | kLPSPI_SlaveByteSwap;

    isSlaveTransferCompleted = false;
    LPSPI_SlaveTransferEDMA(EXAMPLE_LPSPI_SLAVE_BASEADDR, &g_s_edma_handle, &slaveXfer);

    /*Set up lpspi master*/
    memset(&(lpspiEdmaMasterRxRegToRxDataHandle), 0, sizeof(lpspiEdmaMasterRxRegToRxDataHandle));
    memset(&(lpspiEdmaMasterTxDataToTxRegHandle), 0, sizeof(lpspiEdmaMasterTxDataToTxRegHandle));

    EDMA_CreateHandle(&(lpspiEdmaMasterRxRegToRxDataHandle), EXAMPLE_LPSPI_MASTER_DMA_BASEADDR,
                      EXAMPLE_LPSPI_MASTER_DMA_RX_CHANNEL);
    EDMA_CreateHandle(&(lpspiEdmaMasterTxDataToTxRegHandle), EXAMPLE_LPSPI_MASTER_DMA_BASEADDR,
                      EXAMPLE_LPSPI_MASTER_DMA_TX_CHANNEL);

    LPSPI_MasterTransferCreateHandleEDMA(EXAMPLE_LPSPI_MASTER_BASEADDR, &g_m_edma_handle, LPSPI_MasterUserEdmaCallback,
                                         NULL, &lpspiEdmaMasterRxRegToRxDataHandle,
                                         &lpspiEdmaMasterTxDataToTxRegHandle);

    /*Start master transfer*/
    masterXfer.txData      = masterTxData;
    masterXfer.rxData      = masterRxData;
    masterXfer.dataSize    = TRANSFER_SIZE;
    masterXfer.configFlags = EXAMPLE_LPSPI_MASTER_PCS_FOR_TRANSFER | kLPSPI_MasterByteSwap | kLPSPI_MasterPcsContinuous;

    isMasterTransferCompleted = false;
    LPSPI_MasterTransferEDMA(EXAMPLE_LPSPI_MASTER_BASEADDR, &g_m_edma_handle, &masterXfer);

    /*Wait slave received all data .*/
    while ((!isSlaveTransferCompleted) || (!isMasterTransferCompleted))
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
        PRINTF("\r\nLPSPI transfer all data matched! \r\n");
    }
    else
    {
        PRINTF("\r\nError occurred in LPSPI transfer ! \r\n");
    }

    LPSPI_Deinit(EXAMPLE_LPSPI_MASTER_BASEADDR);
    LPSPI_Deinit(EXAMPLE_LPSPI_SLAVE_BASEADDR);

    PRINTF("End of example. \r\n");

    while (1)
    {
    }
}
