/*
 * Copyright 2021 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#if (__ARM_FEATURE_CMSE & 1) == 0
#error "Need ARMv8-M security extensions"
#elif (__ARM_FEATURE_CMSE & 2) == 0
#error "Compile with --cmse"
#endif

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "arm_cmse.h"
#include "board.h"
#include "veneer_table.h"
#include "tzm_config.h"
#include "tzm_api.h"
#include "app.h"

#include "unity.h"
#include "fsl_puf_v3.h"

#define AHB_SECURE_CTRL AHBSC
#define SECTRL0         AHBSC

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define NON_SECURE_START DEMO_CODE_START_NS

typedef union
{
    struct ahb_secure_fault_info
    {
        unsigned access_type : 2;
        unsigned reserved0 : 2;
        unsigned master_sec_level : 2;
        unsigned antipol_master_sec_level : 2;
        unsigned master_number : 4;
        unsigned reserved : 20;
    } fault_info;
    unsigned value;
} ahb_secure_fault_info_t;

#define AHB_LAYERS_COUNT 10U

uint8_t SDK_ALIGN(g_AC[PUF_ACTIVATION_CODE_SIZE], 8u);
puf_config_t g_pufConfig;
/* User key in little-endian format. */
/* "Thispasswordisveryuncommonforher". */
static const uint8_t s_userKey[] __attribute__((aligned)) = {
    0x72, 0x65, 0x68, 0x72, 0x6f, 0x66, 0x6e, 0x6f, 0x6d, 0x6d, 0x6f, 0x63, 0x6e, 0x75, 0x79, 0x72,
    0x65, 0x76, 0x73, 0x69, 0x64, 0x72, 0x6f, 0x77, 0x73, 0x73, 0x61, 0x70, 0x73, 0x69, 0x68, 0x54};

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
static void TestSelftest(void)
{
    uint8_t score = 0;
    status_t result;

    PUF_GetDefaultConfig(&g_pufConfig);

    /* Initialize PUF */
    result = PUF_Init(PUF, &g_pufConfig);
    TEST_ASSERT_EQUAL(result, kStatus_Success);

    /* Run internal PUF self test */
    result = PUF_Test(PUF, &score);
    TEST_ASSERT_EQUAL(result, kStatus_Success);
    TEST_ASSERT_TRUE(score <= 7u);
}

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

static void TestEnroll(void)
{
    uint8_t pufScore                         = 0;
    uint8_t zeroAC[PUF_ACTIVATION_CODE_SIZE] = {0};
    SDK_ALIGN(uint8_t keyCode[PUF_GET_KEY_CODE_SIZE_FOR_KEY_SIZE(32)], 8u);
    SDK_ALIGN(uint8_t key[32], 8u);
    puf_key_ctx_t keyCtx;
    status_t result;

    /* Use invalid buffer */
    result = PUF_Enroll(PUF, NULL, PUF_ACTIVATION_CODE_SIZE, NULL);
    TEST_ASSERT_EQUAL(result, kStatus_InvalidArgument);

    /* Use unaligned buffer */
    result = PUF_Enroll(PUF, &g_AC[1], PUF_ACTIVATION_CODE_SIZE, NULL);
    TEST_ASSERT_EQUAL(result, kStatus_InvalidArgument);

    /* Use invalid buffer size */
    result = PUF_Enroll(PUF, g_AC, PUF_ACTIVATION_CODE_SIZE - 1, NULL);
    TEST_ASSERT_EQUAL(result, kStatus_InvalidArgument);

    /* Use correct parameters */
    result = PUF_Enroll(PUF, g_AC, sizeof(g_AC), &pufScore);
    TEST_ASSERT_EQUAL(result, kStatus_Success);
    TEST_ASSERT_TRUE(pufScore <= 7u);

    TEST_ASSERT_FALSE(memcmp(g_AC, zeroAC, PUF_ACTIVATION_CODE_SIZE) == 0);

    /* Test enrolled state key scope */
    keyCtx.keyScopeEnrolled = kPUF_KeyAllowAll;
    keyCtx.keyScopeStarted  = kPUF_KeyAllowAll;
    keyCtx.userCtx0         = 0u;
    keyCtx.userCtx1         = 0u;

    /* Test kPUF_KeyAllowAll */
    result = PUF_WrapGeneratedRandom(PUF, &keyCtx, 32u, keyCode, sizeof(keyCode));
    TEST_ASSERT_EQUAL(result, kStatus_Success);
    result = PUF_Unwrap(PUF, kPUF_KeyDestRegister, keyCode, sizeof(keyCode), key, sizeof(key));
    TEST_ASSERT_EQUAL(result, kStatus_Success);

    /* Test kPUF_KeyAllowRegister */
    keyCtx.keyScopeEnrolled = kPUF_KeyAllowRegister;
    result                  = PUF_WrapGeneratedRandom(PUF, &keyCtx, 32u, keyCode, sizeof(keyCode));
    TEST_ASSERT_EQUAL(result, kStatus_Success);
    result = PUF_Unwrap(PUF, kPUF_KeyDestRegister, keyCode, sizeof(keyCode), key, sizeof(key));
    TEST_ASSERT_EQUAL(result, kStatus_Success);

    /* Test kPUF_KeyAllowKeyBus */
    keyCtx.keyScopeEnrolled = kPUF_KeyAllowKeyBus;
    result                  = PUF_WrapGeneratedRandom(PUF, &keyCtx, 32u, keyCode, sizeof(keyCode));
    TEST_ASSERT_EQUAL(result, kStatus_Success);
    result = PUF_Unwrap(PUF, kPUF_KeyDestRegister, keyCode, sizeof(keyCode), key, sizeof(key));
    TEST_ASSERT_EQUAL(result, kStatus_PUF_DestinationNotAllowed);
}

static void TestStart(void)
{
    uint8_t pufScore = 0;
    SDK_ALIGN(uint8_t keyCode[PUF_GET_KEY_CODE_SIZE_FOR_KEY_SIZE(32)], 8U);
    SDK_ALIGN(uint8_t key[32], 8u);
    puf_key_ctx_t keyCtx;
    status_t result;

    PUF_Deinit(PUF, &g_pufConfig);
    result = PUF_Init(PUF, &g_pufConfig);
    TEST_ASSERT_EQUAL(result, kStatus_Success);

    /* Use invalid AC size */
    result = PUF_Start(PUF, g_AC, PUF_ACTIVATION_CODE_SIZE - 1, NULL);
    TEST_ASSERT_EQUAL(result, kStatus_InvalidArgument);

    /* Use invalid AC */
    result = PUF_Start(PUF, NULL, PUF_ACTIVATION_CODE_SIZE, NULL);
    TEST_ASSERT_EQUAL(result, kStatus_InvalidArgument);

    /* Use correct parameters */
    result = PUF_Start(PUF, g_AC, sizeof(g_AC), &pufScore);
    TEST_ASSERT_EQUAL(result, kStatus_Success);
    TEST_ASSERT_TRUE(pufScore <= 7u);

    /* Test started state key scope */
    keyCtx.keyScopeEnrolled = kPUF_KeyAllowAll;
    keyCtx.keyScopeStarted  = kPUF_KeyAllowAll;
    keyCtx.userCtx0         = 0u;
    keyCtx.userCtx1         = 0u;

    /* Test kPUF_KeyAllowAll */
    result = PUF_WrapGeneratedRandom(PUF, &keyCtx, 32u, keyCode, sizeof(keyCode));
    TEST_ASSERT_EQUAL(result, kStatus_Success);
    result = PUF_Unwrap(PUF, kPUF_KeyDestRegister, keyCode, sizeof(keyCode), key, sizeof(key));
    TEST_ASSERT_EQUAL(result, kStatus_Success);

    /* Test kPUF_KeyAllowRegister */
    keyCtx.keyScopeStarted = kPUF_KeyAllowRegister;
    result                 = PUF_WrapGeneratedRandom(PUF, &keyCtx, 32u, keyCode, sizeof(keyCode));
    TEST_ASSERT_EQUAL(result, kStatus_Success);
    result = PUF_Unwrap(PUF, kPUF_KeyDestRegister, keyCode, sizeof(keyCode), key, sizeof(key));
    TEST_ASSERT_EQUAL(result, kStatus_Success);

    /* Test kPUF_KeyAllowKeyBus */
    keyCtx.keyScopeStarted = kPUF_KeyAllowKeyBus;
    result                 = PUF_WrapGeneratedRandom(PUF, &keyCtx, 32u, keyCode, sizeof(keyCode));
    TEST_ASSERT_EQUAL(result, kStatus_Success);
    result = PUF_Unwrap(PUF, kPUF_KeyDestRegister, keyCode, sizeof(keyCode), key, sizeof(key));
    TEST_ASSERT_EQUAL(result, kStatus_PUF_DestinationNotAllowed);
}

static void TestDeriveDeviceKey(void)
{
    puf_key_ctx_t keyCtx;
    status_t result;
    SDK_ALIGN(uint8_t key1[512], 8u) = {0};
    SDK_ALIGN(uint8_t key2[512], 8u) = {0};
    uint8_t keyZero[512]             = {0};

    /* Fill in key context */
    keyCtx.keyScopeEnrolled = kPUF_KeyAllowAll;
    keyCtx.keyScopeStarted  = kPUF_KeyAllowAll;
    keyCtx.userCtx0         = 0u;
    keyCtx.userCtx1         = 0u;

    /* Use invalid context */
    result = PUF_GetKey(PUF, NULL, kPUF_KeyDestRegister, key1, sizeof(key1));
    TEST_ASSERT_EQUAL(result, kStatus_InvalidArgument);

    /* Use invalid key destination */
    result = PUF_GetKey(PUF, &keyCtx, kPUF_KeyDestInvalid, key1, sizeof(key1));
    TEST_ASSERT_EQUAL(result, kStatus_InvalidArgument);

    /* Use invalid key buffer */
    result = PUF_GetKey(PUF, &keyCtx, kPUF_KeyDestRegister, NULL, sizeof(key1));
    TEST_ASSERT_EQUAL(result, kStatus_InvalidArgument);

    /* Use invalid key size */
    result = PUF_GetKey(PUF, &keyCtx, kPUF_KeyDestRegister, key1, 127u);
    TEST_ASSERT_EQUAL(result, kStatus_InvalidArgument);

    /* Derive first device key */
    result = PUF_GetKey(PUF, &keyCtx, kPUF_KeyDestRegister, key1, sizeof(key1));
    TEST_ASSERT_EQUAL(result, kStatus_Success);

    /* Derive second device key - need different context */
    keyCtx.userCtx1 = 2u; /* possible values here is limited by qk_restrict_user_context_0 */
    result          = PUF_GetKey(PUF, &keyCtx, kPUF_KeyDestRegister, key2, sizeof(key2));
    TEST_ASSERT_EQUAL(result, kStatus_Success);

    /* Check that key1 is not zero */
    TEST_ASSERT_FALSE(memcmp(keyZero, key1, sizeof(key1)) == 0);
    /* Check that key2 is not zero */
    TEST_ASSERT_FALSE(memcmp(keyZero, key2, sizeof(key1)) == 0);
    /* Check that key1 is not the same as key2 */
    TEST_ASSERT_FALSE(memcmp(key1, key2, sizeof(key1)) == 0);
}

static void TestRandomWrapUnwrap(void)
{
    puf_key_ctx_t keyCtx;
    SDK_ALIGN(uint8_t randKey[128], 8u);
    uint8_t zeroKey[sizeof(randKey)] = {0};
    SDK_ALIGN(uint8_t keyCode[PUF_GET_KEY_CODE_SIZE_FOR_KEY_SIZE(sizeof(randKey))], 8u);
    status_t result;

    /* Fill in key context */
    keyCtx.keyScopeEnrolled = kPUF_KeyAllowRegister;
    keyCtx.keyScopeStarted  = kPUF_KeyAllowRegister;
    keyCtx.userCtx0         = 0u;
    keyCtx.userCtx1         = 0u;

    /* Use invalid context */
    result = PUF_WrapGeneratedRandom(PUF, NULL, sizeof(randKey), keyCode, sizeof(keyCode));
    TEST_ASSERT_EQUAL(result, kStatus_InvalidArgument);

    /* Use invalid key size */
    result = PUF_WrapGeneratedRandom(PUF, &keyCtx, 66u, keyCode, sizeof(keyCode));
    TEST_ASSERT_EQUAL(result, kStatus_InvalidArgument);

    /* Use invalid keyCode buffer */
    result = PUF_WrapGeneratedRandom(PUF, &keyCtx, sizeof(randKey), NULL, sizeof(keyCode));
    TEST_ASSERT_EQUAL(result, kStatus_InvalidArgument);

    /* Use invalid keyCode size */
    result = PUF_WrapGeneratedRandom(PUF, &keyCtx, 256u, NULL, (PUF_GET_KEY_CODE_SIZE_FOR_KEY_SIZE(256) - 1));
    TEST_ASSERT_EQUAL(result, kStatus_InvalidArgument);

    /* Use correct parameters */
    result = PUF_WrapGeneratedRandom(PUF, &keyCtx, sizeof(randKey), keyCode, sizeof(keyCode));
    TEST_ASSERT_EQUAL(result, kStatus_Success);

    /* Use invalid key destination */
    result = PUF_Unwrap(PUF, kPUF_KeyDestInvalid, keyCode, sizeof(keyCode), randKey, sizeof(randKey));
    TEST_ASSERT_EQUAL(result, kStatus_InvalidArgument);

    /* Use invalid keyCode */
    result = PUF_Unwrap(PUF, kPUF_KeyDestRegister, NULL, sizeof(keyCode), randKey, sizeof(randKey));
    TEST_ASSERT_EQUAL(result, kStatus_InvalidArgument);

    /* Use unaligned keyCode */
    result = PUF_Unwrap(PUF, kPUF_KeyDestRegister, &keyCode[1], sizeof(keyCode), randKey, sizeof(randKey));
    TEST_ASSERT_EQUAL(result, kStatus_InvalidArgument);

    /* Use invalid key buffer */
    result = PUF_Unwrap(PUF, kPUF_KeyDestRegister, keyCode, sizeof(keyCode), NULL, sizeof(randKey));
    TEST_ASSERT_EQUAL(result, kStatus_InvalidArgument);

    /* Use different key destination than key scope */
    result = PUF_Unwrap(PUF, kPUF_KeyDestKeyBus, keyCode, sizeof(keyCode), NULL, sizeof(randKey));
    TEST_ASSERT_EQUAL(result, kStatus_PUF_DestinationNotAllowed);

    /* Use correct parameters */
    result = PUF_Unwrap(PUF, kPUF_KeyDestRegister, keyCode, sizeof(keyCode), randKey, sizeof(randKey));
    TEST_ASSERT_EQUAL(result, kStatus_Success);

    /* randKey must contain non-zero data */
    TEST_ASSERT_FALSE(memcmp(randKey, zeroKey, sizeof(randKey)) == 0);
}

static void TestUserKeyWrapUnwrap(void)
{
    puf_key_ctx_t keyCtx;
    SDK_ALIGN(uint8_t keyCode[PUF_GET_KEY_CODE_SIZE_FOR_KEY_SIZE(sizeof(s_userKey))], 8u);
    SDK_ALIGN(uint8_t key[sizeof(s_userKey)], 8u);
    status_t result;

    /* Fill in key context */
    keyCtx.keyScopeEnrolled = kPUF_KeyAllowRegister;
    keyCtx.keyScopeStarted  = kPUF_KeyAllowRegister;
    keyCtx.userCtx0         = 0u;
    keyCtx.userCtx1         = 0u;

    /* Use invalid context */
    result = PUF_Wrap(PUF, NULL, (uint8_t *)s_userKey, sizeof(s_userKey), keyCode, sizeof(keyCode));
    TEST_ASSERT_EQUAL(result, kStatus_InvalidArgument);

    /* Use unaligned key */
    result = PUF_Wrap(PUF, &keyCtx, (uint8_t *)&s_userKey[1], sizeof(s_userKey), keyCode, sizeof(keyCode));
    TEST_ASSERT_EQUAL(result, kStatus_InvalidArgument);

    /* Use invalid key size */
    result = PUF_Wrap(PUF, &keyCtx, (uint8_t *)s_userKey, 192u, keyCode, sizeof(keyCode));
    TEST_ASSERT_EQUAL(result, kStatus_InvalidArgument);

    /* Use invalid key code buffer */
    result = PUF_Wrap(PUF, &keyCtx, (uint8_t *)s_userKey, sizeof(s_userKey), NULL, sizeof(keyCode));
    TEST_ASSERT_EQUAL(result, kStatus_InvalidArgument);

    /* Use unaligned key code buffer */
    result = PUF_Wrap(PUF, &keyCtx, (uint8_t *)s_userKey, sizeof(s_userKey), &keyCode[1], sizeof(keyCode));
    TEST_ASSERT_EQUAL(result, kStatus_InvalidArgument);

    /* Use invalid key code size */
    result = PUF_Wrap(PUF, &keyCtx, (uint8_t *)s_userKey, sizeof(s_userKey), keyCode, sizeof(keyCode) - 1);
    TEST_ASSERT_EQUAL(result, kStatus_InvalidArgument);

    /* Use correct parameters */
    result = PUF_Wrap(PUF, &keyCtx, (uint8_t *)s_userKey, sizeof(s_userKey), keyCode, sizeof(keyCode));
    TEST_ASSERT_EQUAL(result, kStatus_Success);

    /* Use different key destination than key scope */
    result = PUF_Unwrap(PUF, kPUF_KeyDestKeyBus, keyCode, sizeof(keyCode), NULL, sizeof(key));
    TEST_ASSERT_EQUAL(result, kStatus_PUF_DestinationNotAllowed);

    /* Use correct parameters */
    result = PUF_Unwrap(PUF, kPUF_KeyDestRegister, keyCode, sizeof(keyCode), key, sizeof(key));
    TEST_ASSERT_EQUAL(result, kStatus_Success);

    /* Check that unwrapped key is equal to s_userKey */
    TEST_ASSERT_TRUE(memcmp(key, s_userKey, sizeof(key)) == 0);
}

static void TestStop(void)
{
    SDK_ALIGN(uint8_t data[128], 8u);
    puf_key_ctx_t keyCtx;
    status_t result;

    /* Stop PUF */
    result = PUF_Stop(PUF);
    TEST_ASSERT_EQUAL(result, kStatus_Success);

    /* Generate random data operation should be alowed */
    result = PUF_GenerateRandom(PUF, data, sizeof(data));
    TEST_ASSERT_EQUAL(result, kStatus_Success);

    /* Key operations must be rejected in stopped state */
    result = PUF_GetKey(PUF, &keyCtx, kPUF_KeyDestRegister, data, sizeof(data));
    TEST_ASSERT_EQUAL(result, kStatus_PUF_OperationNotAllowed);
}

static void TestZeroize(void)
{
    status_t result;

    /* Zeroize PUF */
    result = PUF_Zeroize(PUF);
    TEST_ASSERT_EQUAL(result, kStatus_Success);

    /* Allow register must be set to zero when zeroized */
    TEST_ASSERT_EQUAL(PUF->AR, 0u);
}

static void TestSetLockNonSecurePrivilege(void)
{
    status_t result;

    /* Set Lock to Non-secure Privilege */
    result = PUF_SetLock(PUF, kPUF_NonsecurePrivilege);
    TEST_ASSERT_EQUAL(result, kStatus_Success);
}

static void TestSetLockSecurePrivilege(void)
{
    status_t result;

    /* Set Lock to Secure Privilege */
    /* Note: Usually Secure world don't use Non-Secure aliases, this is just for demonstration purpose */
    result = PUF_SetLock(PUF_NS, kPUF_SecurePrivilege);
    TEST_ASSERT_EQUAL(result, kStatus_Success);
}

/*!
 * @brief Application-specific implementation of the SystemInitHook() weak function.
 */
void SystemInitHook(void)
{
    /* The TrustZone should be configured as early as possible after RESET.
     * Therefore it is called from SystemInit() during startup. The SystemInitHook() weak function
     * overloading is used for this purpose.
     */
    BOARD_InitTrustZone();
}

/*!
 * @brief HardFault handler. This handler can called from both normal and secure world
 */
void HardFault_Handler(void)
{
    uint32_t ahb_violation_status;
    uint32_t i;
    ahb_secure_fault_info_t ahb_violation_info;

    /* Handling SAU related secure faults */
    PRINTF("\r\nEntering HardFault interrupt!\r\n");
    if (SAU->SFSR != 0)
    {
        if (SAU->SFSR & SAU_SFSR_INVEP_Msk)
        {
            /* Invalid Secure state entry point */
            PRINTF("SAU->SFSR:INVEP fault: Invalid entry point to secure world.\r\n");
        }
        else if (SAU->SFSR & SAU_SFSR_AUVIOL_Msk)
        {
            /* AUVIOL: SAU violation  */
            PRINTF("SAU->SFSR:AUVIOL fault: SAU violation. Access to secure memory from normal world.\r\n");
        }
        else if (SAU->SFSR & SAU_SFSR_INVTRAN_Msk)
        {
            /* INVTRAN: Invalid transition from secure to normal world  */
            PRINTF("SAU->SFSR:INVTRAN fault: Invalid transition from secure to normal world.\r\n");
        }
        else
        {
            PRINTF("Another SAU error.\r\n");
        }
        if (SAU->SFSR & SAU_SFSR_SFARVALID_Msk)
        {
            /* SFARVALID: SFAR contain valid address that caused secure violation */
            PRINTF("Address that caused SAU violation is 0x%X.\r\n", SAU->SFAR);
        }
    }

    /* Handling secure bus related faults */
    if (SCB->CFSR != 0)
    {
        if (SCB->CFSR & SCB_CFSR_IBUSERR_Msk)
        {
            /* IBUSERR: Instruction bus error on an instruction prefetch */
            PRINTF("SCB->BFSR:IBUSERR fault: Instruction bus error on an instruction prefetch.\r\n");
        }
        else if (SCB->CFSR & SCB_CFSR_PRECISERR_Msk)
        {
            /* PRECISERR: Instruction bus error on an instruction prefetch */
            PRINTF("SCB->BFSR:PRECISERR fault: Precise data access error.\r\n");
        }
        else
        {
            PRINTF("Another secure bus error.\r\n");
        }
        if (SCB->CFSR & SCB_CFSR_BFARVALID_Msk)
        {
            /* BFARVALID: BFAR contain valid address that caused secure violation */
            PRINTF("Address that caused secure bus violation is 0x%X.\r\n", SCB->BFAR);
        }
    }

    /* Handling non-secure bus related faults */
    if (SCB_NS->CFSR != 0)
    {
        if (SCB_NS->CFSR & SCB_CFSR_IBUSERR_Msk)
        {
            /* IBUSERR: Instruction bus error on an instruction prefetch */
            PRINTF("SCB_NS->BFSR:IBUSERR fault: Instruction bus error on an instruction prefetch.\r\n");
        }
        else if (SCB_NS->CFSR & SCB_CFSR_PRECISERR_Msk)
        {
            /* PRECISERR: Data bus error on an data read/write */
            PRINTF("SCB_NS->BFSR:PRECISERR fault: Precise data access error.\r\n");
        }
        else
        {
            PRINTF("Another secure bus error.\r\n");
        }
        if (SCB_NS->CFSR & SCB_CFSR_BFARVALID_Msk)
        {
            /* BFARVALID: BFAR contain valid address that caused secure violation */
            PRINTF("Address that caused secure bus violation is 0x%X.\r\n", SCB_NS->BFAR);
        }
    }

    /* Handling AHB secure controller related faults.
     * AHB secure controller faults raise secure bus fault. Detail fault info
     * can be read from AHB secure controller violation registers */
    ahb_violation_status = AHB_SECURE_CTRL->SEC_VIO_INFO_VALID;
    if (ahb_violation_status != 0)
    {
        PRINTF("\r\nAdditional AHB secure controller error information:\r\n");
        for (i = 0; i < (AHB_LAYERS_COUNT - 1); i++)
        {
            if (ahb_violation_status & 0x1U)
            {
                ahb_violation_info.value = AHB_SECURE_CTRL->SEC_VIO_MISC_INFO[i];
                PRINTF("Secure error at AHB layer %d.\r\n", i);
                PRINTF("Address that caused secure violation is 0x%X.\r\n", AHB_SECURE_CTRL->SEC_VIO_ADDR[i]);
                PRINTF("Secure error caused by bus master number %d.\r\n", ahb_violation_info.fault_info.master_number);
                PRINTF("Security level of master %d.\r\n", ahb_violation_info.fault_info.master_sec_level);
                PRINTF("Secure error happened during ");
                switch (ahb_violation_info.fault_info.access_type)
                {
                    case 0:
                        PRINTF("read code access.\r\n");
                        break;
                    case 2:
                        PRINTF("read data access.\r\n");
                        break;
                    case 3:
                        PRINTF("read code access.\r\n");
                        break;
                    default:
                        PRINTF("unknown access.\r\n");
                        break;
                }
            }
            ahb_violation_status = ahb_violation_status >> 1;
        }
    }
    /* This while loop is used for demonstration purposes. Otherwise reset should be invoked */
    while (1)
    {
    }
    /* Perform system RESET */
    SCB->AIRCR =
        (SCB->AIRCR & ~SCB_AIRCR_VECTKEY_Msk) | (0x05FAUL << SCB_AIRCR_VECTKEY_Pos) | SCB_AIRCR_SYSRESETREQ_Msk;
}

/* Unity set up function */
void setUp(void)
{
}

/* Unity tear down function */
void tearDown(void)
{
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Init board hardware. */
    BOARD_InitHardware();

    PRINTF("Hello from secure world!\r\n\r\n");

    UnityBegin();

    RUN_EXAMPLE(TestSelftest, MAKE_UNITY_NUM(k_unity_puf, 0));
    RUN_EXAMPLE(TestGenerateRandomData, MAKE_UNITY_NUM(k_unity_puf, 1));
    RUN_EXAMPLE(TestEnroll, MAKE_UNITY_NUM(k_unity_puf, 2));
    RUN_EXAMPLE(TestStart, MAKE_UNITY_NUM(k_unity_puf, 3));
    RUN_EXAMPLE(TestDeriveDeviceKey, MAKE_UNITY_NUM(k_unity_puf, 4));
    RUN_EXAMPLE(TestRandomWrapUnwrap, MAKE_UNITY_NUM(k_unity_puf, 5));
    RUN_EXAMPLE(TestUserKeyWrapUnwrap, MAKE_UNITY_NUM(k_unity_puf, 6));
    RUN_EXAMPLE(TestStop, MAKE_UNITY_NUM(k_unity_puf, 7));
    RUN_EXAMPLE(TestZeroize, MAKE_UNITY_NUM(k_unity_puf, 8));
    /* Set PUF SEC Level to Non-secure privilege */
    RUN_EXAMPLE(TestSetLockNonSecurePrivilege, MAKE_UNITY_NUM(k_unity_puf, 9));
    /* Set PUF SEC Level to Secure privilege via Non-secure alias */
    RUN_EXAMPLE(TestSetLockSecurePrivilege, MAKE_UNITY_NUM(k_unity_puf, 10));
    /* Use PUF as Secure-privilege */
    RUN_EXAMPLE(TestSelftest, MAKE_UNITY_NUM(k_unity_puf, 11));
    /* Set again PUF SEC Level to Non-secure privilege for Non-secure world*/
    RUN_EXAMPLE(TestSetLockNonSecurePrivilege, MAKE_UNITY_NUM(k_unity_puf, 11));

    PRINTF("\r\nEntering normal world.\r\n");

    /* Call non-secure privilege application - jump to normal world */
    /* Note: since we are not configuring MPU, both applications are privilege */
    TZM_JumpToNormalWorld(NON_SECURE_START);

    while (1)
    {
        /* This point should never be reached */
    }
}
