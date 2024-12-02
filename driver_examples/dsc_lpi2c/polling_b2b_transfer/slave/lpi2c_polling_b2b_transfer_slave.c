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

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Main function
 */
int main(void)
{
    lpi2c_slave_config_t sSlaveConfig;
    status_t reVal = kStatus_Fail;
    uint8_t u8Subaddress;

    BOARD_InitHardware();

    PRINTF("\r\nLPI2C board2board polling example -- Slave transfer.\r\n\r\n");

    /* Set up i2c slave first */
    LPI2C_SlaveGetDefaultConfig(&sSlaveConfig, I2C_MASTER_SLAVE_ADDR_7BIT, LPI2C_SLAVE_CLOCK_FREQUENCY);

    /* Initialize the LPI2C slave peripheral */
    LPI2C_SlaveInit(EXAMPLE_LPI2C_SLAVE, &sSlaveConfig);

    memset(g_slave_buff, 0, sizeof(g_slave_buff));

    /* Wait until being visited by master before slave polling transfer */
    while (!(LPI2C_SlaveGetStatusFlags(EXAMPLE_LPI2C_SLAVE) & kLPI2C_SlaveAddressValidInterruptFlag))
    {
    }
    /* Clear being visited flag */
    LPI2C_SlaveGetReceivedAddress(EXAMPLE_LPI2C_SLAVE);

    /* Start accepting I2C transfers on the LPI2C slave peripheral */
    reVal = LPI2C_SlaveReceive(EXAMPLE_LPI2C_SLAVE, g_slave_buff, I2C_DATA_LENGTH, 0);

    if (reVal != kStatus_Success)
    {
        return -1;
    }

    /* Wait until being visited by master before slave polling transfer */
    while (!(LPI2C_SlaveGetStatusFlags(EXAMPLE_LPI2C_SLAVE) & kLPI2C_SlaveAddressValidInterruptFlag))
    {
    }
    /* Clear being visited flag */
    LPI2C_SlaveGetReceivedAddress(EXAMPLE_LPI2C_SLAVE);

    /* Start accepting I2C transfers on the LPI2C slave peripheral to simulate subaddress and will send ACK to master */
    reVal = LPI2C_SlaveReceive(EXAMPLE_LPI2C_SLAVE, &u8Subaddress, 1, 0);

    if (reVal != kStatus_Success)
    {
        return -1;
    }

    /* Wait until being visited by master before slave polling transfer */
    while (!(LPI2C_SlaveGetStatusFlags(EXAMPLE_LPI2C_SLAVE) & kLPI2C_SlaveAddressValidInterruptFlag))
    {
    }
    /* Clear being visited flag */
    LPI2C_SlaveGetReceivedAddress(EXAMPLE_LPI2C_SLAVE);

    reVal = LPI2C_SlaveSend(EXAMPLE_LPI2C_SLAVE, &g_slave_buff[2], g_slave_buff[1], 0);

    if (reVal != kStatus_Success)
    {
        return -1;
    }

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

    PRINTF("\r\nEnd of LPI2C example .\r\n");

    while (1)
    {
    }
}
