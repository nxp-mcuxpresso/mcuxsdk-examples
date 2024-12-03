/*
 * Copyright 2020 NXP
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
 * brief   Init for CRC-16-CCITT.
 * details Init CRC peripheral module for CRC-16/CCITT-FALSE protocol:
 *         width=16 poly=0x1021 init=0xffff refin=false(kCRC_TransposeBytes)
 *         refout=false(kCRC_TransposeNone) xorout=0x0000 check=0x29b1
 *         http://reveng.sourceforge.net/crc-catalogue/
 *         name="CRC-16/CCITT-FALSE"
 */
static void InitCrc16_CcittFalse(CRC_Type *base)
{
    crc_config_t sConfig;
    /*
     *   sConfig->u32CrcPolynomial          = 0x1021U;
     *   sConfig->u32CrcSeedValue           = 0xFFFFU;
     *   sConfig->eTransposeIn              = kCRC_TransposeBytes;
     *   sConfig->eTransposeOut             = kCRC_TransposeNone;
     *   sConfig->bEnableComplementChecksum = false;
     *   sConfig->eCrcBits                  = kCRC_Bits16;
     */
    CRC_GetDefaultConfig(&sConfig, kCRC_Crc16CCITT);
    CRC_Init(base, &sConfig);
}

/*!
 * brief   Init for CRC-16/KERMIT.
 * details Init CRC peripheral module for CRC-16/KERMIT protocol.
 *          width=16 poly=0x1021 init=0x0000 refin=true(kCRC_TransposeBitsAndBytes)
 *          refout=true(kCRC_TransposeBitsAndBytes) xorout=0x0000 check=0x2189 name="KERMIT"
 *          http://reveng.sourceforge.net/crc-catalogue/
 */
static void InitCrc16_Kermit(CRC_Type *base)
{
    crc_config_t sConfig;

    /*
     *   sConfig->u32CrcPolynomial          = 0x1021U;
     *   sConfig->u32CrcSeedValue           = 0x0000U;
     *   sConfig->eTransposeIn              = kCRC_TransposeBitsAndBytes;
     *   sConfig->eTransposeOut             = kCRC_TransposeBitsAndBytes;
     *   sConfig->bEnableComplementChecksum = false;
     *   sConfig->eCrcBits                  = kCRC_Bits16;
     */
    CRC_GetDefaultConfig(&sConfig, kCRC_Crc16Kermit);
    CRC_Init(base, &sConfig);
}

/*!
 * brief   Init for CRC-16/MAXIM.
 * details Init CRC peripheral module for CRC-16/MAXIM protocol.
 *          width=16 poly=0x8005 init=0x0000 refin=true(kCRC_TransposeBitsAndBytes)
 *           refout=true(kCRC_TransposeBitsAndBytes) xorout=0xffff check=0x44c2 name="CRC-16/MAXIM"
 *          http://reveng.sourceforge.net/crc-catalogue/
 */

static void InitCrc16(CRC_Type *base)
{
    crc_config_t sConfig;

    /*
     *   sConfig->u32CrcPolynomial          = 0x8005U;
     *   sConfig->u32CrcSeedValue           = 0x0000U;
     *   sConfig->eTransposeIn              = kCRC_TransposeBitsAndBytes;
     *   sConfig->eTransposeOut             = kCRC_TransposeBitsAndBytes;
     *   sConfig->bEnableComplementChecksum = true;
     *   sConfig->eCrcBits                  = kCRC_Bits16;
     */
    CRC_GetDefaultConfig(&sConfig, kCRC_Crc16);
    CRC_Init(base, &sConfig);
}

/*!
 * brief   Init for CRC-32/POSIX.
 * details Init CRC peripheral module for CRC-32/POSIX protocol.
 *          width=32 poly=0x04c11db7 init=0x00000000 refin=false(kCRC_TransposeBytes)
 *          refout=false(kCRC_TransposeNone) xorout=0xffffffff check=0x765e7680
 *          name="CRC-32/POSIX"
 *          http://reveng.sourceforge.net/crc-catalogue/
 */
static void InitCrc32_Posix(CRC_Type *base)
{
    crc_config_t sConfig;

    /*
     *   sConfig->u32CrcPolynomial          = 0x04C11DB7U;
     *   sConfig->u32CrcSeedValue           = 0x00000000U;
     *   sConfig->eTransposeIn              = kCRC_TransposeBytes;
     *   sConfig->eTransposeOut             = kCRC_TransposeNone;
     *   sConfig->bEnableComplementChecksum = true;
     *   sConfig->eCrcBits                  = kCRC_Bits32;
     */
    CRC_GetDefaultConfig(&sConfig, kCRC_Crc32Posix);
    CRC_Init(base, &sConfig);
}

/*!
 * brief   Init for CRC-32.
 * details Init CRC peripheral module for CRC-32 protocol.
 *          width=32 poly=0x04c11db7 init=0xffffffff refin=true (kCRC_TransposeBitsAndBytes)
 *          refout=true(kCRC_TransposeBitsAndBytes) xorout=0xffffffff check=0xcbf43926
 *          name="CRC-32"
 *          http://reveng.sourceforge.net/crc-catalogue/
 */
static void InitCrc32(CRC_Type *base)
{
    crc_config_t sConfig;

    /*
     *   sConfig->u32CrcPolynomial          = 0x04C11DB7U;
     *   sConfig->u32CrcSeedValue           = 0xFFFFFFFFU;
     *   sConfig->eTransposeIn              = kCRC_TransposeBitsAndBytes;
     *   sConfig->eTransposeOut             = kCRC_TransposeBitsAndBytes;
     *   sConfig->bEnableComplementChecksum = true;
     *   sConfig->eCrcBits                  = kCRC_Bits32;
     */
    CRC_GetDefaultConfig(&sConfig, kCRC_Crc32);
    CRC_Init(base, &sConfig);
}

/*!
 * brief Main function
 */
int main(void)
{
    char testData[]                     = "123456789";
    const uint16_t checkCcittFalseCrc16 = 0x29b1u;
    const uint16_t checkMaximCrc16      = 0x44c2u;
    const uint16_t checkKermitCrc16     = 0x2189u;
    const uint32_t checkCrc32           = 0xcbf43926u;
    const uint32_t checkPosixCrc32      = 0x765e7680u;

    uint16_t checksum16;
    uint32_t checksum32;

    /* Init hardware*/
    BOARD_InitHardware();

    PRINTF("CRC Peripheral Driver Example.\r\n");

    /****************************************************
     * Test same string base on different CRC protocols. *
     ****************************************************/
    PRINTF("\r\nTest same string base on different CRC protocols.\r\n");
    PRINTF(" test string: %s\r\n", testData);

    /* CRC-16/CCITT-FALSE */
    InitCrc16_CcittFalse(CRC);
    CRC_WriteData(CRC, (uint8_t *)&testData[0], sizeof(testData) - 1);
    checksum16 = CRC_Get16bitResult(CRC);

    PRINTF(" 1.CRC-16 CCITT FALSE: 0x%x\r\n", checksum16);
    if (checksum16 != checkCcittFalseCrc16)
    {
        PRINTF(" ...Check fail. Expected: 0x%x\r\n", checkCcittFalseCrc16);
    }

    /* CRC-16 KERMIT */
    InitCrc16_Kermit(CRC);
    CRC_WriteData(CRC, (uint8_t *)&testData[0], sizeof(testData) - 1);
    checksum16 = CRC_Get16bitResult(CRC);

    PRINTF(" 2.CRC-16 KERMIT: 0x%x\r\n", checksum16);
    if (checksum16 != checkKermitCrc16)
    {
        PRINTF(" ...Check fail. Expected: 0x%x\r\n", checkKermitCrc16);
    }

    /* CRC-16/MAXIM */
    InitCrc16(CRC);
    CRC_WriteData(CRC, (uint8_t *)&testData[0], sizeof(testData) - 1);
    checksum16 = CRC_Get16bitResult(CRC);

    PRINTF(" 3.CRC-16 MAXIM: 0x%x\r\n", checksum16);
    if (checksum16 != checkMaximCrc16)
    {
        PRINTF(" ...Check fail. Expected: 0x%x\r\n", checkMaximCrc16);
    }

    /* CRC-32 */
    InitCrc32(CRC);
    CRC_WriteData(CRC, (uint8_t *)&testData[0], sizeof(testData) - 1);
    checksum32 = CRC_Get32bitResult(CRC);

    PRINTF(" 4.CRC-32: 0x%lx\r\n", checksum32);
    if (checksum32 != checkCrc32)
    {
        PRINTF(" ...Check fail. Expected: 0x%lx\r\n", checkCrc32);
    }

    /* CRC-32/POSIX */
    InitCrc32_Posix(CRC);
    CRC_WriteData(CRC, (uint8_t *)&testData[0], sizeof(testData) - 1);
    checksum32 = CRC_Get32bitResult(CRC);

    PRINTF(" 5.CRC-32 POSIX: 0x%lx\r\n", checksum32);
    if (checksum32 != checkPosixCrc32)
    {
        PRINTF(" ...Check fail. Expected: 0x%lx\r\n", checkPosixCrc32);
    }

    /*********************************************************
     * Test different strings based on CRC-32 POSIX protocol. *
     *********************************************************/
    PRINTF("\r\nTest different strings based on CRC-32 POSIX protocol.\r\n");

    char crcTestData[]            = "abcdefgh";
    const uint32_t checkCrc32Data = 0x5024ec61;
    PRINTF(" 1.test string: %s\r\n", crcTestData);

    /* Should reconfigure seed value each time before doing new CRC check. */
    CRC_SetSeedValue(CRC, 0xFFFFFFFFU);
    CRC_WriteData(CRC, (uint8_t *)&crcTestData[0], sizeof(crcTestData) - 1);
    checksum32 = CRC_Get32bitResult(CRC);

    PRINTF("  CRC-32 POSIX: 0x%lx\r\n", checksum32);
    if (checksum32 != checkCrc32Data)
    {
        PRINTF("  ...Check fail. Expected: 0x%lx\r\n", checkCrc32Data);
    }

    char crcTestData2[]            = "abcd1234";
    const uint32_t checkCrc32Data2 = 0x3fbda0ab;
    PRINTF(" 2.test string: %s\r\n", crcTestData2);

    /* Should reconfigure seed value each time before doing new CRC check. */
    CRC_SetSeedValue(CRC, 0xFFFFFFFFU);
    CRC_WriteData(CRC, (uint8_t *)&crcTestData2[0], sizeof(crcTestData2) - 1);
    checksum32 = CRC_Get32bitResult(CRC);

    PRINTF("  CRC-32 POSIX: 0x%lx\r\n", checksum32);
    if (checksum32 != checkCrc32Data2)
    {
        PRINTF("  ...Check fail. Expected: 0x%lx\r\n", checkCrc32Data2);
    }

    while (1)
    {
    }
}
