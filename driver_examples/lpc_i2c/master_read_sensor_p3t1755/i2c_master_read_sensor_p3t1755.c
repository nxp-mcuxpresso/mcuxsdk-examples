/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*  Standard C Included Files */
#include <string.h>
/*  SDK Included Files */
#include "fsl_debug_console.h"
#include "fsl_p3t1755.h"
#include "fsl_i2c.h"
#include "board.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define I2C_BAUDRATE     100000U
#define I2C_TIME_OUT_INDEX 100000000U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void i2c_master_callback(I2C_Type *base, i2c_master_handle_t *handle, status_t status, void *userData);
/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile status_t g_completionStatus;
volatile bool g_masterCompletionFlag;
i2c_master_handle_t g_i2c_m_handle;
p3t1755_handle_t p3t1755Handle;

/*******************************************************************************
 * Code
 ******************************************************************************/
static void i2c_master_callback(I2C_Type *base, i2c_master_handle_t *handle, status_t status, void *userData)
{
    if (status == kStatus_Success)
    {
        g_masterCompletionFlag = true;
    }

    g_completionStatus = status;
}

status_t i2c_WriteSensor(uint8_t deviceAddress, uint32_t regAddress, uint8_t *regData, size_t dataSize)
{
    status_t result                  = kStatus_Success;
    i2c_master_transfer_t masterXfer = {0};
    uint32_t timeout                 = 0U;

    masterXfer.slaveAddress   = deviceAddress;
    masterXfer.direction      = kI2C_Write;
    masterXfer.subaddress     = regAddress;
    masterXfer.subaddressSize = 1;
    masterXfer.data           = regData;
    masterXfer.dataSize       = dataSize;
    masterXfer.flags          = kI2C_TransferDefaultFlag;

    g_masterCompletionFlag = false;
    g_completionStatus     = kStatus_Success;
    result                 = I2C_MasterTransferNonBlocking(BOARD_TEMP_SENSOR_I2C_BASEADDR, &g_i2c_m_handle, &masterXfer);
    if (kStatus_Success != result)
    {
        return result;
    }

    while (!g_masterCompletionFlag)
    {
        timeout++;
        if ((g_completionStatus != kStatus_Success) || (timeout > I2C_TIME_OUT_INDEX))
        {
            break;
        }
    }

    if (timeout == I2C_TIME_OUT_INDEX)
    {
        result = kStatus_Timeout;
    }
    result = g_completionStatus;

    return result;
}

status_t i2c_ReadSensor(uint8_t deviceAddress, uint32_t regAddress, uint8_t *regData, size_t dataSize)
{
    status_t result                  = kStatus_Success;
    i2c_master_transfer_t masterXfer = {0};
    uint32_t timeout                 = 0U;

    masterXfer.slaveAddress   = deviceAddress;
    masterXfer.direction      = kI2C_Read;
    masterXfer.subaddress     = regAddress;
    masterXfer.subaddressSize = 1;
    masterXfer.data           = regData;
    masterXfer.dataSize       = dataSize;
    masterXfer.flags          = kI2C_TransferDefaultFlag;

    g_masterCompletionFlag = false;
    g_completionStatus     = kStatus_Success;
    result                 = I2C_MasterTransferNonBlocking(BOARD_TEMP_SENSOR_I2C_BASEADDR, &g_i2c_m_handle, &masterXfer);
    if (kStatus_Success != result)
    {
        return result;
    }

    while (!g_masterCompletionFlag)
    {
        timeout++;
        if ((g_completionStatus != kStatus_Success) || (timeout > I2C_TIME_OUT_INDEX))
        {
            break;
        }
    }

    if (timeout == I2C_TIME_OUT_INDEX)
    {
        result = kStatus_Timeout;
    }
    result = g_completionStatus;

    return result;
}

/*!
 * @brief Main function
 */
int main(void)
{
    status_t result = kStatus_Success;
    i2c_master_config_t masterConfig;
    p3t1755_config_t p3t1755Config;
    double temperature;

    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

    PRINTF("\r\ni2c master read sensor data example.\r\n");

    /*
     * masterConfig.debugEnable = false;
     * masterConfig.ignoreAck = false;
     * masterConfig.pinConfig = kI2C_2PinOpenDrain;
     * masterConfig.baudRate_Hz = 100000U;
     * masterConfig.busIdleTimeout_ns = 0;
     * masterConfig.pinLowTimeout_ns = 0;
     * masterConfig.sdaGlitchFilterWidth_ns = 0;
     * masterConfig.sclGlitchFilterWidth_ns = 0;
     */
    I2C_MasterGetDefaultConfig(&masterConfig);

    masterConfig.baudRate_Bps = I2C_BAUDRATE;

    I2C_MasterInit(BOARD_TEMP_SENSOR_I2C_BASEADDR, &masterConfig, I2C_CLOCK_FREQUENCY);
    I2C_MasterTransferCreateHandle(BOARD_TEMP_SENSOR_I2C_BASEADDR, &g_i2c_m_handle, i2c_master_callback, NULL);

    p3t1755Config.writeTransfer = i2c_WriteSensor;
    p3t1755Config.readTransfer  = i2c_ReadSensor;
    p3t1755Config.sensorAddress = SENSOR_SLAVE_ADDR;
    P3T1755_Init(&p3t1755Handle, &p3t1755Config);

    while (1)
    {
        result = P3T1755_ReadTemperature(&p3t1755Handle, &temperature);
        if (result != kStatus_Success)
        {
            PRINTF("\r\nP3T1755 read temperature failed.\r\n");
        }
        else
        {
            PRINTF("\r\nTemperature:%f \r\n", temperature);
        }
        SDK_DelayAtLeastUs(1000000, CLOCK_GetCoreSysClkFreq());
    }
}
