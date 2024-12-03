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
#define I2C_DATA_LENGTH            34U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

uint8_t g_slave_buff[I2C_DATA_LENGTH];
lpi2c_slave_transfer_handle_t g_s_handle;
volatile bool g_SlaveCompletionFlag = false;

/*******************************************************************************
 * Code
 ******************************************************************************/

static void lpi2c_slave_callback(lpi2c_slave_transfer_handle_t *psHandle)
{
    switch (LPI2C_GET_SLAVE_TRANSFER_EVENT(psHandle))
    {
        /*  Address match event */
        case kLPI2C_SlaveAddressMatchEvent:
            LPI2C_GET_SLAVE_TRANSFER_DATA_POINTER(psHandle) = NULL;
            LPI2C_GET_SLAVE_TRANSFER_DATASIZE(psHandle)     = 0;
            break;
        /*  Transmit request */
        case kLPI2C_SlaveTransmitEvent:
            /*  Update information for transmit process */
            LPI2C_GET_SLAVE_TRANSFER_DATA_POINTER(psHandle) = &g_slave_buff[2];
            LPI2C_GET_SLAVE_TRANSFER_DATASIZE(psHandle)     = g_slave_buff[1];
            break;

        /*  Receive request */
        case kLPI2C_SlaveReceiveEvent:
            /*  Update information for received process */
            LPI2C_GET_SLAVE_TRANSFER_DATA_POINTER(psHandle) = g_slave_buff;
            LPI2C_GET_SLAVE_TRANSFER_DATASIZE(psHandle)     = I2C_DATA_LENGTH;
            break;

        /*  Transfer done */
        case kLPI2C_SlaveCompletionEvent:
            g_SlaveCompletionFlag                           = true;
            LPI2C_GET_SLAVE_TRANSFER_DATA_POINTER(psHandle) = NULL;
            LPI2C_GET_SLAVE_TRANSFER_DATASIZE(psHandle)     = 0;
            break;

        default:
            g_SlaveCompletionFlag = false;
            break;
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    lpi2c_slave_config_t sSlaveConfig;
    status_t reVal = kStatus_Fail;

    BOARD_InitHardware();

    PRINTF("\r\nLPI2C board2board interrupt example -- Slave transfer.\r\n\r\n");

    /* Set up i2c slave first */
    LPI2C_SlaveGetDefaultConfig(&sSlaveConfig, I2C_MASTER_SLAVE_ADDR_7BIT, LPI2C_SLAVE_CLOCK_FREQUENCY);

    /* Initialize the LPI2C slave peripheral */
    LPI2C_SlaveInit(EXAMPLE_LPI2C_SLAVE, &sSlaveConfig);

    memset(g_slave_buff, 0, sizeof(g_slave_buff));

    /* Create the LPI2C handle for the non-blocking transfer */
    LPI2C_SlaveTransferCreateHandle(EXAMPLE_LPI2C_SLAVE, &g_s_handle, lpi2c_slave_callback, NULL);

    /* Start accepting I2C transfers on the LPI2C slave peripheral */
    reVal = LPI2C_SlaveTransferNonBlocking(&g_s_handle, kLPI2C_SlaveCompletionEvent | kLPI2C_SlaveAddressMatchEvent);
    if (reVal != kStatus_Success)
    {
        return -1;
    }

    /*  wait for transfer completed. */
    while (!g_SlaveCompletionFlag)
    {
    }
    g_SlaveCompletionFlag = false;

    PRINTF("Slave received data :");
    for (uint32_t i = 0U; i < g_slave_buff[1]; i++)
    {
        if (i % 8 == 0)
        {
            PRINTF("\r\n");
        }
        PRINTF("0x%2x  ", g_slave_buff[2 + i]);
    }
    PRINTF("\r\n\r\n");

    /* Wait for master receive completed.*/
    while (!g_SlaveCompletionFlag)
    {
    }
    g_SlaveCompletionFlag = false;

    PRINTF("\r\nEnd of LPI2C example .\r\n");

    while (1)
    {
    }
}
