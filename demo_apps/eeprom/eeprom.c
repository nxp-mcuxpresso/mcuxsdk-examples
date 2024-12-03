/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "AT24C02.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief delayForAT24C02DataFlashReady function
 *       The AT24C02 need a time to flash the data that has been written
 */
void delayForAT24C02DataFlashReady(void)
{
    volatile uint32_t i = 50000;
    while (i--)
    {
        ;
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    at24c02_handler_t handler;
    at24c02_status_t status;
    uint8_t write_data;
    uint8_t read_data;
    uint8_t eeprom_add;
    uint16_t cnt;

    /* Initializes board hardware. */
    BOARD_InitHardware();

    PRINTF("EEPROM Demo\r\n\r\n");
    PRINTF("This demo will write data into on board EEPROM and then read it back.\r\n\r\n");

    /* Configures the at24c02 handler */
    handler.i2cBase       = AT24C02_I2C_BASE;
    handler.device_addr   = AT24C02_I2C_ADDR;
    handler.baudRate_kbps = AT24C02_I2C_BAUDRATE;

    /* Initializes the i2c which to be used to operate the at24c02 */
    AT24C02_I2CInit(&handler);

    for (cnt = 0; cnt < 256; cnt++)
    {
        eeprom_add = cnt;
        write_data = 255 - cnt;

        status = AT24C02_ByteWrite(&handler, eeprom_add, write_data);
        if (status == kStatus_AT24C02_I2CFail)
        {
            PRINTF("Write failure\r\n");
        }

        /* Wait for the data flashed ready */
        delayForAT24C02DataFlashReady();

        AT24C02_ACKPoll(&handler);

        status = AT24C02_ByteRead(&handler, eeprom_add, &read_data);
        if (status == kStatus_AT24C02_I2CFail)
        {
            PRINTF("Read failure\r\n");
        }

        if (read_data != write_data)
        {
            PRINTF("Read data is 0x%x instead of 0x%x\r\n", read_data, write_data);
        }
        else
        {
            PRINTF("Write data at 0x%x is 0x%x, read back data is 0x%x\r\n", eeprom_add, write_data, read_data);
        }
    }

    PRINTF("\r\nEnd of the demo.\n\r");

    while (1)
    {
    }
}
