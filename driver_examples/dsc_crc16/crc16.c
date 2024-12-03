/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_crc.h"
#include "fsl_common.h"

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
 * brief Main function
 */
int main(void)
{
    char testData[]              = "123456789";
    const uint16_t Crc16Kermit   = 0x2189U;
    const uint16_t Crc16CCITT    = 0x29b1U;
    const uint16_t CrcA          = 0xBF05U;
    const uint16_t Crc16Riello   = 0x63D0U;
    const uint16_t Crc16Tms37157 = 0x26B1U;
    const uint16_t Crc16Mcrf4xx  = 0x6F91U;
    const uint16_t Crc16Augccitt = 0xE5CCU;
    const uint16_t Crc16Xmodem   = 0x31C3U;

    crc_config_t sConfig;
    uint16_t checksum16;

    /* Init hardware */
    BOARD_InitHardware();
    PRINTF("CRC-16 Driver Example.\r\n");

    /****************************************************
     * Test same string base on different CRC seed value. *
     ****************************************************/
    PRINTF(" test string: %s\r\n", testData);

    /* Init the CRC default peripheral module. */
    CRC_GetDefaultConfig(&sConfig);
    CRC_Init(CRC, &sConfig);
    CRC_WriteData(CRC, (uint8_t *)&testData[0], sizeof(testData) - 1, false);
    checksum16 = CRC_Get16bitResult(CRC, false);
    PRINTF(" CRC-16 CCIT FALSE: 0x%x\r\n", checksum16);
    if (checksum16 != Crc16CCITT)
    {
        PRINTF(" ..CRC16 CCITT FALSE Check fail. Expected: 0x%x\r\n", Crc16CCITT);
    }

    /* Change a different CRC module. */
    /* CRC-16/KERMIT */
    CRC_SetSeedValue(CRC, kCRC_Crc16Kermit);
    CRC_WriteData(CRC, (uint8_t *)&testData[0], sizeof(testData) - 1, true);
    checksum16 = CRC_Get16bitResult(CRC, true);
    PRINTF(" 1.CRC-16/KERMIT : 0x%x\r\n", checksum16);
    if (checksum16 != Crc16Kermit)
    {
        PRINTF(" ..CRC-16/KERMIT Check fail. Expected: 0x%x\r\n", Crc16Kermit);
    }

    /* CRC-16/XMODEM */
    CRC_SetSeedValue(CRC, kCRC_Crc16Xmodem);
    CRC_WriteData(CRC, (uint8_t *)&testData[0], sizeof(testData) - 1, false);
    checksum16 = CRC_Get16bitResult(CRC, false);
    PRINTF(" 2.CRC-16/XMODEM : 0x%x\r\n", checksum16);
    if (checksum16 != Crc16Xmodem)
    {
        PRINTF(" ..CRC-16/XMODEM Check fail. Expected: 0x%x\r\n", Crc16Xmodem);
    }

    /* CRC-16/RIELLO */
    CRC_SetSeedValue(CRC, kCRC_Crc16Riello);
    CRC_WriteData(CRC, (uint8_t *)&testData[0], sizeof(testData) - 1, true);
    checksum16 = CRC_Get16bitResult(CRC, true);
    PRINTF(" 3.CRC-16/RIELLO : 0x%x\r\n", checksum16);
    if (checksum16 != Crc16Riello)
    {
        PRINTF(" ..CRC-16/RIELLO Check fail. Expected: 0x%x\r\n", Crc16Riello);
    }

    /* CRC-16/TMS37157 */
    CRC_SetSeedValue(CRC, kCRC_Crc16Tms37157);
    CRC_WriteData(CRC, (uint8_t *)&testData[0], sizeof(testData) - 1, true);
    checksum16 = CRC_Get16bitResult(CRC, true);
    PRINTF(" 4.CRC-16/TMS37157 : 0x%x\r\n", checksum16);
    if (checksum16 != Crc16Tms37157)
    {
        PRINTF(" ..CRC-16/TMS37157 Check fail. Expected: 0x%x\r\n", Crc16Tms37157);
    }

    /* CRC-16-MCRF4XX */
    CRC_SetSeedValue(CRC, kCRC_Crc16Mcrf4xx);
    CRC_WriteData(CRC, (uint8_t *)&testData[0], sizeof(testData) - 1, true);
    checksum16 = CRC_Get16bitResult(CRC, true);
    PRINTF(" 5.CRC-16-MCRF4XX : 0x%x\r\n", checksum16);
    if (checksum16 != Crc16Mcrf4xx)
    {
        PRINTF(" ..CRC-16-MCRF4XX Check fail. Expected: 0x%x\r\n", Crc16Mcrf4xx);
    }

    /* CRC-16/AUG-CCITT */
    CRC_SetSeedValue(CRC, kCRC_Crc16Augccitt);
    CRC_WriteData(CRC, (uint8_t *)&testData[0], sizeof(testData) - 1, false);
    checksum16 = CRC_Get16bitResult(CRC, false);
    PRINTF(" 6.CRC-16/AUG-CCITT : 0x%x\r\n", checksum16);
    if (checksum16 != Crc16Augccitt)
    {
        PRINTF(" ..CRC-16/AUG-CCITT Check fail. Expected: 0x%x\r\n", Crc16Augccitt);
    }

    /* CRC/CRCA */
    CRC_SetSeedValue(CRC, kCRC_CrcA);
    CRC_WriteData(CRC, (uint8_t *)&testData[0], sizeof(testData) - 1, true);
    checksum16 = CRC_Get16bitResult(CRC, true);
    PRINTF(" 7.CRC/CRCA : 0x%x\r\n", checksum16);
    if (checksum16 != CrcA)
    {
        PRINTF(" ..CRC/CRCA Check fail. Expected: 0x%x\r\n", CrcA);
    }

    while (1)
    {
    }
}
