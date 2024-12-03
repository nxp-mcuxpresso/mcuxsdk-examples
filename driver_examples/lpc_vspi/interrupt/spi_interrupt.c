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
static volatile uint32_t masterIndex = BUFFER_SIZE;
static volatile uint32_t slaveIndex  = BUFFER_SIZE;
static volatile bool masterFinished  = false;
static volatile bool slaveFinished   = false;

/*******************************************************************************
 * Code
 ******************************************************************************/

void SPI_MASTER_IRQHandler(void)
{
    /* Read data to avoid rxOverflow */
    if (SPI_GetStatusFlags(EXAMPLE_SPI_MASTER) & kSPI_RxReadyFlag)
    {
        (void)SPI_ReadData(EXAMPLE_SPI_MASTER);
    }
    /* Send data if buffer is not full */
    if (SPI_GetStatusFlags(EXAMPLE_SPI_MASTER) & kSPI_TxReadyFlag)
    {
        if (masterIndex <= 1)
        {
            /* If you do not want to receive the data, use kSPI_ReceiveIgnore to ignore it. */
            SPI_WriteData(EXAMPLE_SPI_MASTER, (uint16_t)(srcBuff[BUFFER_SIZE - masterIndex]),
                          kSPI_FrameAssert | kSPI_ReceiveIgnore);
        }
        else
        {
            SPI_WriteData(EXAMPLE_SPI_MASTER, (uint16_t)(srcBuff[BUFFER_SIZE - masterIndex]), 0U);
        }
        masterIndex--;
    }
    if (masterIndex == 0U)
    {
        masterFinished = true;
        SPI_DisableInterrupts(EXAMPLE_SPI_MASTER, kSPI_TxReadyInterruptEnable);
    }
    SDK_ISR_EXIT_BARRIER;
}

void SPI_SLAVE_IRQHandler(void)
{
    /* Fill tx register with dummy pattern */
    if (SPI_GetStatusFlags(EXAMPLE_SPI_SLAVE) & kSPI_TxReadyFlag)
    {
        SPI_WriteData(EXAMPLE_SPI_SLAVE, 0xFFFF, 0U);
    }
    /* Read rxFIFO if is not empty */
    if (SPI_GetStatusFlags(EXAMPLE_SPI_SLAVE) & kSPI_RxReadyFlag)
    {
        destBuff[BUFFER_SIZE - slaveIndex] = SPI_ReadData(EXAMPLE_SPI_SLAVE);
        slaveIndex--;
    }
    if (slaveIndex == 0U)
    {
        slaveFinished = true;
        SPI_DisableInterrupts(EXAMPLE_SPI_SLAVE, kSPI_TxReadyInterruptEnable | kSPI_RxReadyInterruptEnable);
    }
    SDK_ISR_EXIT_BARRIER;
}

int main(void)
{
    spi_master_config_t masterConfig = {0};
    spi_slave_config_t slaveConfig   = {0};
    uint32_t sourceClock             = 0U;
    uint32_t i                       = 0U;
    uint32_t err                     = 0U;

    /* Init the boards */
    BOARD_InitHardware();

    PRINTF("\r\nSPI one board interrupt example started!\r\n");

    /* Init SPI master */
    /*
     * masterConfig.enableLoopback = false;
     * masterConfig.enableMaster = true;
     * masterConfig.polarity = kSPI_ClockPolarityActiveHigh;
     * masterConfig.phase = kSPI_ClockPhaseFirstEdge;
     * masterConfig.direction = kSPI_MsbFirst;
     * masterConfig.baudRate_Bps = 500000U;
     */
    SPI_MasterGetDefaultConfig(&masterConfig);
    sourceClock          = EXAMPLE_SPI_MASTER_CLK_FREQ;
    masterConfig.sselNum = (spi_ssel_t)EXAMPLE_SPI_SSEL;
    masterConfig.sselPol = (spi_spol_t)EXAMPLE_SPI_SPOL;
    SPI_MasterInit(EXAMPLE_SPI_MASTER, &masterConfig, sourceClock);
    /* Init SPI slave */
    /*
     * slaveConfig.enableSlave = true;
     * slaveConfig.polarity = kSPI_ClockPolarityActiveHigh;
     * slaveConfig.phase = kSPI_ClockPhaseFirstEdge;
     * slaveConfig.direction = kSPI_MsbFirst;
     */
    SPI_SlaveGetDefaultConfig(&slaveConfig);
    slaveConfig.sselPol = (spi_spol_t)EXAMPLE_SPI_SPOL;
    SPI_SlaveInit(EXAMPLE_SPI_SLAVE, &slaveConfig);
    /* Set priority, slave have higher priority */
    NVIC_SetPriority(EXAMPLE_SPI_MASTER_IRQ, 1U);
    NVIC_SetPriority(EXAMPLE_SPI_SLAVE_IRQ, 0U);

    /* Init source buffer */
    for (i = 0U; i < BUFFER_SIZE; i++)
    {
        srcBuff[i] = i;
    }

    /* Enable interrupt, first enable slave and then master. */
    EnableIRQ(EXAMPLE_SPI_MASTER_IRQ);
    EnableIRQ(EXAMPLE_SPI_SLAVE_IRQ);
    SPI_EnableInterrupts(EXAMPLE_SPI_SLAVE, kSPI_RxReadyInterruptEnable | kSPI_TxReadyInterruptEnable);
    SPI_EnableInterrupts(EXAMPLE_SPI_MASTER, kSPI_TxReadyInterruptEnable);

    while ((masterFinished != true) || (slaveFinished != true))
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
