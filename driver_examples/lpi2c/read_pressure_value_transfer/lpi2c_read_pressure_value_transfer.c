/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*  SDK Included Files */
#include "fsl_lpi2c.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
lpi2c_master_handle_t g_m_handle;
bool completionFlag = false;
bool nakFlag        = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
static void lpi2c_master_callback(LPI2C_Type *base, lpi2c_master_handle_t *handle, status_t status, void *userData)
{
    /* Signal transfer success when received success status. */
    if (status == kStatus_Success)
    {
        completionFlag = true;
    }
    /* Signal transfer success when received success status. */
    if (status == kStatus_LPI2C_Nak)
    {
        nakFlag = true;
    }
}

static bool LPI2C_ReadPressureWhoAmI(void)
{
    /*
    How to read the device who_am_I value ?
    Start + Device_address_Write , who_am_I_register;
    Repeart_Start + Device_address_Read , who_am_I_value.
    */
    lpi2c_master_transfer_t masterXfer;
    bool result            = false;
    status_t reVal         = kStatus_Fail;
    uint8_t who_am_i_value = 0x00;

    /* Clean LPI2C transfer structure. */
    memset(&masterXfer, 0, sizeof(masterXfer));
    masterXfer.slaveAddress   = 0x60;
    masterXfer.direction      = kLPI2C_Read;
    masterXfer.subaddress     = 0x0C;
    masterXfer.subaddressSize = 1;
    masterXfer.data           = &who_am_i_value;
    masterXfer.dataSize       = 1;
    masterXfer.flags          = kLPI2C_TransferDefaultFlag;

    while (1)
    {
        reVal = LPI2C_MasterTransferNonBlocking(BOARD_PRESSURE_I2C_BASEADDR, &g_m_handle, &masterXfer);
        if (reVal != kStatus_Success)
        {
            continue;
        }

        /*  wait for transfer completed. */
        while ((!nakFlag) && (!completionFlag))
        {
        }

        if (completionFlag == true)
        {
            completionFlag = false;
            break;
        }
        else
        {
            nakFlag = false;
        }
    }

    if (reVal == kStatus_Success)
    {
        PRINTF("Found a MPL115A2 on board !\r\n");
        result = true;
    }
    else
    {
        PRINTF("\r\n Do not find an barometer device ! \r\n");
        result = false;
    }

    return result;
}

int main(void)
{
    lpi2c_master_config_t masterConfig;
    bool boardHasBarometer;

    /* Init board hardware. */
    BOARD_InitHardware();

    PRINTF("\r\nLPI2C example -- Read Barometer Value\r\n");

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
    LPI2C_MasterInit(BOARD_PRESSURE_I2C_BASEADDR, &masterConfig, BOARD_LPI2C_CLK_FREQ);
    LPI2C_MasterTransferCreateHandle(BOARD_PRESSURE_I2C_BASEADDR, &g_m_handle, lpi2c_master_callback, NULL);

    boardHasBarometer = LPI2C_ReadPressureWhoAmI();

    if (true == boardHasBarometer)
    {
        PRINTF("\r\nThere is a Barometer on board.\r\n");
    }
    else
    {
        PRINTF("\r\nThere is no Barometer on board.\r\n");
    }

    PRINTF("\r\nEnd of LPI2C example .\r\n");
    while (1)
    {
    }
}
