/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_lpi2c.h"
#include "camera_config.h"
#include "fsl_debug_console.h"



lpi2c_master_handle_t g_m_handle;
volatile bool g_MasterCompletionFlag = false;
volatile bool g_MasterNackFlag       = false;


void lpi2c_camera_callback(LPI2C_Type *base, lpi2c_master_handle_t *handle, status_t status, void *userData)
{
    if (status == kStatus_LPI2C_Nak)
    {
        g_MasterNackFlag = true;
    }
    else
    {
        g_MasterCompletionFlag = true;
        /* Display failure information when status is not success. */
        if (status != kStatus_Success)
        {
            PRINTF("Error occured during transfer!");
        }
    }
}

void BOARD_Camera_I2C_Init(LPI2C_Type *base)
{
    /* i2c master init */
	lpi2c_master_config_t masterConfig;

    CLOCK_AttachClk(kOSC_CLK_to_FCCLK0);
    CLOCK_AttachClk(kFCCLK0_to_FLEXCOMM8);

    /* Create the LPI2C handle for the non-blocking transfer */
    LPI2C_MasterTransferCreateHandle(SCCB_BASE, &g_m_handle, lpi2c_camera_callback, NULL);
	
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
    LPI2C_MasterInit(SCCB_BASE, &masterConfig, SCCB_CLOCK_FREQ);
	
}

status_t BOARD_I2C_Tx(LPI2C_Type *base,
                        uint8_t deviceAddress,
                        uint32_t subAddress,
                        uint8_t subaddressSize,
                        uint8_t *txBuff,
                        uint8_t txBuffSize)
{
    status_t reVal = kStatus_Success;
    lpi2c_master_transfer_t masterXfer;
	memset(&masterXfer, 0, sizeof(masterXfer));

    /* Prepare transfer structure. */
    masterXfer.slaveAddress   = deviceAddress;
    masterXfer.direction      = kLPI2C_Write;
    masterXfer.subaddress     = subAddress;
    masterXfer.subaddressSize = subaddressSize;
    masterXfer.data           = txBuff;
    masterXfer.dataSize       = txBuffSize;
    masterXfer.flags          = kLPI2C_TransferDefaultFlag;

    reVal = LPI2C_MasterTransferNonBlocking(base, &g_m_handle, &masterXfer);
    if (reVal != kStatus_Success)
    {
        return -1;
    }
	
    /*  wait for transfer completed. */
    while ((!g_MasterNackFlag) && (!g_MasterCompletionFlag))
    {
    }

    g_MasterNackFlag = false;

    if (g_MasterCompletionFlag == true)
    {
        g_MasterCompletionFlag = false;
        return kStatus_Success;
    }
    else
    {
        return kStatus_Fail;
    }
}

status_t BOARD_I2C_Rx(LPI2C_Type *base,
                           uint8_t deviceAddress,
                           uint32_t subAddress,
                           uint8_t subaddressSize,
                           uint8_t *rxBuff,
                           uint8_t rxBuffSize)
{
    status_t reVal = kStatus_Success;
    lpi2c_master_transfer_t masterXfer;
	memset(&masterXfer, 0, sizeof(masterXfer));
    /* Prepare transfer structure. */
    masterXfer.slaveAddress   = deviceAddress;
    masterXfer.subaddress     = subAddress;
    masterXfer.subaddressSize = 1;
    masterXfer.data           = NULL;
    masterXfer.dataSize       = 0;
    masterXfer.direction      = kLPI2C_Write;
    masterXfer.flags          = kLPI2C_TransferDefaultFlag;

    reVal = LPI2C_MasterTransferNonBlocking(base, &g_m_handle, &masterXfer);
    if (reVal != kStatus_Success)
    {
        return -1;
    }
    /*  wait for transfer completed. */
    while ((!g_MasterNackFlag) && (!g_MasterCompletionFlag))
    {
    }

    g_MasterNackFlag = false;
    if (g_MasterCompletionFlag == true)
    {
        g_MasterCompletionFlag = false;
    }
    else
    {
        return kStatus_Fail;
    }

    /* Prepare transfer structure. */
    masterXfer.slaveAddress   = deviceAddress;
    masterXfer.subaddress     = 0x0;
    masterXfer.subaddressSize = 0;
    masterXfer.data           = rxBuff;
    masterXfer.dataSize       = rxBuffSize;
    masterXfer.direction      = kLPI2C_Read;
    masterXfer.flags          = kLPI2C_TransferDefaultFlag;

    reVal = LPI2C_MasterTransferNonBlocking(base, &g_m_handle, &masterXfer);
    if (reVal != kStatus_Success)
    {
        return -1;
    }
    /*  wait for transfer completed. */
    while ((!g_MasterNackFlag) && (!g_MasterCompletionFlag))
    {
    }

    g_MasterNackFlag = false;
    if (g_MasterCompletionFlag == true)
    {
        g_MasterCompletionFlag = false;
        return kStatus_Success;
    }
    else
    {
        return kStatus_Fail;
    }
}


status_t BOARD_Camera_I2C_Send(uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize)
{
    return BOARD_I2C_Tx(SCCB_BASE, deviceAddress, subAddress, subAddressSize, (uint8_t *)txBuff, txBuffSize);
}

status_t BOARD_Camera_I2C_Receive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize)
{
    return BOARD_I2C_Rx(SCCB_BASE, deviceAddress, subAddress, subAddressSize, rxBuff, rxBuffSize);
}