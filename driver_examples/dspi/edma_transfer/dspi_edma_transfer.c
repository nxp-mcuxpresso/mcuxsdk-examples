/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
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
#include "fsl_dspi_edma.h"
#include "fsl_edma.h"
#include "fsl_dmamux.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_DSPI_MASTER_DMA_MUX_BASEADDR ((DMAMUX_Type *)(EXAMPLE_DSPI_MASTER_DMA_MUX_BASE))
#define EXAMPLE_DSPI_MASTER_DMA_BASEADDR     ((DMA_Type *)(EXAMPLE_DSPI_MASTER_DMA_BASE))

#define EXAMPLE_DSPI_SLAVE_DMA_MUX_BASEADDR ((DMAMUX_Type *)(EXAMPLE_DSPI_SLAVE_DMA_MUX_BASE))
#define EXAMPLE_DSPI_SLAVE_DMA_BASEADDR     ((DMA_Type *)(EXAMPLE_DSPI_SLAVE_DMA_BASE))

#define TRANSFER_SIZE     256U    /*! Transfer dataSize */
#define TRANSFER_BAUDRATE 500000U /*! Transfer baudrate - 500k */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* DSPI user callback */
void DSPI_MasterUserCallback(SPI_Type *base, dspi_master_edma_handle_t *handle, status_t status, void *userData);
void DSPI_SlaveUserCallback(SPI_Type *base, dspi_slave_edma_handle_t *handle, status_t status, void *userData);

/*******************************************************************************
 * Variables
 ******************************************************************************/
uint8_t masterRxData[TRANSFER_SIZE] = {0};
uint8_t masterTxData[TRANSFER_SIZE] = {0};
uint8_t slaveRxData[TRANSFER_SIZE]  = {0};
uint8_t slaveTxData[TRANSFER_SIZE]  = {0};

dspi_master_edma_handle_t g_dspi_edma_m_handle;
edma_handle_t dspiEdmaMasterRxRegToRxDataHandle;
#if (!(defined(FSL_FEATURE_DSPI_HAS_GASKET) && FSL_FEATURE_DSPI_HAS_GASKET))
edma_handle_t dspiEdmaMasterTxDataToIntermediaryHandle;
#endif
edma_handle_t dspiEdmaMasterIntermediaryToTxRegHandle;

dspi_slave_edma_handle_t g_dspi_edma_s_handle;
edma_handle_t dspiEdmaSlaveRxRegToRxDataHandle;
edma_handle_t dspiEdmaSlaveTxDataToTxRegHandle;

volatile bool isTransferCompleted = false;

/*******************************************************************************
 * Code
 ******************************************************************************/

void DSPI_SlaveUserCallback(SPI_Type *base, dspi_slave_edma_handle_t *handle, status_t status, void *userData)
{
    if (status == kStatus_Success)
    {
        PRINTF("This is DSPI slave edma transfer completed callback. \r\n\r\n");
    }
    isTransferCompleted = true;
}

void DSPI_MasterUserCallback(SPI_Type *base, dspi_master_edma_handle_t *handle, status_t status, void *userData)
{
    if (status == kStatus_Success)
    {
        PRINTF("This is DSPI master edma transfer completed callback. \r\n\r\n");
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_InitHardware();

    PRINTF("DSPI edma example start.\r\n");
    PRINTF("This example use one dspi instance as master and another as slave on one board.\r\n");
    PRINTF("Master use edma way , slave uses interrupt.\r\n");
    PRINTF("Please make sure you make the correct line connection. Basically, the connection is: \r\n");
    PRINTF("DSPI_master -- DSPI_slave   \r\n");
    PRINTF("   CLK      --    CLK  \r\n");
    PRINTF("   PCS      --    PCS  \r\n");
    PRINTF("   SOUT     --    SIN  \r\n");
    PRINTF("   SIN      --    SOUT \r\n");

    /* DMA Mux setting and EDMA init */
    uint32_t masterRxChannel, masterTxChannel;
    uint32_t slaveRxChannel, slaveTxChannel;
    edma_config_t userConfig;

    masterRxChannel = 0U;
    masterTxChannel = 1U;
/* If DSPI instances support Gasket feature, only two channels are needed. */
#if (!(defined(FSL_FEATURE_DSPI_HAS_GASKET) && FSL_FEATURE_DSPI_HAS_GASKET))
    uint32_t masterIntermediaryChannel;
    masterIntermediaryChannel = 2U;
#endif

    /* Request DMA channels for TX & RX. */
    if ((dma_request_source_t)EXAMPLE_DSPI_SLAVE_DMA_RX_REQUEST_SOURCE < 0x200)
    {
        slaveRxChannel = 3U;
        slaveTxChannel = 4U;
    }
    else
    {
        slaveRxChannel = 16U;
        slaveTxChannel = 17U;
    }

    /* DMA MUX init */
    DMAMUX_Init(EXAMPLE_DSPI_MASTER_DMA_MUX_BASEADDR);

#if (defined EXAMPLE_DSPI_MASTER_DMA_MUX_BASE) && (defined EXAMPLE_DSPI_SLAVE_DMA_MUX_BASE) && \
    (EXAMPLE_DSPI_MASTER_DMA_MUX_BASE != EXAMPLE_DSPI_SLAVE_DMA_MUX_BASE)
    DMAMUX_Init(EXAMPLE_DSPI_SLAVE_DMA_MUX_BASEADDR);
#endif

    DMAMUX_SetSource(EXAMPLE_DSPI_MASTER_DMA_MUX_BASEADDR, masterRxChannel, EXAMPLE_DSPI_MASTER_DMA_RX_REQUEST_SOURCE);
    DMAMUX_EnableChannel(EXAMPLE_DSPI_MASTER_DMA_MUX_BASEADDR, masterRxChannel);

#if (defined EXAMPLE_DSPI_MASTER_DMA_TX_REQUEST_SOURCE)
    DMAMUX_SetSource(EXAMPLE_DSPI_MASTER_DMA_MUX_BASEADDR, masterTxChannel, EXAMPLE_DSPI_MASTER_DMA_TX_REQUEST_SOURCE);
    DMAMUX_EnableChannel(EXAMPLE_DSPI_MASTER_DMA_MUX_BASEADDR, masterTxChannel);
#endif

    DMAMUX_SetSource(EXAMPLE_DSPI_SLAVE_DMA_MUX_BASEADDR, slaveRxChannel, EXAMPLE_DSPI_SLAVE_DMA_RX_REQUEST_SOURCE);
    DMAMUX_EnableChannel(EXAMPLE_DSPI_SLAVE_DMA_MUX_BASEADDR, slaveRxChannel);

#if (defined EXAMPLE_DSPI_SLAVE_DMA_TX_REQUEST_SOURCE)
    DMAMUX_SetSource(EXAMPLE_DSPI_SLAVE_DMA_MUX_BASEADDR, slaveTxChannel, EXAMPLE_DSPI_SLAVE_DMA_TX_REQUEST_SOURCE);
    DMAMUX_EnableChannel(EXAMPLE_DSPI_SLAVE_DMA_MUX_BASEADDR, slaveTxChannel);
#endif

    /* EDMA init*/
    /*
     * userConfig.enableRoundRobinArbitration = false;
     * userConfig.enableHaltOnError = true;
     * userConfig.enableContinuousLinkMode = false;
     * userConfig.enableDebugMode = false;
     */
    EDMA_GetDefaultConfig(&userConfig);

    EDMA_Init(EXAMPLE_DSPI_MASTER_DMA_BASEADDR, &userConfig);

#if (defined EXAMPLE_DSPI_SLAVE_DMA_BASE) && (defined EXAMPLE_DSPI_MASTER_DMA_BASE) && \
    (EXAMPLE_DSPI_SLAVE_DMA_BASE != EXAMPLE_DSPI_MASTER_DMA_BASE)
    EDMA_Init(EXAMPLE_DSPI_SLAVE_DMA_BASEADDR, &userConfig);
#endif

    /*DSPI init*/
    uint32_t srcClock_Hz;
    uint32_t errorCount;
    uint32_t i;
    dspi_master_config_t masterConfig;
    dspi_slave_config_t slaveConfig;
    dspi_transfer_t masterXfer;
    dspi_transfer_t slaveXfer;

    /*Master config*/
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

    /*Slave config*/
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

    /* Set up dspi slave first */
    memset(&(dspiEdmaSlaveRxRegToRxDataHandle), 0, sizeof(dspiEdmaSlaveRxRegToRxDataHandle));
    memset(&(dspiEdmaSlaveTxDataToTxRegHandle), 0, sizeof(dspiEdmaSlaveTxDataToTxRegHandle));
    EDMA_CreateHandle(&(dspiEdmaSlaveRxRegToRxDataHandle), EXAMPLE_DSPI_SLAVE_DMA_BASEADDR, slaveRxChannel);
    EDMA_CreateHandle(&(dspiEdmaSlaveTxDataToTxRegHandle), EXAMPLE_DSPI_SLAVE_DMA_BASEADDR, slaveTxChannel);

    isTransferCompleted = false;

    DSPI_SlaveTransferCreateHandleEDMA(EXAMPLE_DSPI_SLAVE_BASEADDR, &g_dspi_edma_s_handle, DSPI_SlaveUserCallback, NULL,
                                       &dspiEdmaSlaveRxRegToRxDataHandle, &dspiEdmaSlaveTxDataToTxRegHandle);

    slaveXfer.txData      = slaveTxData;
    slaveXfer.rxData      = slaveRxData;
    slaveXfer.dataSize    = TRANSFER_SIZE;
    slaveXfer.configFlags = kDSPI_SlaveCtar0;

    if (kStatus_Success != DSPI_SlaveTransferEDMA(EXAMPLE_DSPI_SLAVE_BASEADDR, &g_dspi_edma_s_handle, &slaveXfer))
    {
        PRINTF("There is error when start DSPI_SlaveTransferEDMA \r\n");
    }

    /* Set up dspi master */
    memset(&(dspiEdmaMasterRxRegToRxDataHandle), 0, sizeof(dspiEdmaMasterRxRegToRxDataHandle));
#if (!(defined(FSL_FEATURE_DSPI_HAS_GASKET) && FSL_FEATURE_DSPI_HAS_GASKET))
    memset(&(dspiEdmaMasterTxDataToIntermediaryHandle), 0, sizeof(dspiEdmaMasterTxDataToIntermediaryHandle));
#endif
    memset(&(dspiEdmaMasterIntermediaryToTxRegHandle), 0, sizeof(dspiEdmaMasterIntermediaryToTxRegHandle));

    EDMA_CreateHandle(&(dspiEdmaMasterRxRegToRxDataHandle), EXAMPLE_DSPI_MASTER_DMA_BASEADDR, masterRxChannel);
#if (!(defined(FSL_FEATURE_DSPI_HAS_GASKET) && FSL_FEATURE_DSPI_HAS_GASKET))
    EDMA_CreateHandle(&(dspiEdmaMasterTxDataToIntermediaryHandle), EXAMPLE_DSPI_MASTER_DMA_BASEADDR,
                      masterIntermediaryChannel);
#endif
    EDMA_CreateHandle(&(dspiEdmaMasterIntermediaryToTxRegHandle), EXAMPLE_DSPI_MASTER_DMA_BASEADDR, masterTxChannel);
#if defined(FSL_FEATURE_DSPI_HAS_GASKET) && FSL_FEATURE_DSPI_HAS_GASKET
    DSPI_MasterTransferCreateHandleEDMA(EXAMPLE_DSPI_MASTER_BASEADDR, &g_dspi_edma_m_handle, DSPI_MasterUserCallback,
                                        NULL, &dspiEdmaMasterRxRegToRxDataHandle, NULL,
                                        &dspiEdmaMasterIntermediaryToTxRegHandle);
#else
    DSPI_MasterTransferCreateHandleEDMA(EXAMPLE_DSPI_MASTER_BASEADDR, &g_dspi_edma_m_handle, DSPI_MasterUserCallback,
                                        NULL, &dspiEdmaMasterRxRegToRxDataHandle,
                                        &dspiEdmaMasterTxDataToIntermediaryHandle,
                                        &dspiEdmaMasterIntermediaryToTxRegHandle);
#endif

    /* Start master transfer */
    masterXfer.txData      = masterTxData;
    masterXfer.rxData      = masterRxData;
    masterXfer.dataSize    = TRANSFER_SIZE;
    masterXfer.configFlags = kDSPI_MasterCtar0 | EXAMPLE_DSPI_MASTER_PCS_FOR_TRANSFER | kDSPI_MasterPcsContinuous;

    if (kStatus_Success != DSPI_MasterTransferEDMA(EXAMPLE_DSPI_MASTER_BASEADDR, &g_dspi_edma_m_handle, &masterXfer))
    {
        PRINTF("There is error when start DSPI_MasterTransferEDMA \r\n ");
    }

    /* Wait until transfer completed */
    while (!isTransferCompleted)
    {
    }

    /* Check the data */
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
    if (errorCount == 0)
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
