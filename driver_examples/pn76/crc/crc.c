/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_crc.h"
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

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * brief   Example for CRC-16.
 * details Set the CRC-16 configuration and check the value in result register.
 *         width=16 poly=0x1021 init=0x0000 refin=true refout=true xorout=0x0000
 *         check=0x2189U name="CRC-16"
 */
void EXAMPLE_CRC16(CRC_Type *base, crc_type_t crcType, uint8_t *buffer, uint8_t dataLen)
{
    uint32_t crcCalcValue;

    crc_config_t crc16Config;

    CRC_GetDefaultConfig(&crc16Config);

    CRC_SetConfig(base, &crc16Config);

    CRC_SetSeed(base, 0x0000U);

    CRC_WriteData(base, buffer, dataLen);

    crcCalcValue = CRC_GetResult(base);

    if (crcCalcValue == 0x2189U)
        PRINTF(" 1.CRC-16 : 0x%x\r\n", crcCalcValue);
    else
        PRINTF(" Check CRC-16 fail... Expected: 0x%x Actual: 0x%x\r\n", 0x2189U, crcCalcValue);
}

/*!
 * brief   Example for CRC-32.
 * details Set the CRC-32 configuration and check the value in result register.
 *         width=32 poly=0x04C11DB7 init=0xffffffff refin=true refout=true
 *         xorout=0xffffffff check=0x340BC6D9 name="CRC-32"
 */
void EXAMPLE_CRC32(CRC_Type *base, crc_type_t crcType, uint8_t *buffer, uint8_t dataLen)
{
    uint32_t crcCalcValue;

    crc_config_t crc32Config;

    CRC_GetDefaultConfig(&crc32Config);

    crc32Config.crcMode = kCRC_Crc32;

    CRC_SetConfig(base, &crc32Config);

    CRC_SetSeed(base, 0xFFFFFFFFU);

    CRC_WriteData(base, buffer, dataLen);

    crcCalcValue = CRC_GetResult(base);

    crcCalcValue ^= 0xFFFFFFFFUL;

    if (crcCalcValue == 0xCBF43926UL)
        PRINTF(" 2.CRC-32 : 0x%x\r\n", crcCalcValue);
    else
        PRINTF(" Check CRC-32 fail... Expected: 0x%x Actual: 0x%x\r\n", 0xCBF43926UL, crcCalcValue);
}

/*!
 * @brief Main function
 */
int main(void)
{
    uint8_t testData[] = "123456789";
    uint8_t dataLen    = sizeof(testData) - 1;
    /* Init board hardware. */
    BOARD_InitHardware();

    PRINTF("\r\nTest same string base on different CRC protocols.\r\n");
    PRINTF(" test string: %s\r\n", testData);

    CRC_Init(CRC);
    /* Check CRC-16 and CRC-32 protocol. */
    EXAMPLE_CRC16(CRC, kCRC_Crc16, testData, dataLen);
    EXAMPLE_CRC32(CRC, kCRC_Crc32, testData, dataLen);
    while (1)
    {
    }
}
