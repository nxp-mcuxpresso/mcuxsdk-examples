/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018, 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "fsl_qspi.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#ifndef QSPI_CMD_SEQ_READ
#define QSPI_CMD_SEQ_READ 0U
#endif
#ifndef QSPI_CMD_SEQ_WRITE_ENABLE
#define QSPI_CMD_SEQ_WRITE_ENABLE 4U
#endif
#ifndef QSPI_CMD_SEQ_READ_STATUS_REG
#define QSPI_CMD_SEQ_READ_STATUS_REG 12U
#endif
#ifndef QSPI_CMD_SEQ_WRITE_REG
#define QSPI_CMD_SEQ_WRITE_REG 20U
#endif
#ifndef QSPI_CMD_SEQ_ERASE_SECTOR
#define QSPI_CMD_SEQ_ERASE_SECTOR 28U
#endif
#ifndef QSPI_CMD_SEQ_ERASE_ALL
#define QSPI_CMD_SEQ_ERASE_ALL 8U
#endif
#ifndef QSPI_CMD_SEQ_PROGRAM_PAGE
#define QSPI_CMD_SEQ_PROGRAM_PAGE 16U
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 *****************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/* Check if serial flash erase or program finished. */
static void check_if_finished(void)
{
    uint32_t val = 0;
    /* Check WIP bit */
    do
    {
        while ((QSPI_GetStatusFlags(EXAMPLE_QSPI) & (uint32_t)kQSPI_Busy) != 0U)
        {
        }
        QSPI_ClearFifo(EXAMPLE_QSPI, (uint32_t)kQSPI_RxFifo);
        QSPI_ExecuteIPCommand(EXAMPLE_QSPI, QSPI_CMD_SEQ_READ_STATUS_REG);
        while ((QSPI_GetStatusFlags(EXAMPLE_QSPI) & (uint32_t)kQSPI_Busy) != 0U)
        {
        }
        val = EXAMPLE_QSPI->RBDR[0];
        /* Clear ARDB area */
        QSPI_ClearErrorFlag(EXAMPLE_QSPI, (uint32_t)kQSPI_RxBufferDrain);
    } while ((val & 1UL) != 0UL);
}

/* Write enable command */
static void cmd_write_enable(void)
{
#if defined(QSPI_CMD_REUSE_LUT) && QSPI_CMD_REUSE_LUT
    BOARD_QspiUpdateLUT(QSPI_CMD_SEQ_WRITE_ENABLE, QSPI_CMD_TYPE_WRITE_ENABLE);
#endif
    QSPI_ExecuteIPCommand(EXAMPLE_QSPI, QSPI_CMD_SEQ_WRITE_ENABLE);
}

#if defined(FLASH_ENABLE_QUAD_CMD)
/* Enable Quad mode */
void enable_quad_mode(void)
{
    uint32_t val[4] = {FLASH_ENABLE_QUAD_CMD, 0, 0, 0};

    while ((QSPI_GetStatusFlags(EXAMPLE_QSPI) & (uint32_t)kQSPI_Busy) != 0U)
    {
    }
    QSPI_SetIPCommandAddress(EXAMPLE_QSPI, FSL_FEATURE_QSPI_AMBA_BASE);

    /* Clear Tx FIFO */
    QSPI_ClearFifo(EXAMPLE_QSPI, (uint32_t)kQSPI_TxFifo);

    cmd_write_enable();

    /* Write data into TX FIFO, needs to write at least 16 bytes of data */
    QSPI_WriteBlocking(EXAMPLE_QSPI, val, 16U);

    /* Set seq id, write register */
    QSPI_ExecuteIPCommand(EXAMPLE_QSPI, QSPI_CMD_SEQ_WRITE_REG);

    /* Wait until finished */
    check_if_finished();
}
#endif

#if (defined(FSL_FEATURE_QSPI_HAS_DDR) && FSL_FEATURE_QSPI_HAS_DDR)
void enable_ddr_mode(void)
{
    QSPI_EnableDDRMode(EXAMPLE_QSPI, true);
}
#endif

#if defined(FLASH_ENABLE_OCTAL_CMD)
/* Enable Quad DDR mode */
void enable_octal_mode(void)
{
    uint32_t val[4] = {FLASH_ENABLE_OCTAL_CMD, 0, 0, 0};

    while ((QSPI_GetStatusFlags(EXAMPLE_QSPI) & (uint32_t)kQSPI_Busy) != 0U)
    {
    }
    QSPI_SetIPCommandAddress(EXAMPLE_QSPI, FSL_FEATURE_QSPI_AMBA_BASE);

    /* Clear Tx FIFO */
    QSPI_ClearFifo(EXAMPLE_QSPI, (uint32_t)kQSPI_TxFifo);

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

/* Erase sector */
void erase_sector(uint32_t addr)
{
    while ((QSPI_GetStatusFlags(EXAMPLE_QSPI) & (uint32_t)kQSPI_Busy) != 0U)
    {
    }
    QSPI_ClearFifo(EXAMPLE_QSPI, (uint32_t)kQSPI_TxFifo);
    QSPI_SetIPCommandAddress(EXAMPLE_QSPI, addr);

    cmd_write_enable();

#if defined(QSPI_CMD_REUSE_LUT) && QSPI_CMD_REUSE_LUT
    BOARD_QspiUpdateLUT(QSPI_CMD_SEQ_ERASE_SECTOR, QSPI_CMD_TYPE_ERASE_SECTOR);
#endif

    QSPI_ExecuteIPCommand(EXAMPLE_QSPI, QSPI_CMD_SEQ_ERASE_SECTOR);
    check_if_finished();

#if defined(FSL_FEATURE_QSPI_SOCCR_HAS_CLR_LPCAC) && (FSL_FEATURE_QSPI_SOCCR_HAS_CLR_LPCAC)
    QSPI_ClearCache(EXAMPLE_QSPI);
#endif
}

/* Erase all command */
void erase_all(void)
{
    while ((QSPI_GetStatusFlags(EXAMPLE_QSPI) & (uint32_t)kQSPI_Busy) != 0U)
    {
    }
    QSPI_SetIPCommandAddress(EXAMPLE_QSPI, FSL_FEATURE_QSPI_AMBA_BASE);
    cmd_write_enable();
    QSPI_ExecuteIPCommand(EXAMPLE_QSPI, QSPI_CMD_SEQ_ERASE_ALL);
    check_if_finished();
#if defined(FSL_FEATURE_QSPI_SOCCR_HAS_CLR_LPCAC) && (FSL_FEATURE_QSPI_SOCCR_HAS_CLR_LPCAC)
    QSPI_ClearCache(EXAMPLE_QSPI);
#endif
}

/* Program page into serial flash using QSPI polling way */
void program_page(uint32_t dest_addr, uint32_t *src_addr)
{
    uint32_t leftLongWords = 0;

    while ((QSPI_GetStatusFlags(EXAMPLE_QSPI) & (uint32_t)kQSPI_Busy) != 0U)
    {
    }
    QSPI_ClearFifo(EXAMPLE_QSPI, (uint32_t)kQSPI_TxFifo);

    QSPI_SetIPCommandAddress(EXAMPLE_QSPI, dest_addr);
    cmd_write_enable();
    while ((QSPI_GetStatusFlags(EXAMPLE_QSPI) & (uint32_t)kQSPI_Busy) != 0U)
    {
    }

    /* First write some data into TXFIFO to prevent from underrun */
    QSPI_WriteBlocking(EXAMPLE_QSPI, src_addr, ((uint32_t)FSL_FEATURE_QSPI_TXFIFO_DEPTH * sizeof(uint32_t)));
    src_addr += FSL_FEATURE_QSPI_TXFIFO_DEPTH;

    /* Start the program */
    QSPI_SetIPCommandSize(EXAMPLE_QSPI, FLASH_PAGE_SIZE);
    QSPI_ExecuteIPCommand(EXAMPLE_QSPI, QSPI_CMD_SEQ_PROGRAM_PAGE);

    leftLongWords = FLASH_PAGE_SIZE - ((uint32_t)FSL_FEATURE_QSPI_TXFIFO_DEPTH * sizeof(uint32_t));
    QSPI_WriteBlocking(EXAMPLE_QSPI, src_addr, leftLongWords);

    /* Wait until flash finished program */
    while ((QSPI_GetStatusFlags(EXAMPLE_QSPI) & (uint32_t)kQSPI_Busy) != 0U)
    {
    }
    QSPI_SetIPCommandSize(EXAMPLE_QSPI, 0);
    check_if_finished();
    while ((QSPI_GetStatusFlags(EXAMPLE_QSPI) & ((uint32_t)kQSPI_Busy | (uint32_t)kQSPI_IPAccess)) != 0U)
    {
    }

#if defined(FSL_FEATURE_QSPI_SOCCR_HAS_CLR_LPCAC) && (FSL_FEATURE_QSPI_SOCCR_HAS_CLR_LPCAC)
    QSPI_ClearCache(EXAMPLE_QSPI);
#endif
}

/* Use IP read to read flash data. */
void ip_read_flash(uint32_t addr, uint32_t *buffer, uint32_t size)
{
    uint32_t rxFifoSize = 4U * (uint32_t)FSL_FEATURE_QSPI_RXFIFO_DEPTH;
    uint32_t leftSize   = size;
    uint32_t curAddr    = addr;
    uint32_t *curBuff   = buffer;
    uint32_t transSize;

    while (leftSize != 0U)
    {
        transSize = (leftSize > rxFifoSize) ? rxFifoSize : leftSize;

        QSPI_ClearFifo(EXAMPLE_QSPI, (uint32_t)kQSPI_RxFifo);
        QSPI_SetIPCommandAddress(EXAMPLE_QSPI, curAddr);
        QSPI_SetIPCommandSize(EXAMPLE_QSPI, transSize);
        QSPI_ExecuteIPCommand(EXAMPLE_QSPI, QSPI_CMD_SEQ_READ);
        while ((QSPI_GetStatusFlags(EXAMPLE_QSPI) & (uint32_t)kQSPI_Busy) != 0U)
        {
        }
        QSPI_ReadBlocking(EXAMPLE_QSPI, curBuff, transSize);

        assert(curAddr <= (UINT32_MAX - transSize));
        assert((transSize % sizeof(*curBuff)) == 0U);
        curAddr += transSize;
        curBuff += ((size_t)transSize / sizeof(*curBuff));
        leftSize -= transSize;
    }
}

void qspi_nor_flash_init(QuadSPI_Type *base)
{
    uint32_t clockSourceFreq = 0;
    qspi_config_t config     = {0};

    /* Get QSPI default settings and configure the qspi */
    QSPI_GetDefaultQspiConfig(&config);

    /* Set AHB buffer size for reading data through AHB bus */
#if (FLASH_PAGE_SIZE <= FSL_FEATURE_QSPI_AHB_BUFFER_SIZE)
    config.AHBbufferSize[3] = FLASH_PAGE_SIZE;
#else
    config.AHBbufferSize[3] = FSL_FEATURE_QSPI_AHB_BUFFER_SIZE;
#endif
    clockSourceFreq = QSPI_CLK_FREQ;

    QSPI_Init(base, &config, clockSourceFreq);

#if defined(EXAMPLE_QSPI_HAS_SOC_CONFIG) && (EXAMPLE_QSPI_HAS_SOC_CONFIG)
    BOARD_QspiSocConfigure(EXAMPLE_QSPI);
#endif

#if defined(FLASH_NEED_DQS)
    /* Set DQS config */
    QSPI_SetDqsConfig(base, &dqsConfig);
#endif

    /* According to serial flash feature to configure flash settings */
    QSPI_SetFlashConfig(base, &single_config);

#if defined(FSL_FEATURE_QSPI_SOCCR_HAS_CLR_LPCAC) && (FSL_FEATURE_QSPI_SOCCR_HAS_CLR_LPCAC)
    QSPI_ClearCache(base);
#endif
}
