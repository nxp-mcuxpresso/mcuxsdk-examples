/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
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
#include "fsl_lpi2c_cmsis.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define I2C_MASTER_SLAVE_ADDR_7BIT (0x7EU)
#define I2C_DATA_LENGTH            (32) /* MAX is 256 */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

uint8_t g_slave_buff[I2C_DATA_LENGTH];
uint8_t g_master_buff[I2C_DATA_LENGTH];

volatile bool g_slaveCompleted  = false;
volatile bool g_masterCompleted = false;

/*******************************************************************************
 * Code
 ******************************************************************************/

static void lpi2c_slave_callback(uint32_t event)
{
    switch (event)
    {
        /* The master has sent a stop transition on the bus */
        case ARM_I2C_EVENT_TRANSFER_DONE:
            g_slaveCompleted = true;
            break;

        default:
            break;
    }
}
static void lpi2c_master_callback(uint32_t event)
{
    switch (event)
    {
        /* The master has sent a stop transition on the bus */
        case ARM_I2C_EVENT_TRANSFER_DONE:
            g_masterCompleted = true;
            break;

        /* master arbitration lost */
        case ARM_I2C_EVENT_ARBITRATION_LOST:
            g_masterCompleted = true;
            break;

        default:
            break;
    }
}

void test_interrupt_transfer(void)
{
    uint32_t i = 0;

    PRINTF("\r\ntest interrupt transfer\r\n");

    memset(g_slave_buff, 0, sizeof(g_slave_buff));
    /* Set up i2c master to send data to slave */
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
    /* Start accepting I2C transfers on the LPI2C slave peripheral */
    EXAMPLE_I2C_SLAVE.SlaveReceive(g_slave_buff, I2C_DATA_LENGTH);
    /* Start transmitting I2C transfers on the LPI2C master peripheral */
    EXAMPLE_I2C_MASTER.MasterTransmit(I2C_MASTER_SLAVE_ADDR_7BIT, g_master_buff, I2C_DATA_LENGTH, false);

    /* Wait for the transfer to complete. */
    while (!g_slaveCompleted)
    {
    }

    g_slaveCompleted = false;

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

    memset(g_master_buff, 0, sizeof(g_master_buff));
    /* Set up i2c slave to send data to master*/
    for (i = 0; i < I2C_DATA_LENGTH; i++)
    {
        g_slave_buff[i] = i;
    }

    /* Display the data the slave will send */
    PRINTF("slave will send data :");
    for (i = 0; i < I2C_DATA_LENGTH; i++)
    {
        if (i % 8 == 0)
        {
            PRINTF("\r\n");
        }
        PRINTF("0x%2x  ", g_slave_buff[i]);
    }

    PRINTF("\r\n\r\n");

    /* Start transmitting I2C transfers on the LPI2C slave peripheral */
    EXAMPLE_I2C_SLAVE.SlaveTransmit(g_slave_buff, I2C_DATA_LENGTH);
    /* Start accepting I2C transfers on the LPI2C master peripheral */
    EXAMPLE_I2C_MASTER.MasterReceive(I2C_MASTER_SLAVE_ADDR_7BIT, g_master_buff, I2C_DATA_LENGTH, false);

    /* Wait for the transfer to complete. */
    while (!g_masterCompleted)
    {
    }

    g_masterCompleted = false;
    /* Display the master data */
    PRINTF("\r\nmaster received data :");
    for (i = 0; i < I2C_DATA_LENGTH; i++)
    {
        if (i % 8 == 0)
        {
            PRINTF("\r\n");
        }
        PRINTF("0x%2x  ", g_master_buff[i]);
    }
    PRINTF("\r\n\r\n");
}

/*!
 * @brief Main function
 */
int main(void)
{
    /*Init BOARD*/
    BOARD_InitHardware();

    /*set I2C master and I2C slave priority*/
    NVIC_SetPriority(I2C_SLAVE_IRQ, 0);
    NVIC_SetPriority(I2C_MASTER_IRQ, 1);

    /* Initialize the LPI2C slave peripheral */
    EXAMPLE_I2C_SLAVE.Initialize(lpi2c_slave_callback);
    EXAMPLE_I2C_SLAVE.PowerControl(ARM_POWER_FULL);

    /* Change the slave address */
    EXAMPLE_I2C_SLAVE.Control(ARM_I2C_OWN_ADDRESS, I2C_MASTER_SLAVE_ADDR_7BIT);

    /* Initialize the LPI2C master peripheral */
    EXAMPLE_I2C_MASTER.Initialize(lpi2c_master_callback);
    EXAMPLE_I2C_MASTER.PowerControl(ARM_POWER_FULL);

    /* Change the default baudrate configuration */
    EXAMPLE_I2C_MASTER.Control(ARM_I2C_BUS_SPEED, ARM_I2C_BUS_SPEED_STANDARD);

    /*test interrupt transfer*/
    test_interrupt_transfer();

    /* Hang at the end */
    while (1)
    {
    }
}
