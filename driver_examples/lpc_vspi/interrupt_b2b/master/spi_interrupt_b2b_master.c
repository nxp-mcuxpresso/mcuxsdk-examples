/*
 * Copyright  2017 NXP
 * All rights reserved.
 *
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
static uint32_t txIndex             = BUFFER_SIZE;
static uint32_t rxIndex             = BUFFER_SIZE;
static volatile bool masterFinished = false;

/*******************************************************************************
 * Code
 ******************************************************************************/

void SPI_MASTER_IRQHandler(void)
{
    if ((SPI_GetStatusFlags(EXAMPLE_SPI_MASTER) & kSPI_RxReadyFlag) && (rxIndex > 0))
    {
        destBuff[BUFFER_SIZE - rxIndex] = SPI_ReadData(EXAMPLE_SPI_MASTER);
        rxIndex--;
    }
    if ((SPI_GetStatusFlags(EXAMPLE_SPI_MASTER) & kSPI_TxReadyFlag) && (txIndex > 0))
    {
        if (txIndex == 1)
        {
            /* need to disable interrupts before write last data */
            SPI_DisableInterrupts(EXAMPLE_SPI_MASTER, kSPI_TxReadyInterruptEnable);
            SPI_WriteData(EXAMPLE_SPI_MASTER, (uint16_t)(srcBuff[BUFFER_SIZE - txIndex]), kSPI_FrameAssert);
        }
        else
        {
            SPI_WriteData(EXAMPLE_SPI_MASTER, (uint16_t)(srcBuff[BUFFER_SIZE - txIndex]), 0);
        }
        txIndex--;
    }
    if ((txIndex == 0U) && (rxIndex == 0U))
    {
        masterFinished = true;
        SPI_DisableInterrupts(EXAMPLE_SPI_MASTER, kSPI_RxReadyInterruptEnable);
    }
    SDK_ISR_EXIT_BARRIER;
}

int main(void)
{
    spi_master_config_t masterConfig = {0};
    uint32_t sourceClock             = 0U;
    uint32_t i                       = 0U;
    uint32_t err                     = 0U;

    /* Init the boards */
    BOARD_InitHardware();

    PRINTF("\r\nSPI board to board interrupt master example started!\r\n");

    /* Init SPI master */
    /*
     * masterConfig->enableLoopback = false;
     * masterConfig->enableMaster = true;
     * masterConfig->polarity = kSPI_ClockPolarityActiveHigh;
     * masterConfig->phase = kSPI_ClockPhaseFirstEdge;
     * masterConfig->direction = kSPI_MsbFirst;
     * masterConfig->baudRate_Bps = 500000U;
     */
    SPI_MasterGetDefaultConfig(&masterConfig);
    sourceClock          = EXAMPLE_SPI_MASTER_CLK_FREQ;
    masterConfig.sselNum = (spi_ssel_t)EXAMPLE_SPI_SSEL;
    masterConfig.sselPol = (spi_spol_t)EXAMPLE_SPI_SPOL;
    SPI_MasterInit(EXAMPLE_SPI_MASTER, &masterConfig, sourceClock);

    /* Init source buffer */
    for (i = 0U; i < BUFFER_SIZE; i++)
    {
        srcBuff[i] = i;
    }

    /* Enable interrupt, first enable slave and then master. */
    EnableIRQ(EXAMPLE_SPI_MASTER_IRQ);
    SPI_EnableInterrupts(EXAMPLE_SPI_MASTER, kSPI_TxReadyInterruptEnable | kSPI_RxReadyInterruptEnable);

    while (masterFinished != true)
    {
    }

    /* Check the data received */
    for (i = 0U; i < BUFFER_SIZE; i++)
    {
        if (destBuff[i] != srcBuff[i])
        {
            PRINTF("\r\nThe %d data is wrong, the data received is %d \r\n", i, destBuff[i]);
            err++;
        }
    }
    if (err == 0U)
    {
        PRINTF("\r\nSPI transfer finished!\r\n");
    }

    while (1)
    {
    }
}
