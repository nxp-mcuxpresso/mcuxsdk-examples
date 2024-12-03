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
#define I2C_DATA_LENGTH            34U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void i2c_slave_callback(i2c_slave_transfer_handle_t *psHandle);
/*******************************************************************************
 * Variables
 ******************************************************************************/

uint8_t g_slave_buff[I2C_DATA_LENGTH];
i2c_slave_transfer_handle_t g_s_handle;
volatile bool g_SlaveCompletionFlag = false;

/*******************************************************************************
 * Code
 ******************************************************************************/

static void i2c_slave_callback(i2c_slave_transfer_handle_t *psHandle)
{
    switch (I2C_GET_SLAVE_TRANSFER_EVENT(psHandle))
    {
        /*  Address match event */
        case kI2C_SlaveAddressMatchEvent:
            I2C_GET_SLAVE_TRANSFER_DATA_POINTER(psHandle) = NULL;
            I2C_GET_SLAVE_TRANSFER_DATASIZE(psHandle)     = 0U;
            break;
        /*  Transmit request */
        case kI2C_SlaveOutofTransmitDataEvent:
            /*  Update information for transmit process */
            I2C_GET_SLAVE_TRANSFER_DATA_POINTER(psHandle) = &g_slave_buff[2];
            I2C_GET_SLAVE_TRANSFER_DATASIZE(psHandle)     = g_slave_buff[1];
            break;

        /*  Receive request */
        case kI2C_SlaveOutofReceiveSpaceEvent:
            /*  Update information for received process */
            I2C_GET_SLAVE_TRANSFER_DATA_POINTER(psHandle) = g_slave_buff;
            I2C_GET_SLAVE_TRANSFER_DATASIZE(psHandle)     = I2C_DATA_LENGTH;
            break;

        /*  Transfer done */
        case kI2C_SlaveCompletionEvent:
            g_SlaveCompletionFlag                         = true;
            I2C_GET_SLAVE_TRANSFER_DATA_POINTER(psHandle) = NULL;
            I2C_GET_SLAVE_TRANSFER_DATASIZE(psHandle)     = 0U;
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
    i2c_slave_config_t slaveConfig;

    BOARD_InitHardware();

    PRINTF("\r\nI2C board2board EDMA example -- Slave transfer.\r\n");
    PRINTF("\r\nSlave uses interrupt way.\r\n\r\n");

    /*1.Set up i2c slave first*/
    /*
     * slaveConfig->addressingMode = kI2C_Address7bit;
     * slaveConfig->enableGeneralCall = false;
     * slaveConfig->enableWakeUp = false;
     * slaveConfig->enableBaudRateCtl = false;
     * slaveConfig->enableSlave = true;
     */
    I2C_SlaveGetDefaultConfig(&slaveConfig, I2C_MASTER_SLAVE_ADDR_7BIT, I2C_SLAVE_CLK_FREQ);
    I2C_SlaveInit(EXAMPLE_I2C_SLAVE_BASEADDR, &slaveConfig);

    for (uint32_t i = 0U; i < I2C_DATA_LENGTH; i++)
    {
        g_slave_buff[i] = 0;
    }

    memset(&g_s_handle, 0, sizeof(g_s_handle));

    I2C_SlaveTransferCreateHandle(EXAMPLE_I2C_SLAVE_BASEADDR, &g_s_handle, i2c_slave_callback, NULL);

    /* Set up slave transfer. */
    i2c_slave_transfer_t sTransfer;
    memset(&sTransfer, 0, sizeof(sTransfer));
    sTransfer.u8EventMask = kI2C_SlaveCompletionEvent | kI2C_SlaveAddressMatchEvent;
    /* Start slave non-blocking transfer. */
    I2C_SlaveTransferNonBlocking(&g_s_handle, &sTransfer);

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

    PRINTF("\r\nEnd of I2C example .\r\n");

    while (1)
    {
    }
}
