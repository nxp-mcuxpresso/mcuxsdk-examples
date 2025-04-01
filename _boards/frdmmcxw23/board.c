/*
 * Copyright 2024-2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "fsl_iocon.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_usart.h"
#include "fsl_flexcomm.h"
#include "pin_mux.h"
#include "fsl_power.h"

#if defined(SDK_I2C_BASED_COMPONENT_USED) && SDK_I2C_BASED_COMPONENT_USED
#include "fsl_i2c.h"
#endif /* SDK_I2C_BASED_COMPONENT_USED */

#if defined(ENABLE_RAM_VECTOR_TABLE)
/* By default vector table is in flash, and interrupt handlers cannot be updated.
 * When ENABLE_RAM_VECTOR_TABLE is enabled, vector table is copied to RAM on the first call to InstallIRQHandler
 * In SCB->VTOR register 6 lowest bits are reserved, so __VECTOR_RAM needs to be aligned on 0x80
 * Original vector table in flash is 0x140 bytes */
#define VECTOR_TABLE_SIZE_BYTES (0x140u)
#if defined(__IAR_SYSTEMS_ICC__)
#pragma data_alignment                                            = 0x200
uint32_t __VECTOR_RAM[VECTOR_TABLE_SIZE_BYTES / sizeof(uint32_t)] = {0};
#else
uint32_t __VECTOR_RAM[VECTOR_TABLE_SIZE_BYTES / sizeof(uint32_t)] __ALIGNED(0x200) = {0};
#endif
#endif

void BOARD_InitDebugConsole(void)
{
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);
    RESET_ClearPeripheralReset(BOARD_DEBUG_UART_RST);
    DbgConsole_Init(BOARD_DEBUG_UART_INSTANCE, BOARD_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE, BOARD_DEBUG_UART_CLK_FREQ);
}

void BOARD_UninitDebugConsole(void)
{
    CLOCK_DisableClock(BOARD_DEBUG_UART_CLKSRC);
}

void BOARD_InitDebugConsole2(void)
{
    CLOCK_AttachClk(BOARD_DEBUG2_UART_CLK_ATTACH);
    RESET_ClearPeripheralReset(BOARD_DEBUG2_UART_RST);
    DbgConsole_Init(BOARD_DEBUG2_UART_INSTANCE, BOARD_DEBUG2_UART_BAUDRATE, BOARD_DEBUG2_UART_TYPE, BOARD_DEBUG2_UART_CLK_FREQ);
}

void BOARD_UninitDebugConsole2(void)
{
    CLOCK_DisableClock(BOARD_DEBUG2_UART_CLKSRC);
}

#if defined(SDK_I2C_BASED_COMPONENT_USED) && SDK_I2C_BASED_COMPONENT_USED
/** I2C configuration */
static i2c_master_config_t s_i2cConfig;

/**
 * Writes the given data to the given address
 *
 * @param deviceAddress The address of the device to write to
 * @param subAddress The sub-address to write to
 * @param txBuff Buffer containing the data to write
 * @param txBuffSize The number of bytes in the buffer
 * @return kStatus_Success upon success
 */
status_t BOARD_I2cWriteFunc(uint8_t deviceAddress, uint32_t subAddress, uint8_t *txBuff, size_t txBuffSize)
{
    status_t result                  = kStatus_Success;
    i2c_master_transfer_t masterXfer = {0};

    masterXfer.slaveAddress   = deviceAddress;
    masterXfer.direction      = kI2C_Write;
    masterXfer.subaddress     = subAddress;
    masterXfer.subaddressSize = 1;
    masterXfer.data           = txBuff;
    masterXfer.dataSize       = txBuffSize;
    masterXfer.flags          = kI2C_TransferDefaultFlag;
    result                    = I2C_MasterTransferBlocking(BOARD_I2C_MASTER, &masterXfer);

    return result;
}

/**
 * Reads data from the given address
 *
 * @param deviceAddress The address of the device to read from
 * @param subAddress The sub-address to read from
 * @param txBuff Buffer to store the read data
 * @param txBuffSize The size of the receive buffer
 * @return kStatus_Success upon success
 */
status_t BOARD_I2cReadFunc(uint8_t deviceAddress, uint32_t subAddress, uint8_t *rxBuff, size_t rxBuffSize)
{
    i2c_master_transfer_t masterXfer = {0};

    masterXfer.slaveAddress   = deviceAddress;
    masterXfer.direction      = kI2C_Read;
    masterXfer.subaddress     = subAddress;
    masterXfer.subaddressSize = 1;
    masterXfer.data           = rxBuff;
    masterXfer.dataSize       = rxBuffSize;
    masterXfer.flags          = kI2C_TransferDefaultFlag;
    I2C_MasterTransferBlocking(BOARD_I2C_MASTER, &masterXfer);
    return 0;
}

void BOARD_InitI2c()
{
    /* Configure I2C */
    I2C_MasterGetDefaultConfig(&s_i2cConfig);
    s_i2cConfig.baudRate_Bps = BOARD_I2C_BAUDRATE;
    I2C_MasterInit(BOARD_I2C_MASTER, &s_i2cConfig, BOARD_I2C_MASTER_CLOCK_FREQUENCY);
}
#endif /* SDK_I2C_BASED_COMPONENT_USED */