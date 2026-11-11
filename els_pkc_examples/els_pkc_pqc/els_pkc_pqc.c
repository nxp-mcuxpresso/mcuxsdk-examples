/*
 * Copyright 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "els_pkc_pqc.h"
#include "mcux_els.h"
#include "mcux_pkc.h"
#if defined(FSL_FEATURE_SOC_TRNG_COUNT) && (FSL_FEATURE_SOC_TRNG_COUNT > 0)
#include "fsl_trng.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#if (defined(PKC0) && !defined(PKC))
#define PKC PKC0
#endif

typedef uint32_t (*els_pkc_pqc_example_t)(void);

typedef struct
{
    const char *name;
    els_pkc_pqc_example_t run;
} els_pkc_pqc_test_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/
static const els_pkc_pqc_test_t s_tests[] = {
    {"MlDsa44 keygen, sign and verify:", mcuxClSignature_MlDsa44_KeyGenSignVerify_example},
    {"MlDsa44 verify example:", mcuxClSignature_MlDsa44_Verify_example},
    {"MlDsa65 keygen, sign and verify:", mcuxClSignature_MlDsa65_KeyGenSignVerify_example},
    {"MlDsa65 verify example:", mcuxClSignature_MlDsa65_Verify_example},
    {"MlDsa87 keygen, sign and verify:", mcuxClSignature_MlDsa87_KeyGenSignVerify_example},
    {"MlDsa87 verify example:", mcuxClSignature_MlDsa87_Verify_example},
};

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    char ch;
    uint8_t pass = 0;
    uint8_t fail = 0;

    /* Init board hardware. */
    BOARD_InitHardware();

    /* Enable ELS and related clocks */
    if (ELS_PowerDownWakeupInit(ELS) != kStatus_Success)
    {
        PRINTF("\r\nELS init failed\r\n");
        return kStatus_Fail;
    }

    /* Enable PKC related clocks and RAM zeroize */
    if (PKC_PowerDownWakeupInit(PKC) != kStatus_Success)
    {
        PRINTF("\r\nPKC init failed\r\n");
        return kStatus_Fail;
    }

#if defined(FSL_FEATURE_SOC_TRNG_COUNT) && (FSL_FEATURE_SOC_TRNG_COUNT > 0)
    /* Initialize the TRNG, it is the entropy source for the DRBG used by ML-DSA */
    {
        trng_config_t trngConfig;
        TRNG_GetDefaultConfig(&trngConfig);
        if (TRNG_Init(TRNG, &trngConfig) != kStatus_Success)
        {
            PRINTF("\r\nTRNG init failed\r\n");
            return kStatus_Fail;
        }
    }
#endif

    PRINTF("\r\nELS PKC PQC example\r\n");
    PRINTF("\r\n============================\r\n");

    for (uint32_t i = 0U; i < (sizeof(s_tests) / sizeof(s_tests[0])); i++)
    {
        PRINTF("%s", s_tests[i].name);
        if (s_tests[i].run() == ELS_PKC_EXAMPLE_STATUS_OK)
        {
            pass++;
            PRINTF("pass \r\n");
        }
        else
        {
            fail++;
            PRINTF("fail \r\n");
        }
    }

    PRINTF("\r\n============================\r\n");
    PRINTF("RESULT: ");
    if (fail == 0U)
    {
        PRINTF("All %d test PASS!!\r\n", pass);
    }
    else
    {
        PRINTF("%d / %d test PASSED, %d FAILED!!\r\n", pass, pass + fail, fail);
    }

    PRINTF("ELS PKC PQC example END \r\n");
    while (1)
    {
        ch = GETCHAR();
        PUTCHAR(ch);
    }
}
