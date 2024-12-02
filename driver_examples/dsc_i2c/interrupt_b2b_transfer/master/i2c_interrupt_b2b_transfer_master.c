/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*  Standard C Included Files */
#include <string.h>
/*  SDK Included Files */
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_i2c.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define I2C_MASTER_SLAVE_ADDR_7BIT 0x7EU
#define I2C_DATA_LENGTH            33U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static void i2c_master_callback(i2c_master_transfer_handle_t *psHandle);

/*******************************************************************************
 * Variables
 ******************************************************************************/

uint8_t g_master_txBuff[I2C_DATA_LENGTH];
uint8_t g_master_rxBuff[I2C_DATA_LENGTH];
i2c_master_transfer_handle_t g_m_handle;
volatile bool g_MasterCompletionFlag = false;

/*******************************************************************************
 * Code
 ******************************************************************************/

static void i2c_master_callback(i2c_master_transfer_handle_t *psHandle)
{
    /* Signal transfer success when received success status. */
    if (I2C_GET_TRANSFER_COMPLETION_STATUS(psHandle) == kStatus_Success)
    {
        g_MasterCompletionFlag = true;
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    i2c_master_config_t masterConfig;
    uint32_t sourceClock;
    i2c_master_transfer_t masterXfer;

    BOARD_InitHardware();

    PRINTF("\r\nI2C board2board interrupt example -- Master transfer.\r\n");

    /* Set up i2c master to send data to slave*/
    /* First data in txBuff is data length of the transmiting data. */
    g_master_txBuff[0] = I2C_DATA_LENGTH - 1U;
    for (uint32_t i = 1U; i < I2C_DATA_LENGTH; i++)
    {
        g_master_txBuff[i] = i - 1;
    }

    PRINTF("Master will send data :");
    for (uint32_t i = 0U; i < I2C_DATA_LENGTH - 1U; i++)
    {
        if (i % 8 == 0)
        {
            PRINTF("\r\n");
        }
        PRINTF("0x%2x  ", g_master_txBuff[i + 1]);
    }
    PRINTF("\r\n\r\n");

    /*
     * masterConfig->baudRate_Bps = 100000U;
     * masterConfig->enableStopHold = false;
     * masterConfig->glitchFilterWidth = 0U;
     * masterConfig->enableMaster = true;
     */
    I2C_MasterGetDefaultConfig(&masterConfig, I2C_MASTER_CLK_FREQ);
    I2C_MasterInit(EXAMPLE_I2C_MASTER_BASEADDR, &masterConfig);

    memset(&g_m_handle, 0, sizeof(g_m_handle));
    memset(&masterXfer, 0, sizeof(masterXfer));

    /* pu8Command = 0x01, data = g_master_txBuff - write to slave.
      start + u8SlaveAddress(w) + pu8Command + length of data buffer + data buffer + stop*/
    uint8_t deviceAddress        = 0x01U;
    masterXfer.u8SlaveAddress    = I2C_MASTER_SLAVE_ADDR_7BIT;
    masterXfer.eDirection        = kI2C_MasterTransmit;
    masterXfer.pu8Command        = &deviceAddress;
    masterXfer.u8CommandSize     = 1;
    masterXfer.pu8Data           = g_master_txBuff;
    masterXfer.u16DataSize       = I2C_DATA_LENGTH;
    masterXfer.u8ControlFlagMask = kI2C_TransferStartStopFlag;

    I2C_MasterTransferCreateHandle(EXAMPLE_I2C_MASTER_BASEADDR, &g_m_handle, i2c_master_callback, NULL);
    I2C_MasterTransferNonBlocking(&g_m_handle, &masterXfer);

    /*  Wait for transfer completed. */
    while (!g_MasterCompletionFlag)
    {
    }
    g_MasterCompletionFlag = false;

    PRINTF("Receive sent data from slave :");

    /* pu8Command = 0x01, data = g_master_rxBuff - read from slave.
      start + u8SlaveAddress(w) + pu8Command + repeated start + u8SlaveAddress(r) + rx data buffer + stop */
    masterXfer.u8SlaveAddress    = I2C_MASTER_SLAVE_ADDR_7BIT;
    masterXfer.eDirection        = kI2C_MasterReceive;
    masterXfer.pu8Command        = &deviceAddress;
    masterXfer.u8CommandSize     = 1;
    masterXfer.pu8Data           = g_master_rxBuff;
    masterXfer.u16DataSize       = I2C_DATA_LENGTH - 1;
    masterXfer.u8ControlFlagMask = kI2C_TransferStartStopFlag;

    I2C_MasterTransferNonBlocking(&g_m_handle, &masterXfer);

    /*  Reset master completion flag to false. */
    g_MasterCompletionFlag = false;

    /*  Wait for transfer completed. */
    while (!g_MasterCompletionFlag)
    {
    }
    g_MasterCompletionFlag = false;

    for (uint32_t i = 0U; i < I2C_DATA_LENGTH - 1; i++)
    {
        if (i % 8 == 0)
        {
            PRINTF("\r\n");
        }
        PRINTF("0x%2x  ", g_master_rxBuff[i]);
    }
    PRINTF("\r\n\r\n");

    /* Transfer completed. Check the data.*/
    for (uint32_t i = 0U; i < I2C_DATA_LENGTH - 1; i++)
    {
        if (g_master_rxBuff[i] != g_master_txBuff[i + 1])
        {
            PRINTF("\r\nError occurred in the transfer ! \r\n");
            break;
        }
    }

    PRINTF("\r\nEnd of I2C example .\r\n");
    while (1)
    {
    }
}
