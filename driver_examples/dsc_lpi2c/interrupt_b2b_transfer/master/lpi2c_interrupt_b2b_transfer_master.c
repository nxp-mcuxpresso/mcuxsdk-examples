/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_lpi2c.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define I2C_MASTER_SLAVE_ADDR_7BIT 0x7EU
#define I2C_BAUDRATE               100000U
#define I2C_DATA_LENGTH            33U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

uint8_t g_master_txBuff[I2C_DATA_LENGTH];
uint8_t g_master_rxBuff[I2C_DATA_LENGTH];
lpi2c_master_transfer_handle_t g_m_handle;
volatile bool g_MasterCompletionFlag = false;

/*******************************************************************************
 * Code
 ******************************************************************************/

static void lpi2c_master_callback(lpi2c_master_transfer_handle_t *psHandle)
{
    status_t status = LPI2C_GET_TRANSFER_COMPLETION_STATUS(psHandle);
    /* Display failure information when status is not success. */
    if (status != kStatus_Success)
    {
        PRINTF("Master transfer failed with status %d: \r\n", status);
    }
    g_MasterCompletionFlag = true;
}

/*!
 * @brief Main function
 */
int main(void)
{
    lpi2c_master_config_t sMasterConfig;
    lpi2c_master_transfer_t sMasterXfer = {0};
    uint8_t deviceAddress               = 0x01U;
    status_t reVal                      = kStatus_Fail;

    BOARD_InitHardware();

    PRINTF("\r\nLPI2C board2board interrupt example -- Master transfer.\r\n");

    /* Set up i2c master to send data to slave*/
    /* First data in txBuff is data length of the transmiting data. */
    g_master_txBuff[0] = I2C_DATA_LENGTH - 1U;
    for (uint32_t i = 1U; i < I2C_DATA_LENGTH; i++)
    {
        g_master_txBuff[i] = i - 1;
    }

    PRINTF("Master will send data :");
    for (uint32_t i = 0U; i < I2C_DATA_LENGTH - 1; i++)
    {
        if (i % 8 == 0)
        {
            PRINTF("\r\n");
        }
        PRINTF("0x%2x  ", g_master_txBuff[i + 1]);
    }
    PRINTF("\r\n\r\n");

    /* Get basic configuration */
    LPI2C_MasterGetDefaultConfig(&sMasterConfig, LPI2C_MASTER_CLOCK_FREQUENCY);

    /* Change the default baudrate configuration */
    sMasterConfig.u32BaudRateBps = I2C_BAUDRATE;

    /* Initialize the LPI2C master peripheral */
    LPI2C_MasterInit(EXAMPLE_LPI2C_MASTER, &sMasterConfig);

    /* Create the LPI2C handle for the non-blocking transfer */
    LPI2C_MasterTransferCreateHandle(EXAMPLE_LPI2C_MASTER, &g_m_handle, lpi2c_master_callback, NULL);

    /* deviceAddress = 0x01, data = g_master_txBuff - write to slave.
      start + slaveAddress(w) + deviceAddress + length of data buffer + data buffer + stop*/
    sMasterXfer.u8SlaveAddress    = I2C_MASTER_SLAVE_ADDR_7BIT;
    sMasterXfer.eDirection        = kLPI2C_Write;
    sMasterXfer.pu8Command        = &deviceAddress;
    sMasterXfer.u8CommandSize     = 1;
    sMasterXfer.pData             = g_master_txBuff;
    sMasterXfer.u16DataSize       = I2C_DATA_LENGTH;
    sMasterXfer.u8ControlFlagMask = kLPI2C_TransferStartStopFlag;

    /* Send master non-blocking data to slave */
    reVal = LPI2C_MasterTransferNonBlocking(&g_m_handle, &sMasterXfer);

    if (reVal != kStatus_Success)
    {
        return -1;
    }
    /*  Wait for transfer completed. */
    while (!g_MasterCompletionFlag)
    {
    }
    g_MasterCompletionFlag = false;

    PRINTF("Receive sent data from slave :");

    /* deviceAddress = 0x01, data = g_master_rxBuff - read from slave.
      start + slaveAddress(w) + deviceAddress + repeated start + slaveaddress(r) + rx data buffer + stop */
    sMasterXfer.u8SlaveAddress    = I2C_MASTER_SLAVE_ADDR_7BIT;
    sMasterXfer.eDirection        = kLPI2C_Read;
    sMasterXfer.pu8Command        = &deviceAddress;
    sMasterXfer.u8CommandSize     = 1;
    sMasterXfer.pData             = g_master_rxBuff;
    sMasterXfer.u16DataSize       = I2C_DATA_LENGTH - 1;
    sMasterXfer.u8ControlFlagMask = kLPI2C_TransferStartStopFlag;

    reVal = LPI2C_MasterTransferNonBlocking(&g_m_handle, &sMasterXfer);
    if (reVal != kStatus_Success)
    {
        return -1;
    }

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

    PRINTF("\r\nEnd of LPI2C example .\r\n");
    while (1)
    {
    }
}
