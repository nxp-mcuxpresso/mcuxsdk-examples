/*
 * Copyright  2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_spi.h"
#include "board.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void EXAMPLE_SPIMasterInit(void);
static void EXAMPLE_MasterStartTransfer(void);
static void EXAMPLE_TransferDataCheck(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static uint8_t txBuffer[BUFFER_SIZE];
static uint8_t rxBuffer[BUFFER_SIZE];
static uint32_t txIndex            = BUFFER_SIZE;
static uint32_t rxIndex            = BUFFER_SIZE;
static volatile bool slaveFinished = false;

/*******************************************************************************
 * Code
 ******************************************************************************/

void SPI_MASTER_IRQHandler(void)
{
    /* Check if data is ready in RX register. */
    if ((SPI_GetStatusFlags(EXAMPLE_SPI_MASTER) & kSPI_RxFifoNotEmptyFlag))
    {
        rxBuffer[BUFFER_SIZE - rxIndex] = SPI_ReadData(EXAMPLE_SPI_MASTER);
        rxIndex--;
    }
    /* Write data to TX regsiter if TX register is ready. */
    if ((SPI_GetInterruptStatusFlags(EXAMPLE_SPI_MASTER) & kSPI_TxFifoHalfEmptyInterruptFlag) && (txIndex != 0U))
    {
        SPI_WriteData(EXAMPLE_SPI_MASTER, (uint16_t)(txBuffer[BUFFER_SIZE - txIndex]));
        txIndex--;
    }
    /* If no data to be transferred, disable the interrupt. */
    if ((txIndex == 0U) && (rxIndex == 0U))
    {
        slaveFinished = true;
        SPI_DisableInterrupts(EXAMPLE_SPI_MASTER, kSPI_TxFifoHalfEmptyInterruptEnable);
    }
}

int main(void)
{
    /* Initialize the boards */
    BOARD_InitHardware();

    /* Initialize the SPI master with configuration. */
    EXAMPLE_SPIMasterInit();

    /* Start transfer with slave board. */
    EXAMPLE_MasterStartTransfer();

    /* Check the received data. */
    EXAMPLE_TransferDataCheck();

    /* De-initialize the SPI master. */
    SPI_Deinit(EXAMPLE_SPI_MASTER);

    while (1)
    {
    }
}

static void EXAMPLE_SPIMasterInit(void)
{
    spi_master_config_t masterConfig = {0};
    uint32_t srcFreq                 = 0U;

    /* configuration from using SPI_MasterGetDefaultConfig():
     * userConfig->enableLoopback = false;
     * userConfig->enableMaster = true;
     * userConfig->polarity = kSPI_ClockPolarityActiveHigh;
     * userConfig->phase = kSPI_ClockPhaseFirstEdge;
     * userConfig->baudRate_Bps = 500000U;
     * userConfig->dataWidth = 8U;
     */
    SPI_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate_Bps = EXAMPLE_SPI_MASTER_BAUDRATE;
    srcFreq                   = EXAMPLE_SPI_MASTER_CLK_FREQ;
    SPI_MasterInit(EXAMPLE_SPI_MASTER, &masterConfig, srcFreq);
}

static void EXAMPLE_MasterStartTransfer(void)
{
    uint32_t i = 0U;

    /* Init source buffer */
    for (i = 0U; i < BUFFER_SIZE; i++)
    {
        txBuffer[i] = i;
        rxBuffer[i] = 0U;
    }

    /* Enable SPI receive ready interrupt. */
    EnableIRQ(EXAMPLE_SPI_MASTER_IRQ);
    /* Enable SPI TX half empty interrupt. */
    SPI_EnableInterrupts(EXAMPLE_SPI_MASTER, kSPI_TxFifoHalfEmptyInterruptEnable);
}

static void EXAMPLE_TransferDataCheck(void)
{
    uint32_t i = 0U, err = 0U;
    /* Waiting for the transmission complete. */
    while (slaveFinished != true)
    {
    }

    /*Check if the data is right*/
    for (i = 0; i < BUFFER_SIZE; i++)
    {
        /* Check if data matched. */
        if (txBuffer[i] != rxBuffer[i])
        {
            err++;
        }
    }

    if (0U == err)
    {
        BOARD_LedMatrixShowNumber(0, 0, false);
    }
    else
    {
        BOARD_LedMatrixShowNumber(1, 0, false);
    }
}
