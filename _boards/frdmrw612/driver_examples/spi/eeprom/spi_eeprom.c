/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_spi.h"
#include "board.h"
#include "app.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BUFFER_SIZE (64)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
static uint8_t srcBuff[BUFFER_SIZE];
static uint8_t destBuff[BUFFER_SIZE];

/*******************************************************************************
 * Code
 ******************************************************************************/
static uint8_t eeprom_read_status(SPI_Type *base)
{
    spi_transfer_t xfer = {0};
    uint8_t wdata[2]    = {0x5U, 0};
    uint8_t rdata[2];

    /*Start Transfer*/
    xfer.txData      = wdata;
    xfer.rxData      = rdata;
    xfer.dataSize    = sizeof(rdata);
    xfer.configFlags = kSPI_FrameDelay | kSPI_FrameAssert;
    SPI_MasterTransferBlocking(base, &xfer);

    return rdata[1];
}

static void eeprom_write_status(SPI_Type *base, uint8_t status)
{
    spi_transfer_t xfer = {0};
    uint8_t data[2]     = {0x1U, status};

    /*Start Transfer*/
    xfer.txData      = data;
    xfer.rxData      = NULL;
    xfer.dataSize    = sizeof(data);
    xfer.configFlags = kSPI_FrameDelay | kSPI_FrameAssert;
    SPI_MasterTransferBlocking(base, &xfer);

    data[0] = eeprom_read_status(base);
    if (data[0] != status)
    {
        PRINTF("EEPROM write status is not set Properly ........");
    }
}

static void eeprom_write_enable(SPI_Type *base)
{
    uint32_t status;
    spi_transfer_t xfer = {0};
    uint8_t data        = 0x6U;

    /*Start Transfer*/
    xfer.txData      = &data;
    xfer.rxData      = NULL;
    xfer.dataSize    = sizeof(data);
    xfer.configFlags = kSPI_FrameDelay | kSPI_FrameAssert;
    SPI_MasterTransferBlocking(base, &xfer);

    status = eeprom_read_status(base);
    if ((status & 2U) != 2U)
    {
        PRINTF("EEPROM Write-Enable is not set Properly ........");
    }
}

static void eeprom_write_disable(SPI_Type *base)
{
    uint32_t status;
    spi_transfer_t xfer = {0};
    uint8_t data        = 0x4U;

    /*Start Transfer*/
    xfer.txData      = &data;
    xfer.rxData      = NULL;
    xfer.dataSize    = sizeof(data);
    xfer.configFlags = kSPI_FrameDelay | kSPI_FrameAssert;
    SPI_MasterTransferBlocking(base, &xfer);

    status = eeprom_read_status(base);
    if ((status & 2U) == 2U)
    {
        PRINTF("EEPROM Write-Enable is not set Properly ........");
    }
}

static void eeprom_write(SPI_Type *base, uint32_t addr, uint8_t *data, uint32_t len)
{
    uint32_t status;
    spi_transfer_t xfer = {0};
    uint8_t cmd[4]      = {0x2U, (addr & 0xFF0000U) >> 16, (addr & 0xFF00U) >> 8, addr & 0xFF};

    /*Start Transfer*/
    xfer.txData      = cmd;
    xfer.rxData      = NULL;
    xfer.dataSize    = sizeof(cmd);
    xfer.configFlags = kSPI_FrameDelay;
    SPI_MasterTransferBlocking(base, &xfer);

    xfer.txData      = data;
    xfer.rxData      = NULL;
    xfer.dataSize    = len;
    xfer.configFlags = kSPI_FrameDelay | kSPI_FrameAssert;
    SPI_MasterTransferBlocking(base, &xfer);

    /* Wait eeprom ready */
    do
    {
        status = eeprom_read_status(EXAMPLE_SPI_MASTER);
    } while ((status & 0x1U) != 0);
}

static void eeprom_read(SPI_Type *base, uint32_t addr, uint8_t *data, uint32_t len)
{
    uint32_t status;
    spi_transfer_t xfer = {0};
    uint8_t cmd[4]      = {0x3U, (addr & 0xFF0000U) >> 16, (addr & 0xFF00U) >> 8, addr & 0xFF};

    /*Start Transfer*/
    xfer.txData      = cmd;
    xfer.rxData      = NULL;
    xfer.dataSize    = sizeof(cmd);
    xfer.configFlags = kSPI_FrameDelay;
    SPI_MasterTransferBlocking(base, &xfer);

    xfer.txData      = NULL;
    xfer.rxData      = data;
    xfer.dataSize    = len;
    xfer.configFlags = kSPI_FrameDelay | kSPI_FrameAssert;
    SPI_MasterTransferBlocking(base, &xfer);

    /* Wait eeprom ready */
    do
    {
        status = eeprom_read_status(EXAMPLE_SPI_MASTER);
    } while ((status & 0x1U) != 0);
}

int main(void)
{
    spi_master_config_t userConfig = {0};
    uint32_t srcFreq               = 0;
    uint32_t status;
    uint32_t i;

    BOARD_InitHardware();
    PRINTF("\n\rMaster Start...\n\r");
    /*
     * userConfig.enableLoopback = false;
     * userConfig.enableMaster = true;
     * userConfig.polarity = kSPI_ClockPolarityActiveHigh;
     * userConfig.phase = kSPI_ClockPhaseFirstEdge;
     * userConfig.direction = kSPI_MsbFirst;
     * userConfig.baudRate_Bps = 500000U;
     */
    SPI_MasterGetDefaultConfig(&userConfig);
    srcFreq            = EXAMPLE_SPI_MASTER_CLK_FREQ;
    userConfig.sselNum = (spi_ssel_t)EXAMPLE_SPI_SSEL;
    userConfig.sselPol = (spi_spol_t)EXAMPLE_SPI_SPOL;
    SPI_MasterInit(EXAMPLE_SPI_MASTER, &userConfig, srcFreq);

    status = eeprom_read_status(EXAMPLE_SPI_MASTER);
    if ((status & 2U) == 2U)
    {
        PRINTF("EEPROM is Write-Enabled ........");
    }
    else
    {
        eeprom_write_enable(EXAMPLE_SPI_MASTER);
    }

    /* Init Buffer*/
    for (i = 0; i < BUFFER_SIZE; i++)
    {
        srcBuff[i] = i;
    }

    eeprom_write(EXAMPLE_SPI_MASTER, 0x100, srcBuff, sizeof(srcBuff));
    eeprom_read(EXAMPLE_SPI_MASTER, 0x100, destBuff, sizeof(destBuff));

    /*Check if the data is right*/
    for (i = 0; i < BUFFER_SIZE; i++)
    {
        if (srcBuff[i] != destBuff[i])
        {
            PRINTF("The %d is wrong! data is %d\n\r", i, destBuff[i]);
        }
    }

    eeprom_write_enable(EXAMPLE_SPI_MASTER);
    eeprom_write_status(EXAMPLE_SPI_MASTER, 0x4U);
    eeprom_write_disable(EXAMPLE_SPI_MASTER);

    while (1)
    {
    }
}
