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

#define I2C_MASTER_SLAVE_ADDR_7BIT 0x7EU
#define I2C_DATA_LENGTH            34U
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

uint8_t g_slave_buff[I2C_DATA_LENGTH];
i2c_slave_handle_t g_s_handle;
volatile bool g_SlaveCompletionFlag = false;

/*******************************************************************************
 * Code

 ******************************************************************************/

static void i2c_slave_callback(I2C0_Type *base, volatile i2c_slave_transfer_t *xfer, void *param)
{
    switch (xfer->event)
    {
        /* Receive request */
        case kI2C_SlaveReceiveEvent:
            xfer->rxData = g_slave_buff;
            xfer->rxSize = I2C_DATA_LENGTH;
            break;

        /* The master has sent a stop transition on the bus */
        case kI2C_SlaveCompletionEvent:
            g_SlaveCompletionFlag = true;
            break;

        /* Transmit request */
        case kI2C_SlaveTransmitEvent:
            xfer->txData = &g_slave_buff[2];
            xfer->txSize = g_slave_buff[1];
            break;

        default:
            g_SlaveCompletionFlag = false;
            break;
    }
}

int main(void)
{
    i2c_slave_config_t slaveConfig;
    status_t reVal = kStatus_Fail;

    BOARD_InitHardware();

    /* Set up i2c slave first*/
    I2C_SlaveGetDefaultConfig(&slaveConfig);

    /* Change the slave address */
    slaveConfig.address0.address = I2C_MASTER_SLAVE_ADDR_7BIT;

    /* Initialize the I2C slave peripheral */
    I2C_SlaveInit(EXAMPLE_I2C_SLAVE, &slaveConfig);

    memset(g_slave_buff, 0, sizeof(g_slave_buff));

    /* Create the I2C handle for the non-blocking transfer */
    I2C_SlaveTransferCreateHandle(EXAMPLE_I2C_SLAVE, &g_s_handle, i2c_slave_callback, NULL);

    /* Start accepting I2C transfers on the I2C slave peripheral */
    reVal = I2C_SlaveTransferNonBlocking(EXAMPLE_I2C_SLAVE, &g_s_handle, kI2C_SlaveCompletionEvent);
    if (reVal != kStatus_Success)
    {
        return -1;
    }
    /*  Wait for transfer completed. */
    while (!g_SlaveCompletionFlag)
    {
    }
    g_SlaveCompletionFlag = false;

    /* Wait for master receive completed.*/
    while (!g_SlaveCompletionFlag)
    {
    }
    g_SlaveCompletionFlag = false;

    /* Wait some time for master complete transfer. */
    uint32_t count = 0x1000;
    while (count--)
    {
        __NOP();
    }

    BOARD_LedMatrixShowNumber(0, 0, false);

    while (1)
    {
    }
}
