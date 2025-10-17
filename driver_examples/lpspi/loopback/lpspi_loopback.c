/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017,2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_lpspi.h"
#include "board.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define TRANSFER_SIZE     (128U)    /*! Transfer dataSize .*/
#define TRANSFER_BAUDRATE (500000U) /*! Transfer baudrate - 500k */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*******************************************************************************
 * Code
 ******************************************************************************/
static void LPSPI_Init(void)
{
    lpspi_master_config_t masterConfig;

    /* Master config */
    LPSPI_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate                      = TRANSFER_BAUDRATE;
    masterConfig.bitsPerFrame                  = 8;
    masterConfig.cpol                          = kLPSPI_ClockPolarityActiveHigh;
    masterConfig.cpha                          = kLPSPI_ClockPhaseFirstEdge;
    masterConfig.direction                     = kLPSPI_MsbFirst;
    masterConfig.whichPcs                      = EXAMPLE_LPSPI_PCS_FOR_INT;
    masterConfig.pcsActiveHighOrLow            = kLPSPI_PcsActiveLow;
    masterConfig.pinCfg                        = kLPSPI_SdiInSdoOut;
    masterConfig.dataOutConfig                 = kLpspiDataOutTristate;
    masterConfig.pcsToSckDelayInNanoSec        = 1000000000 / masterConfig.baudRate;
    masterConfig.lastSckToPcsDelayInNanoSec    = 1000000000 / masterConfig.baudRate;
    masterConfig.betweenTransferDelayInNanoSec = 1000000000 / masterConfig.baudRate;
    masterConfig.enableInputDelay              = false;

    LPSPI_MasterInit(EXAMPLE_LPSPI_BASEADDR, &masterConfig, EXAMPLE_LPSPI_CLOCK_FREQ);
}

/*!
 * @brief Main function
 */
int main(void)
{
    uint8_t i;
    lpspi_transfer_t masterXfer;
    uint8_t masterRxData[TRANSFER_SIZE] = {0};
    uint8_t masterTxData[TRANSFER_SIZE] = {0};

    /* Init board hardware. */
    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

    PRINTF("\r\nLPSPI loopback example start\r\n");
    LPSPI_Init();

    for (i = 0; i < TRANSFER_SIZE; i++)
    {
        masterTxData[i] = i;
    }
    /*Start master transfer*/
    masterXfer.txData      = masterTxData;
    masterXfer.rxData      = masterRxData;
    masterXfer.dataSize    = TRANSFER_SIZE;
    masterXfer.configFlags = EXAMPLE_LPSPI_PCS_FOR_TRANS | kLPSPI_MasterPcsContinuous;
    LPSPI_MasterTransferBlocking(EXAMPLE_LPSPI_BASEADDR, &masterXfer);

    /* Compare Tx and Rx data. */
    for (i = 0; i < TRANSFER_SIZE; i++)
    {
        if (masterTxData[i] != masterRxData[i])
        {
            break;
        }
    }

    if (TRANSFER_SIZE == i)
    {
        PRINTF("LPSPI loopback test pass!!!");
    }
    else
    {
        PRINTF("LPSPI loopback test fail!!!");
    }

    while (1)
    {
        __NOP();
    }
}
