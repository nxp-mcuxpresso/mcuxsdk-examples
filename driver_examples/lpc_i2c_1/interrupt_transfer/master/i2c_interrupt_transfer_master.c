/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*  Standard C Included Files */
#include <stdio.h>
#include <string.h>
#include "board.h"
#include "fsl_i2c.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define I2C_MASTER_SLAVE_ADDR_7BIT (0x7EU)
#define I2C_BAUDRATE               (100000) /* 100K */
#define I2C_DATA_LENGTH            (33)     /* MAX is 256 */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

uint8_t g_master_txBuff[I2C_DATA_LENGTH];
uint8_t g_master_rxBuff[I2C_DATA_LENGTH];

i2c_master_handle_t g_m_handle;

volatile bool g_MasterCompletionFlag = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
static void i2c_master_callback(I2C0_Type *base, i2c_master_handle_t *handle, status_t status, void *userData)
{
    /* Signal transfer success when received success status. */
    if (status == kStatus_Success)
    {
        g_MasterCompletionFlag = true;
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    i2c_master_transfer_t masterXfer = {0};
    status_t reVal                   = kStatus_Fail;

    BOARD_InitHardware();

    /* Set up i2c master to send data to slave*/
    /* First data in txBuff is data length of the transmiting data. */
    g_master_txBuff[0] = I2C_DATA_LENGTH - 1U;
    for (uint32_t i = 1U; i < I2C_DATA_LENGTH; i++)
    {
        g_master_txBuff[i] = i - 1;
    }

    i2c_master_config_t masterConfig;

    /*
     *  masterConfig.enableMaster = true;
     *  masterConfig.baudRate_Bps = 100000U;
     */
    I2C_MasterGetDefaultConfig(&masterConfig);

    /* Change the default baudrate configuration */
    masterConfig.baudRate_Bps = I2C_BAUDRATE;

    /* Initialize the I2C master peripheral */
    I2C_MasterInit(EXAMPLE_I2C_MASTER, &masterConfig, I2C_MASTER_CLOCK_FREQUENCY);

    /* Create the I2C handle for the non-blocking transfer */
    I2C_MasterTransferCreateHandle(EXAMPLE_I2C_MASTER, &g_m_handle, i2c_master_callback, NULL);

    /* subAddress = 0x01, data = g_master_txBuff - write to slave.
      start + slaveaddress(w) + subAddress + length of data buffer + data buffer + stop*/
    uint8_t deviceAddress     = 0x01U;
    masterXfer.slaveAddress   = I2C_MASTER_SLAVE_ADDR_7BIT;
    masterXfer.direction      = kI2C_Write;
    masterXfer.subaddress     = (uint32_t)deviceAddress;
    masterXfer.subaddressSize = 1;
    masterXfer.data           = g_master_txBuff;
    masterXfer.dataSize       = I2C_DATA_LENGTH;
    masterXfer.flags          = kI2C_TransferDefaultFlag;

    /* Send master non-blocking data to slave */
    reVal = I2C_MasterTransferNonBlocking(EXAMPLE_I2C_MASTER, &g_m_handle, &masterXfer);

    /*  Reset master completion flag to false. */
    g_MasterCompletionFlag = false;

    if (reVal != kStatus_Success)
    {
        return -1;
    }

    /*  Wait for transfer completed. */
    while (!g_MasterCompletionFlag)
    {
    }
    g_MasterCompletionFlag = false;

    /* subAddress = 0x01, data = g_master_rxBuff - read from slave.
      start + slaveaddress(w) + subAddress + repeated start + slaveaddress(r) + rx data buffer + stop */
    masterXfer.slaveAddress   = I2C_MASTER_SLAVE_ADDR_7BIT;
    masterXfer.direction      = kI2C_Read;
    masterXfer.subaddress     = (uint32_t)deviceAddress;
    masterXfer.subaddressSize = 1;
    masterXfer.data           = g_master_rxBuff;
    masterXfer.dataSize       = I2C_DATA_LENGTH - 1;
    masterXfer.flags          = kI2C_TransferDefaultFlag;

    reVal = I2C_MasterTransferNonBlocking(EXAMPLE_I2C_MASTER, &g_m_handle, &masterXfer);

    /*  Reset master completion flag to false. */
    g_MasterCompletionFlag = false;

    if (reVal != kStatus_Success)
    {
        return -1;
    }

    /*  Wait for transfer completed. */
    while (!g_MasterCompletionFlag)
    {
    }
    g_MasterCompletionFlag = false;

    /* Transfer completed. Check the data.*/
    for (uint32_t i = 0U; i < I2C_DATA_LENGTH - 1; i++)
    {
        if (g_master_rxBuff[i] != g_master_txBuff[i + 1])
        {
            BOARD_LedMatrixShowNumber(1, 0, false);
            break;
        }
    }

    BOARD_LedMatrixShowNumber(0, 0, false);
    while (1)
    {
    }
}
