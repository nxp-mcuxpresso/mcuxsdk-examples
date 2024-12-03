/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "app.h"
#include "els_pkc_fips_config.h"
#include <mcux_els.h>
#include <mcux_pkc.h>
#if defined(FSL_FEATURE_SOC_TRNG_COUNT) && (FSL_FEATURE_SOC_TRNG_COUNT > 0U)
#include "fsl_trng.h"
#endif

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
 * @brief Initialize crypto hardware
 */
static void CRYPTO_InitHardware(void)
{
    status_t status_els;
    status_t status_pkc;

    status_els = ELS_PowerDownWakeupInit(ELS);

    /* Enable PKC related clocks and RAM zeroize */
    status_pkc = PKC_PowerDownWakeupInit(PKC);

    if (status_els != kStatus_Success || status_pkc != kStatus_Success)
    {
        return;
    }

#if defined(FSL_FEATURE_SOC_TRNG_COUNT) && (FSL_FEATURE_SOC_TRNG_COUNT > 0U)
    /* Initialize the TRNG driver */
    {
        trng_config_t trng_config;
        /* Get default TRNG configs*/
        TRNG_GetDefaultConfig(&trng_config);
        /* Set sample mode of the TRNG ring oscillator to Von Neumann, for better random data.*/
        /* Initialize TRNG */
        TRNG_Init(TRNG, &trng_config);
    }
#endif /* FSL_FEATURE_SOC_TRNG_COUNT */
}

/*!
 * @brief Function to execute all KATs based on the user options
 * defined in the els_pkc_fips_config.h file.
 */
static void execute_kats(void)
{
    /* Execute all FIPS Self Tests if user has specified */
    if ((bool)(s_UserOptions & FIPS_ALL_TESTS))
    {
        for (uint32_t i = 0U; i < sizeof(s_AlgorithmMappings) / sizeof(s_AlgorithmMappings[0U]); ++i)
        {
            PRINTF("EXECUTING %s SELF-TEST\r\n", s_AlgorithmMappings[i].name);
            s_AlgorithmMappings[i].executionFunction(s_AlgorithmMappings[i].option, s_AlgorithmMappings[i].name);
        }
        PRINTF("\r\n");
        return;
    }
    for (uint32_t i = 0U; i < sizeof(s_AlgorithmMappings) / sizeof(s_AlgorithmMappings[0U]); ++i)
    {
        if ((bool)(s_UserOptions & s_AlgorithmMappings[i].option))
        {
            PRINTF("EXECUTING %s SELF-TEST\r\n", s_AlgorithmMappings[i].name);
            s_AlgorithmMappings[i].executionFunction(s_AlgorithmMappings[i].option, s_AlgorithmMappings[i].name);
        }
    }
    PRINTF("\r\n");
}

/*!
 * @brief Print algorithm information, if HW acclereated or
 * only software implemenatation.
 */
static void print_algorithm_infos(void)
{
    PRINTF("HW ACCELERATION ALGORITHM INFORMATION:\r\n");
    PRINTF("    ECB DRBG: ELS\r\n");
    PRINTF("    CTR DRBG: ELS\r\n");
    PRINTF("    CKDF SP800-108: ELS\r\n");
    PRINTF("    HKDF RFC5869: ELS\r\n");
    PRINTF("    ECDSA NISTP 256: ELS\r\n");
    PRINTF("    ECDSA NISTP 384: PKC\r\n");
    PRINTF("    ECDSA NISTP 521: PKC\r\n");
    PRINTF("    EDDSA ED25519: PKC\r\n");
    PRINTF("    ECDH NISTP 256: ELS\r\n");
    PRINTF("    ECDH NISTP 384: PKC\r\n");
    PRINTF("    ECDH NISTP 521: PKC\r\n");
    PRINTF("    ECC KEYGEN NISTP 256: ELS\r\n");
    PRINTF("    ECC KEYGEN NISTP 384: PKC\r\n");
    PRINTF("    ECC KEYGEN NISTP 521: PKC\r\n");
    PRINTF("    RSA-PKCS15-2048: PKC\r\n");
    PRINTF("    RSA-PKCS15-3072: PKC\r\n");
    PRINTF("    RSA-PKCS15-4096: PKC\r\n");
    PRINTF("    RSA-PSS-2048: PKC\r\n");
    PRINTF("    RSA-PSS-3072: PKC\r\n");
    PRINTF("    RSA-PSS-4096: PKC\r\n");
    PRINTF("    AES-CCM-128: ELS\r\n");
    PRINTF("    AES-CCM-256: ELS\r\n");
    PRINTF("    AES-GCM-128: ELS\r\n");
    PRINTF("    AES-GCM-192: ELS\r\n");
    PRINTF("    AES-GCM-256: ELS\r\n");
    PRINTF("    AES-CTR-128: ELS\r\n");
    PRINTF("    AES-CTR-192: ELS\r\n");
    PRINTF("    AES-CTR-256: ELS\r\n");
    PRINTF("    AES-ECB-128: ELS\r\n");
    PRINTF("    AES-ECB-192: ELS\r\n");
    PRINTF("    AES-ECB-256: ELS\r\n");
    PRINTF("    AES-CBC-128: ELS\r\n");
    PRINTF("    AES-CBC-192: ELS\r\n");
    PRINTF("    AES-CBC-256: ELS\r\n");
    PRINTF("    AES-CMAC-128: ELS\r\n");
    PRINTF("    AES-CMAC-256: ELS\r\n");
    PRINTF("    HMAC-SHA224: SOFTWARE IMPLEMENTATION\r\n");
    PRINTF("    HMAC-SHA256: ELS\r\n");
    PRINTF("    HMAC-SHA384: SOFTWARE IMPLEMENTATION\r\n");
    PRINTF("    HMAC-SHA512: SOFTWARE IMPLEMENTATION\r\n");
    PRINTF("    SHA224: ELS\r\n");
    PRINTF("    SHA256: ELS\r\n");
    PRINTF("    SHA384: ELS\r\n");
    PRINTF("    SHA512: ELS\r\n");

    PRINTF("\r\n");
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Init hardware */
    BOARD_InitHardware();
    CRYPTO_InitHardware();

    PRINTF("START OF ELS PKC FIPS SELF-TEST...\r\n");

    /* Print information regarding each algorithm,
     * whether it is HW accelerated or not.
     */
    print_algorithm_infos();

    /* Enable the ELS */
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClEls_Enable_Async());
    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_Enable_Async) != token) || (MCUXCLELS_STATUS_OK_WAIT != result))
    {
        PRINTF("ERROR AT ELS INIT\r\n");
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClEls_WaitForOperation(MCUXCLELS_ERROR_FLAGS_CLEAR));
    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_WaitForOperation) != token) || (MCUXCLELS_STATUS_OK != result))
    {
        PRINTF("ERROR AT ELS INIT\r\n");
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    execute_kats();

    /* Disable the ELS */
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClEls_Disable());
    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_Disable) != token) || (MCUXCLELS_STATUS_OK != result))
    {
        PRINTF("ERROR AT ELS DE-INIT\r\n");
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    PRINTF("END OF ELS PKC FIPS SELF-TEST\r\n");

    while (true)
    {
    }
}
