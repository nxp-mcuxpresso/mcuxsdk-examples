/*
 * Copyright 2021 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "veneer_table.h"
#include "app.h"

#include "unity.h"
#include "fsl_puf_v3.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define PRINTF_NSE DbgConsole_Printf_NSE

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

static void TestGenerateRandomData(void)
{
    SDK_ALIGN(uint8_t data1[256], 8u) = {0};
    SDK_ALIGN(uint8_t data2[256], 8u) = {0};
    uint8_t zeroData[256]             = {0};
    status_t result;

    /* Use invalid buffer */
    result = PUF_GenerateRandom(PUF, NULL, 256);
    TEST_ASSERT_EQUAL(result, kStatus_InvalidArgument);

    /* Use unaligned buffer */
    result = PUF_GenerateRandom(PUF, &data1[1], 256);
    TEST_ASSERT_EQUAL(result, kStatus_InvalidArgument);

    /* Use invalid data size */
    result = PUF_GenerateRandom(PUF, data1, 123);
    TEST_ASSERT_EQUAL(result, kStatus_InvalidArgument);

    /* Use correct parameters */
    result = PUF_GenerateRandom(PUF, data1, sizeof(data1));
    TEST_ASSERT_EQUAL(result, kStatus_Success);
    result = PUF_GenerateRandom(PUF, data2, sizeof(data2));
    TEST_ASSERT_EQUAL(result, kStatus_Success);

    /* Check that data1 are not zero */
    TEST_ASSERT_FALSE(memcmp(data1, zeroData, sizeof(data1)) == 0);
    /* Check that data2 are not zero */
    TEST_ASSERT_FALSE(memcmp(data2, zeroData, sizeof(data2)) == 0);
    /* Check that data1 are not the same as data2 */
    TEST_ASSERT_FALSE(memcmp(data1, data2, sizeof(data1)) == 0);
}

static void TestSetLockSecurePrivilege(void)
{
    status_t result;

    /* Set Lock to Non-secure Privilege */
    result = PUF_SetLock(PUF, kPUF_SecurePrivilege);
    TEST_ASSERT_EQUAL(result, kStatus_Success);
}

void setUp(void)
{
}

void tearDown(void)
{
}

/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_InitHardware();

    PRINTF_NSE("\r\n\r\nWelcome in normal world!\r\n\r\n");

    UnityBegin();

    /* Use PUF as Non-Secure privilege */
    RUN_EXAMPLE(TestGenerateRandomData, MAKE_UNITY_NUM(k_unity_puf, 12));
    /* Set PUF to Secure privilege */
    RUN_EXAMPLE(TestSetLockSecurePrivilege, MAKE_UNITY_NUM(k_unity_puf, 13));

    /* Via non-secure callable set PUFv3 back to non-secure privilege */
    PRINTF_NSE("\r\nClaim PUFv3 lock to non-secure security level via non-secure callable\r\n\r\n");
    NSC_PUF_ClaimLock(kPUF_NonsecurePrivilege);

    /* Test to execute PUF command as non-secure privilege */
    RUN_EXAMPLE(TestGenerateRandomData, MAKE_UNITY_NUM(k_unity_puf, 14));
    /* Set PUF to Secure privilege again */
    RUN_EXAMPLE(TestSetLockSecurePrivilege, MAKE_UNITY_NUM(k_unity_puf, 15));

    PRINTF_NSE(
        "\r\nUsing PUFv3 from non-secure world while locked to secure security level should generate Hard Fault!:\r\n");
    /* Use PUFv3 from non-secure world while locked to secure, will generate Hardfalt! */
    RUN_EXAMPLE(TestGenerateRandomData, MAKE_UNITY_NUM(k_unity_puf, 14));

    UnityEnd();

    while (1)
    {
    }
}
