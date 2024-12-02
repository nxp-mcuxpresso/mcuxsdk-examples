/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*  Standard C Included Files */
#include <stdio.h>
#include <string.h>
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_lpi2c.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define EXAMPLE_I2C_MASTER ((LPI2C_Type *)EXAMPLE_I2C_MASTER_BASE)
#define EXAMPLE_I2C_SLAVE  ((LPI2C_Type *)EXAMPLE_I2C_SLAVE_BASE)

#define I2C_MASTER_SLAVE_ADDR_7BIT (0x7EU)
#define I2C_BAUDRATE               (100000) /* 100K */

#define I2C_DATA_LENGTH (32) /* MAX is 256 */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

uint8_t g_slave_buff[I2C_DATA_LENGTH];
uint8_t g_master_buff[I2C_DATA_LENGTH];

lpi2c_master_handle_t g_m_handle;
lpi2c_slave_handle_t g_s_handle;

volatile bool g_slaveCompleted = false;

/*******************************************************************************
 * Code
 ******************************************************************************/

static void lpi2c_slave_callback(LPI2C_Type *base, lpi2c_slave_transfer_t *xfer, void *param)
{
    switch (xfer->event)
    {
        /* Setup the slave receive buffer */
        case kLPI2C_SlaveReceiveEvent:
            xfer->data     = g_slave_buff;
            xfer->dataSize = I2C_DATA_LENGTH;
            break;

        /* The master has sent a stop transition on the bus */
        case kLPI2C_SlaveCompletionEvent:
            g_slaveCompleted = true;
            break;

        /* Transmit event not handled in this demo */
        case kLPI2C_SlaveTransmitEvent:
        default:
            break;
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    uint32_t i = 0;
    lpi2c_slave_config_t slaveConfig;
    lpi2c_master_transfer_t masterXfer = {0};
    status_t reVal                     = kStatus_Fail;

    BOARD_InitHardware();

    PRINTF("\r\nLPI2C example -- MasterInterrupt_SlaveInterrupt.\r\n");

/*  Set i2c slave interrupt priority higher. */
#if (EXAMPLE_I2C_SLAVE_BASE == LPI2C0_BASE)
    NVIC_SetPriority(LPI2C0_IRQn, 0);
#elif (EXAMPLE_I2C_SLAVE_BASE == LPI2C1_BASE)
    NVIC_SetPriority(LPI2C1_IRQn, 0);
#elif (EXAMPLE_I2C_SLAVE_BASE == LPI2C2_BASE)
#if defined(FSL_FEATURE_INTMUX_IRQ_START_INDEX)
    /* Don't try to set LPI2C2 IRQ priority if it is an INTMUX IRQ */
    if (LPI2C2_IRQn < FSL_FEATURE_INTMUX_IRQ_START_INDEX)
#endif /* FSL_FEATURE_INTMUX_IRQ_START_INDEX */
    {
        NVIC_SetPriority(LPI2C2_IRQn, 0);
    }
#endif

#if (EXAMPLE_I2C_MASTER_BASE == LPI2C0_BASE)
    NVIC_SetPriority(LPI2C0_IRQn, 1);
#elif (EXAMPLE_I2C_MASTER_BASE == LPI2C1_BASE)
    NVIC_SetPriority(LPI2C1_IRQn, 1);
#elif (EXAMPLE_I2C_MASTER_BASE == LPI2C2_BASE)
#if defined(FSL_FEATURE_INTMUX_IRQ_START_INDEX)
    /* Don't try to set LPI2C2 IRQ priority if it is an INTMUX IRQ */
    if (LPI2C2_IRQn < FSL_FEATURE_INTMUX_IRQ_START_INDEX)
    {
#endif // FSL_FEATURE_INTMUX_IRQ_START_INDEX
        NVIC_SetPriority(LPI2C2_IRQn, 1);
    }
#endif

    /* Set up i2c slave first*/
    /*
     * slaveConfig.address0 = 0U;
     * slaveConfig.address1 = 0U;
     * slaveConfig.addressMatchMode = kLPI2C_MatchAddress0;
     * slaveConfig.filterDozeEnable = true;
     * slaveConfig.filterEnable = true;
     * slaveConfig.enableGeneralCall = false;
     * slaveConfig.ignoreAck = false;
     * slaveConfig.enableReceivedAddressRead = false;
     * slaveConfig.sdaGlitchFilterWidth_ns = 0;
     * slaveConfig.sclGlitchFilterWidth_ns = 0;
     * slaveConfig.dataValidDelay_ns = 0;
     * slaveConfig.clockHoldTime_ns = 0;
     */
    LPI2C_SlaveGetDefaultConfig(&slaveConfig);

    /* Change the slave address */
    slaveConfig.address0 = I2C_MASTER_SLAVE_ADDR_7BIT;

    /* Initialize the LPI2C slave peripheral */
    LPI2C_SlaveInit(EXAMPLE_I2C_SLAVE, &slaveConfig, LPI2C_SLAVE_CLOCK_FREQUENCY);

    memset(g_slave_buff, 0, sizeof(g_slave_buff));

    /* Create the LPI2C handle for the non-blocking transfer */
    LPI2C_SlaveTransferCreateHandle(EXAMPLE_I2C_SLAVE, &g_s_handle, lpi2c_slave_callback, NULL);

    /* Start accepting I2C transfers on the LPI2C slave peripheral */
    reVal = LPI2C_SlaveTransferNonBlocking(EXAMPLE_I2C_SLAVE, &g_s_handle,
                                           kLPI2C_SlaveReceiveEvent | kLPI2C_SlaveCompletionEvent);
    if (reVal != kStatus_Success)
    {
        return -1;
    }
    /* Set up i2c master to send data to slave*/
    for (i = 0; i < I2C_DATA_LENGTH; i++)
    {
        g_master_buff[i] = i;
    }

    /* Display the data the master will send */
    PRINTF("Master will send data :");
    for (i = 0; i < I2C_DATA_LENGTH; i++)
    {
        if (i % 8 == 0)
        {
            PRINTF("\r\n");
        }
        PRINTF("0x%2x  ", g_master_buff[i]);
    }

    PRINTF("\r\n\r\n");

    lpi2c_master_config_t masterConfig;

    /*
     * masterConfig.debugEnable = false;
     * masterConfig.ignoreAck = false;
     * masterConfig.pinConfig = kLPI2C_2PinOpenDrain;
     * masterConfig.baudRate_Hz = 100000U;
     * masterConfig.busIdleTimeout_ns = 0;
     * masterConfig.pinLowTimeout_ns = 0;
     * masterConfig.sdaGlitchFilterWidth_ns = 0;
     * masterConfig.sclGlitchFilterWidth_ns = 0;
     */
    LPI2C_MasterGetDefaultConfig(&masterConfig);

    /* Change the default baudrate configuration */
    masterConfig.baudRate_Hz = I2C_BAUDRATE;

    /* Initialize the LPI2C master peripheral */
    LPI2C_MasterInit(EXAMPLE_I2C_MASTER, &masterConfig, LPI2C_MASTER_CLOCK_FREQUENCY);

    /* Create the LPI2C handle for the non-blocking transfer */
    LPI2C_MasterTransferCreateHandle(EXAMPLE_I2C_MASTER, &g_m_handle, NULL, NULL);

    /* Setup the master transfer */
    masterXfer.slaveAddress   = I2C_MASTER_SLAVE_ADDR_7BIT;
    masterXfer.direction      = kLPI2C_Write;
    masterXfer.subaddress     = 0;
    masterXfer.subaddressSize = 0;
    masterXfer.data           = g_master_buff;
    masterXfer.dataSize       = I2C_DATA_LENGTH;
    masterXfer.flags          = kLPI2C_TransferDefaultFlag;

    /* Send master non-blocking data to slave */
    reVal = LPI2C_MasterTransferNonBlocking(EXAMPLE_I2C_MASTER, &g_m_handle, &masterXfer);
    if (reVal != kStatus_Success)
    {
        return -1;
    }
    /* Wait for the transfer to complete. */
    while (!g_slaveCompleted)
    {
    }

    /* Transfer completed. Check the data. */
    for (i = 0; i < I2C_DATA_LENGTH; i++)
    {
        if (g_slave_buff[i] != g_master_buff[i])
        {
            PRINTF("\r\nError occurred in this transfer ! \r\n");
            break;
        }
    }

    /* Check if we matched on all of the bytes */
    if (i == I2C_DATA_LENGTH)
    {
        PRINTF("\r\n Transfer successful!\r\n ");
    }

    /* Display the slave data */
    PRINTF("\r\nSlave received data :");
    for (i = 0; i < I2C_DATA_LENGTH; i++)
    {
        if (i % 8 == 0)
        {
            PRINTF("\r\n");
        }
        PRINTF("0x%2x  ", g_slave_buff[i]);
    }
    PRINTF("\r\n\r\n");

    /* Hang at the end */
    while (1)
    {
    }
}
