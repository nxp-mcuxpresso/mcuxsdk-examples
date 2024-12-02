/*
 * Copyright 2020-2023 NXP
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

#define LPI2C_MASTER_SLAVE_ADDR_7BIT 0x7EU
#define LPI2C_BAUDRATE               100000U
#define LPI2C_DATA_LENGTH            33U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

uint8_t g_master_txBuff[LPI2C_DATA_LENGTH];
uint8_t g_master_rxBuff[LPI2C_DATA_LENGTH];

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Main function
 */
int main(void)
{
    lpi2c_master_config_t sMasterConfig;
    status_t reVal          = kStatus_Fail;
    uint8_t u8DeviceAddress = 0x01U;
    uint16_t u16TxCount     = 0xFFU;

    BOARD_InitHardware();

    PRINTF("\r\nLPI2C board2board polling example -- Master transfer.\r\n");

    /* Set up i2c master to send data to slave*/
    /* First data in txBuff is data length of the transmiting data. */
    g_master_txBuff[0] = LPI2C_DATA_LENGTH - 1U;
    for (uint32_t i = 1U; i < LPI2C_DATA_LENGTH; i++)
    {
        g_master_txBuff[i] = i - 1;
    }

    PRINTF("Master will send data :");
    for (uint32_t i = 0U; i < LPI2C_DATA_LENGTH - 1U; i++)
    {
        if (i % 8 == 0)
        {
            PRINTF("\r\n");
        }
        PRINTF("0x%2x  ", g_master_txBuff[i + 1]);
    }
    PRINTF("\r\n\r\n");

    /* Set up I2C master */
    LPI2C_MasterGetDefaultConfig(&sMasterConfig, LPI2C_MASTER_CLOCK_FREQUENCY);

    /* Change the default baudrate configuration */
    sMasterConfig.u32BaudRateBps = LPI2C_BAUDRATE;

    /* Initialize the LPI2C master peripheral */
    LPI2C_MasterInit(EXAMPLE_LPI2C_MASTER, &sMasterConfig);

    /* Send master blocking data to slave */
    if (kStatus_Success == LPI2C_MasterStart(EXAMPLE_LPI2C_MASTER, LPI2C_MASTER_SLAVE_ADDR_7BIT, kLPI2C_Write))
    {
        /* Check master tx FIFO empty or not */
        LPI2C_MasterGetFifoCounts(EXAMPLE_LPI2C_MASTER, NULL, &u16TxCount);
        while (u16TxCount)
        {
            LPI2C_MasterGetFifoCounts(EXAMPLE_LPI2C_MASTER, NULL, &u16TxCount);
        }
        /* Check communicate with slave successful or not */
        if (LPI2C_MasterGetStatusFlags(EXAMPLE_LPI2C_MASTER) & kLPI2C_MasterNackDetectInterruptFlag)
        {
            return kStatus_LPI2C_Nak;
        }

        /* subAddress = 0x01, data = g_master_txBuff - write to slave.
          start + slaveaddress(w) + subAddress + length of data buffer + data buffer + stop*/
        reVal = LPI2C_MasterSend(EXAMPLE_LPI2C_MASTER, &u8DeviceAddress, 1, false);
        if (reVal != kStatus_Success)
        {
            return -1;
        }

        reVal = LPI2C_MasterSend(EXAMPLE_LPI2C_MASTER, g_master_txBuff, LPI2C_DATA_LENGTH, false);
        if (reVal != kStatus_Success)
        {
            return -1;
        }

        reVal = LPI2C_MasterStop(EXAMPLE_LPI2C_MASTER);
        if (reVal != kStatus_Success)
        {
            return -1;
        }
    }

    /* Wait until the slave is ready for transmit, wait time depend on user's case.
       Slave devices that need some time to process received byte or are not ready yet to
       send the next byte, can pull the clock low to signal to the master that it should wait.*/
    SDK_DelayAtLeastMs(20, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    PRINTF("Receive sent data from slave :");

    /* Receive blocking data from slave */
    /* subAddress = 0x01, data = g_master_rxBuff - read from slave.
      start + slaveaddress(w) + subAddress + repeated start + slaveaddress(r) + rx data buffer + stop */
    if (kStatus_Success == LPI2C_MasterStart(EXAMPLE_LPI2C_MASTER, LPI2C_MASTER_SLAVE_ADDR_7BIT, kLPI2C_Write))
    {
        /* Check master tx FIFO empty or not */
        LPI2C_MasterGetFifoCounts(EXAMPLE_LPI2C_MASTER, NULL, &u16TxCount);
        while (u16TxCount)
        {
            LPI2C_MasterGetFifoCounts(EXAMPLE_LPI2C_MASTER, NULL, &u16TxCount);
        }
        /* Check communicate with slave successful or not */
        while (LPI2C_MasterGetStatusFlags(EXAMPLE_LPI2C_MASTER) & kLPI2C_MasterNackDetectInterruptFlag)
        {
        }

        reVal = LPI2C_MasterSend(EXAMPLE_LPI2C_MASTER, &u8DeviceAddress, 1, false);
        if (reVal != kStatus_Success)
        {
            return -1;
        }

        reVal = LPI2C_MasterRepeatedStart(EXAMPLE_LPI2C_MASTER, LPI2C_MASTER_SLAVE_ADDR_7BIT, kLPI2C_Read);
        if (reVal != kStatus_Success)
        {
            return -1;
        }

        reVal = LPI2C_MasterReceive(EXAMPLE_LPI2C_MASTER, g_master_rxBuff, LPI2C_DATA_LENGTH - 1, false);
        if (reVal != kStatus_Success)
        {
            return -1;
        }

        reVal = LPI2C_MasterStop(EXAMPLE_LPI2C_MASTER);
        if (reVal != kStatus_Success)
        {
            return -1;
        }
    }

    for (uint32_t i = 0U; i < LPI2C_DATA_LENGTH - 1; i++)
    {
        if (i % 8 == 0)
        {
            PRINTF("\r\n");
        }
        PRINTF("0x%2x  ", g_master_rxBuff[i]);
    }
    PRINTF("\r\n\r\n");

    /* Transfer completed. Check the data.*/
    for (uint32_t i = 0U; i < LPI2C_DATA_LENGTH - 1U; i++)
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
