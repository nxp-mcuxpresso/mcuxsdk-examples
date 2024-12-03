/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _AT24C02_H_
#define _AT24C02_H_

#include "fsl_common.h"
#include "fsl_lpi2c.h"

/*!
 * @addtogroup AT24C02
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define AT24C02_I2C_BASE     LPI2C0
#define AT24C02_I2C_ADDR     0x50
#define AT24C02_I2C_BAUDRATE 100000U

/*! @brief WM8960 return status. */
typedef enum _at24c02_status
{
    kStatus_AT24C02_Success = 0x0,
    kStatus_AT24C02_I2CFail = 0x1,
    kStatus_AT24C02_Fail    = 0x2
} at24c02_status_t;

/*! @brief AT24C02 configure definition. */
typedef struct at24c02_handler
{
    /* I2C relevant definition. */
    LPI2C_Type *i2cBase;    /*!< I2C base pointer. */
    uint8_t device_addr;    /*!< I2C device address. */
    uint32_t baudRate_kbps; /*!< I2C baud rate. */
} at24c02_handler_t;

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Initialize the I2C module in AT24C02.
 *
 * EEPROM AT24C02 uses i2c to write/read the registers in it.
 * @param handler AT24C02 handler structure.
 */
at24c02_status_t AT24C02_I2CInit(at24c02_handler_t *handler);

/*!
 * @brief Byte Write to AT24C02.
 *
 * @param handler AT24C02 handler structure.
 * @param address data word address
 * @param data    write data
 */
at24c02_status_t AT24C02_ByteWrite(at24c02_handler_t *handler, uint8_t address, uint8_t data);

/*!
 * @brief ACK polling for AT24C02.
 *
 * @param handler AT24C02 handler structure.
 */
at24c02_status_t AT24C02_ACKPoll(at24c02_handler_t *handler);

/*!
 * @brief Byte Read from AT24C02.
 *
 * @param handler AT24C02 handler structure.
 * @param address data word address
 * @param data    write data
 */
at24c02_status_t AT24C02_ByteRead(at24c02_handler_t *handler, uint8_t address, uint8_t *data);

#if defined(__cplusplus)
}
#endif

/*! @} */

#endif /* _AT24C02_H_ */
