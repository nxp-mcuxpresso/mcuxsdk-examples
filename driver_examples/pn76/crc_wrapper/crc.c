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
 * @brief Main function
 */
int main(void)
{
    status_t status;
    uint32_t crcCalcValue[1] = {0};
    uint8_t testData[]       = "123456789";
    uint8_t dataLen          = sizeof(testData) - 1;

    BOARD_InitHardware();

    PRINTF("\r\nTest same string base on different CRC protocols.\r\n");
    PRINTF(" test string: %s\r\n", testData);

    CRC_Init();

    status = CRC_CalculateCrc(0x0U, testData, dataLen, crcCalcValue);

    if (status != kStatus_Success)
        PRINTF(" Check CRC-16 fail\r\n");
    else
    {
        if (crcCalcValue[0] == 0x2189U)
            PRINTF(" 1.CRC-16 : 0x%x\r\n", crcCalcValue[0]);
        else
            PRINTF(" Check CRC-16 fail... Expected: 0x2189 Actual: 0x%x\r\n", crcCalcValue);
    }

    status = CRC_CalculateCrc(0xFFFFFFFFU, testData, dataLen, crcCalcValue);

    if (status != kStatus_Success)
        PRINTF(" Check CRC-32 fail\r\n");
    else
    {
        crcCalcValue[0] ^= 0xFFFFFFFFUL;

        if (crcCalcValue[0] == 0xCBF43926UL)
            PRINTF(" 2.CRC-32 : 0x%x\r\n", crcCalcValue[0]);
        else
            PRINTF(" Check CRC-32 fail... Expected: 0xCBF43926 Actual: 0x%x\r\n", crcCalcValue);
    }

    while (1)
    {
    }
}
