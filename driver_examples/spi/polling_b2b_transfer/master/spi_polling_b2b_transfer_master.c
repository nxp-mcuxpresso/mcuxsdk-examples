/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
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

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
#define BUFFER_SIZE (64)
static uint8_t srcBuff[BUFFER_SIZE];
static uint8_t destBuff[BUFFER_SIZE];
/*******************************************************************************
 * Code
 ******************************************************************************/

int main(void)
{
    spi_master_config_t userConfig = {0};
    uint32_t srcFreq               = 0;
    uint32_t i                     = 0;
    uint32_t err                   = 0;
    spi_transfer_t xfer            = {0};

    BOARD_InitHardware();
    PRINTF("\n\rMaster Start...\n\r");
    /*
     * userConfig.enableStopInWaitMode = false;
     * userConfig.polarity = kSPI_ClockPolarityActiveHigh;
     * userConfig.phase = kSPI_ClockPhaseFirstEdge;
     * userConfig.direction = kSPI_MsbFirst;
     * userConfig.dataMode = kSPI_8BitMode;
     * userConfig.txWatermark = kSPI_TxFifoOneHalfEmpty;
     * userConfig.rxWatermark = kSPI_RxFifoOneHalfFull;
     * userConfig.pinMode = kSPI_PinModeNormal;
     * userConfig.outputMode = kSPI_SlaveSelectAutomaticOutput;
     * userConfig.baudRate_Bps = 500000U;
     */
    SPI_MasterGetDefaultConfig(&userConfig);
    srcFreq = EXAMPLE_SPI_MASTER_CLK_FREQ;
    SPI_MasterInit(EXAMPLE_SPI_MASTER, &userConfig, srcFreq);

    /* Init Buffer*/
    for (i = 0; i < BUFFER_SIZE; i++)
    {
        srcBuff[i] = i;
    }

    /*Start Transfer*/
    xfer.txData   = srcBuff;
    xfer.rxData   = destBuff;
    xfer.dataSize = BUFFER_SIZE;
    SPI_MasterTransferBlocking(EXAMPLE_SPI_MASTER, &xfer);

    /*Check if the data is right*/
    for (i = 0; i < BUFFER_SIZE; i++)
    {
        if (srcBuff[i] != destBuff[i])
        {
            err++;
            PRINTF("The %d is wrong! data is %d\n\r", i, destBuff[i]);
        }
    }
    if (err == 0)
    {
        PRINTF("Succeed!\n\r");
    }

    while (1)
    {
    }
}
