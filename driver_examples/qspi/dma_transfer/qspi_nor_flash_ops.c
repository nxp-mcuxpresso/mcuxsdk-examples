/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "fsl_qspi_dma.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 *****************************************************************************/
static qspi_dma_handle_t qspiHandle = {0};
volatile bool isFinished            = false;
extern dma_handle_t dmaHandle;
/*******************************************************************************
 * Code
 ******************************************************************************/
static void callback(QuadSPI_Type *base, qspi_dma_handle_t *handle, status_t status, void *userData)
{
    isFinished = true;
}

/* Check if serial flash erase or program finished. */
void check_if_finished(void)
{
    uint32_t val = 0;
    /* Check WIP bit */
    do
    {
        while (QSPI_GetStatusFlags(EXAMPLE_QSPI) & kQSPI_Busy)
        {
        }
        QSPI_ClearFifo(EXAMPLE_QSPI, kQSPI_RxFifo);
        QSPI_ExecuteIPCommand(EXAMPLE_QSPI, 12U);
        while (QSPI_GetStatusFlags(EXAMPLE_QSPI) & kQSPI_Busy)
        {
        }
        val = EXAMPLE_QSPI->RBDR[0];
        /* Clear ARDB area */
        QSPI_ClearErrorFlag(EXAMPLE_QSPI, kQSPI_RxBufferDrain);
    } while (val & 0x1);
}

/* Write enable command */
void cmd_write_enable(void)
{
    while (QSPI_GetStatusFlags(EXAMPLE_QSPI) & kQSPI_Busy)
    {
    }
    QSPI_ExecuteIPCommand(EXAMPLE_QSPI, 4U);
}

#if defined(FLASH_ENABLE_QUAD_CMD)
/* Enable Quad mode */
void enable_quad_mode(void)
{
    uint32_t val[4] = {FLASH_ENABLE_QUAD_CMD, 0, 0, 0};

    while (QSPI_GetStatusFlags(EXAMPLE_QSPI) & kQSPI_Busy)
    {
    }
    QSPI_SetIPCommandAddress(EXAMPLE_QSPI, FSL_FEATURE_QSPI_AMBA_BASE);

    /* Clear Tx FIFO */
    QSPI_ClearFifo(EXAMPLE_QSPI, kQSPI_TxFifo);

    /* Write enable */
    cmd_write_enable();

    /* Write data into TX FIFO, needs to write at least 16 bytes of data */
    QSPI_WriteBlocking(EXAMPLE_QSPI, val, 16U);

    /* Set seq id, write register */
    QSPI_ExecuteIPCommand(EXAMPLE_QSPI, 20);

    /* Wait until finished */
    check_if_finished();
}
#endif

void enable_ddr_mode(void)
{
    QSPI_EnableDDRMode(EXAMPLE_QSPI, true);
}

#if defined(FLASH_ENABLE_OCTAL_CMD)
/* Enable Quad DDR mode */
void enable_octal_mode(void)
{
    uint32_t val[4] = {FLASH_ENABLE_OCTAL_CMD, 0, 0, 0};

    while (QSPI_GetStatusFlags(EXAMPLE_QSPI) & kQSPI_Busy)
    {
    }
    QSPI_SetIPCommandAddress(EXAMPLE_QSPI, FSL_FEATURE_QSPI_AMBA_BASE);

    /* Clear Tx FIFO */
    QSPI_ClearFifo(EXAMPLE_QSPI, kQSPI_TxFifo);

    /* Write enable */
    QSPI_ExecuteIPCommand(EXAMPLE_QSPI, 32U);

    /* Write data into TX FIFO, needs to write at least 16 bytes of data */
    QSPI_WriteBlocking(EXAMPLE_QSPI, val, 16U);

    /* Set seq id, write register */
    QSPI_ExecuteIPCommand(EXAMPLE_QSPI, 20);

    /* Wait until finished */
    check_if_finished();
}
#endif

/*Erase sector */
void erase_sector(uint32_t addr)
{
    while (QSPI_GetStatusFlags(EXAMPLE_QSPI) & kQSPI_Busy)
    {
    }
    QSPI_ClearFifo(EXAMPLE_QSPI, kQSPI_TxFifo);
    QSPI_SetIPCommandAddress(EXAMPLE_QSPI, addr);
    cmd_write_enable();
    QSPI_ExecuteIPCommand(EXAMPLE_QSPI, 28U);
    check_if_finished();

#if defined(FSL_FEATURE_QSPI_SOCCR_HAS_CLR_LPCAC) && (FSL_FEATURE_QSPI_SOCCR_HAS_CLR_LPCAC)
    QSPI_ClearCache(EXAMPLE_QSPI);
#endif
}

/* Erase all command */
void erase_all(void)
{
    while (QSPI_GetStatusFlags(EXAMPLE_QSPI) & kQSPI_Busy)
    {
    }
    QSPI_SetIPCommandAddress(EXAMPLE_QSPI, FSL_FEATURE_QSPI_AMBA_BASE);
    /* Write enable*/
    cmd_write_enable();
    QSPI_ExecuteIPCommand(EXAMPLE_QSPI, 8U);
    check_if_finished();
#if defined(FSL_FEATURE_QSPI_SOCCR_HAS_CLR_LPCAC) && (FSL_FEATURE_QSPI_SOCCR_HAS_CLR_LPCAC)
    QSPI_ClearCache(EXAMPLE_QSPI);
#endif
}

/* Program page into serial flash using QSPI polling way */
void program_page(uint32_t dest_addr, uint32_t *src_addr)
{
    qspi_transfer_t xfer = {0};

    xfer.data     = (uint32_t *)src_addr;
    xfer.dataSize = FLASH_PAGE_SIZE;
    while (QSPI_GetStatusFlags(EXAMPLE_QSPI) & kQSPI_Busy)
    {
    }
    QSPI_SetIPCommandAddress(EXAMPLE_QSPI, dest_addr);
    QSPI_SetIPCommandSize(EXAMPLE_QSPI, FLASH_PAGE_SIZE);
    cmd_write_enable();
    while (QSPI_GetStatusFlags(EXAMPLE_QSPI) & kQSPI_Busy)
    {
    }
    /* Use DMA transfer */
    QSPI_TransferSendDMA(EXAMPLE_QSPI, &qspiHandle, &xfer);

    /* Execute the programe page command */
    QSPI_ExecuteIPCommand(EXAMPLE_QSPI, 16U);

    /*Wait for DMA transfer finished*/
    while (isFinished != true)
    {
    }

    isFinished = false;
    /* Wait until flash finished program */
    check_if_finished();
    while (QSPI_GetStatusFlags(EXAMPLE_QSPI) & (kQSPI_Busy | kQSPI_IPAccess))
    {
    }
#if defined(FSL_FEATURE_QSPI_SOCCR_HAS_CLR_LPCAC) && (FSL_FEATURE_QSPI_SOCCR_HAS_CLR_LPCAC)
    QSPI_ClearCache(EXAMPLE_QSPI);
#endif
}

void qspi_nor_flash_init(QuadSPI_Type *base)
{
    uint32_t clockSourceFreq = 0;
    qspi_config_t config     = {0};

    /*Get QSPI default settings and configure the qspi */
    QSPI_GetDefaultQspiConfig(&config);

    /*Set AHB buffer size for reading data through AHB bus */
    if (FLASH_PAGE_SIZE <= FSL_FEATURE_QSPI_AHB_BUFFER_SIZE)
    {
        config.AHBbufferSize[3] = FLASH_PAGE_SIZE;
    }
    else
    {
        config.AHBbufferSize[3] = FSL_FEATURE_QSPI_AHB_BUFFER_SIZE;
    }
    clockSourceFreq    = QSPI_CLK_FREQ;
    config.txWatermark = 2;
    QSPI_Init(EXAMPLE_QSPI, &config, clockSourceFreq);

#if defined(FLASH_NEED_DQS)
    /* Set DQS config */
    QSPI_SetDqsConfig(base, &dqsConfig);
#endif

    /*According to serial flash feature to configure flash settings */
    QSPI_SetFlashConfig(base, &single_config);

    QSPI_TransferTxCreateHandleDMA(EXAMPLE_QSPI, &qspiHandle, callback, NULL, &dmaHandle);

#if defined(FSL_FEATURE_QSPI_SOCCR_HAS_CLR_LPCAC) && (FSL_FEATURE_QSPI_SOCCR_HAS_CLR_LPCAC)
    QSPI_ClearCache(base);
#endif
}
