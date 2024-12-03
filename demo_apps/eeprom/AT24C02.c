/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "AT24C02.h"
#include "fsl_lpi2c.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* lpi2c handler */
lpi2c_master_handle_t g_m_handle;

/* The completion flag */
volatile bool completionFlag = false;

/* Completion flag */
static void lpi2c_master_callback(LPI2C_Type *base,
                                  lpi2c_master_handle_t *handle,
                                  status_t completionStatus,
                                  void *userData)
{
    switch (completionStatus)
    {
        /*  Transfer done */
        case kStatus_Success:
            completionFlag = true;
            break;

        default:
            break;
    }
}

/*******************************************************************************
 * Code
 ******************************************************************************/
extern uint32_t AT24C02_GetI2cSrcClkFreq(void);

at24c02_status_t AT24C02_I2CInit(at24c02_handler_t *handler)
{
    /* i2c master init */
    uint32_t sourceClock = 0;
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
    masterConfig.baudRate_Hz = AT24C02_I2C_BAUDRATE;
    sourceClock              = AT24C02_GetI2cSrcClkFreq();
    LPI2C_MasterInit(handler->i2cBase, &masterConfig, sourceClock);
    LPI2C_MasterTransferCreateHandle(handler->i2cBase, &g_m_handle, lpi2c_master_callback, NULL);
    return kStatus_AT24C02_Success;
}

at24c02_status_t AT24C02_ByteWrite(at24c02_handler_t *handler, uint8_t address, uint8_t data)
{
    /* Setup the master transfer */
    lpi2c_master_transfer_t masterXfer = {0};
    masterXfer.slaveAddress            = handler->device_addr;
    masterXfer.direction               = kLPI2C_Write;
    masterXfer.subaddress              = address;
    masterXfer.subaddressSize          = 1;
    masterXfer.data                    = &data;
    masterXfer.dataSize                = 1;
    masterXfer.flags                   = kLPI2C_TransferDefaultFlag;

    LPI2C_MasterTransferNonBlocking(handler->i2cBase, &g_m_handle, &masterXfer);

    while (!completionFlag)
    {
        ;
    }

    completionFlag = false;

    return kStatus_AT24C02_Success;
}

at24c02_status_t AT24C02_ACKPoll(at24c02_handler_t *handler)
{
    /* Used for the Twr time: 5ms */
    lpi2c_master_transfer_t masterXfer = {0};
    masterXfer.slaveAddress            = handler->device_addr;
    masterXfer.direction               = kLPI2C_Write;
    masterXfer.subaddress              = 0;
    masterXfer.subaddressSize          = 0;
    masterXfer.data                    = NULL;
    masterXfer.dataSize                = 0;
    masterXfer.flags                   = kLPI2C_TransferDefaultFlag;

    LPI2C_MasterTransferNonBlocking(handler->i2cBase, &g_m_handle, &masterXfer);

    while (!completionFlag)
    {
        ;
    }

    completionFlag = false;

    return kStatus_AT24C02_Success;
}

at24c02_status_t AT24C02_ByteRead(at24c02_handler_t *handler, uint8_t address, uint8_t *data)
{
    lpi2c_master_transfer_t masterXfer = {0};
    masterXfer.slaveAddress            = handler->device_addr;
    masterXfer.direction               = kLPI2C_Read;
    masterXfer.subaddress              = address;
    masterXfer.subaddressSize          = 1;
    masterXfer.data                    = data;
    masterXfer.dataSize                = 1;
    masterXfer.flags                   = kLPI2C_TransferDefaultFlag;

    LPI2C_MasterTransferNonBlocking(handler->i2cBase, &g_m_handle, &masterXfer);

    while (!completionFlag)
    {
        ;
    }

    completionFlag = false;

    return kStatus_AT24C02_Success;
}
