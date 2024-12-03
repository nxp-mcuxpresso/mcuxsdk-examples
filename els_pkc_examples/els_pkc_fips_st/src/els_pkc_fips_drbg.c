/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "els_pkc_fips_drbg.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* ECB-DRBG variables */
static uint8_t s_EntropyECBDrbg[32U] = {0x0AU, 0x0AU, 0x0AU, 0x0AU, 0x0AU, 0x0AU, 0x0AU, 0x0AU, 0x0AU, 0x0AU, 0x0AU,
                                        0x0AU, 0x0AU, 0x0AU, 0x0AU, 0x0AU, 0x0AU, 0x0AU, 0x0AU, 0x0AU, 0x0AU, 0x0AU,
                                        0x0AU, 0x0AU, 0x0AU, 0x0AU, 0x0AU, 0x0AU, 0x0AU, 0x0AU, 0x0AU, 0x0AU};

static uint8_t s_DataKeyECBDrbg[32U] = {0x08U, 0x4BU, 0x35U, 0x2FU, 0x38U, 0xABU, 0x28U, 0xD9U, 0xC1U, 0xC7U, 0xFFU,
                                        0x16U, 0x55U, 0x8EU, 0x0AU, 0x12U, 0x37U, 0x7DU, 0x82U, 0x0CU, 0xD6U, 0xECU,
                                        0xA3U, 0xA3U, 0x52U, 0xA6U, 0xFEU, 0xC3U, 0x81U, 0xF3U, 0x58U, 0x44U};

static uint8_t s_OutputKATECBDrbg[16U] = {0x28U, 0x9FU, 0x1EU, 0x7AU, 0x01U, 0x0CU, 0x84U, 0x71U,
                                          0xEBU, 0xEEU, 0x52U, 0xDFU, 0xAAU, 0x17U, 0xFEU, 0x03U};

/* CTR-DRBG variables */
static uint8_t s_EntropyCTRDrbg[32U] = {0x8CU, 0x25U, 0x66U, 0xACU, 0x86U, 0x0FU, 0x23U, 0x2FU, 0xA3U, 0x17U, 0x08U,
                                        0x7FU, 0x84U, 0xF0U, 0x17U, 0x6FU, 0x98U, 0xF5U, 0x7EU, 0xC0U, 0x87U, 0xAEU,
                                        0x8CU, 0xAEU, 0x97U, 0x52U, 0x7BU, 0xEAU, 0x92U, 0x24U, 0xABU, 0xF1U};

static uint8_t s_DataCTRDrbg[16U] = {0xD4U, 0xC7U, 0x9AU, 0x62U, 0x7CU, 0x71U, 0x13U, 0x4EU,
                                     0x95U, 0x68U, 0x15U, 0x28U, 0x20U, 0x17U, 0x52U, 0x35U};

static uint8_t s_IvKeyCTRDrbg[32U] = {0x6EU, 0xB0U, 0x55U, 0xA3U, 0x30U, 0xF2U, 0xC1U, 0x81U, 0x77U, 0x48U, 0x18U,
                                      0x5DU, 0x5CU, 0xF1U, 0x18U, 0x62U, 0xE2U, 0x7AU, 0x17U, 0x3CU, 0x7CU, 0xAAU,
                                      0x98U, 0x31U, 0x28U, 0xACU, 0x7DU, 0x35U, 0x6DU, 0x39U, 0xF4U, 0x40U};

static uint8_t s_OutputKATCTRDrbg[16U] = {0x81U, 0x6DU, 0xE2U, 0x59U, 0xAFU, 0xF3U, 0x0FU, 0x71U,
                                          0x65U, 0x7EU, 0x1EU, 0x34U, 0x5FU, 0xDCU, 0xDFU, 0xFAU};
/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Execute ECB_DRBG.
 */
static status_t drbg_aes_ecb(void)
{
    /* Instantiate DRBG with the entropy above */
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClEls_Rng_DrbgTestInstantiate_Async(s_EntropyECBDrbg));
    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_Rng_DrbgTestInstantiate_Async) != token) ||
        (MCUXCLELS_STATUS_OK_WAIT != result))
    {
        return STATUS_ERROR_GENERIC;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClEls_WaitForOperation(MCUXCLELS_ERROR_FLAGS_CLEAR));
    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_WaitForOperation) != token) || (MCUXCLELS_STATUS_OK != result))
    {
        return STATUS_ERROR_GENERIC;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClEls_WaitForOperation(MCUXCLELS_ERROR_FLAGS_CLEAR));
    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_WaitForOperation) != token) || (MCUXCLELS_STATUS_OK != result))
    {
        return STATUS_ERROR_GENERIC;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    uint8_t ecb_output[16U] = {0U};

    /* Execute ECB_DRBG */
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClEls_Rng_DrbgTestAesEcb_Async(s_DataKeyECBDrbg, ecb_output));

    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_Rng_DrbgTestAesEcb_Async) != token) ||
        (MCUXCLELS_STATUS_OK_WAIT != result))
    {
        return STATUS_ERROR_GENERIC;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClEls_WaitForOperation(MCUXCLELS_ERROR_FLAGS_CLEAR));
    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_WaitForOperation) != token) || (MCUXCLELS_STATUS_OK != result))
    {
        return STATUS_ERROR_GENERIC;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    /* Compare results */
    if (!assert_equal(ecb_output, s_OutputKATECBDrbg, sizeof(ecb_output)))
    {
        return STATUS_ERROR_GENERIC;
    }

    return STATUS_SUCCESS;
}

/*!
 * @brief Execute CTR_DRBG.
 */
static status_t drbg_aes_ctr(void)
{
    /* Instantiate DRBG with the entropy above */
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClEls_Rng_DrbgTestInstantiate_Async(s_EntropyCTRDrbg));
    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_Rng_DrbgTestInstantiate_Async) != token) ||
        (MCUXCLELS_STATUS_OK_WAIT != result))
    {
        return STATUS_ERROR_GENERIC;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClEls_WaitForOperation(MCUXCLELS_ERROR_FLAGS_CLEAR));
    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_WaitForOperation) != token) || (MCUXCLELS_STATUS_OK != result))
    {
        return STATUS_ERROR_GENERIC;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    uint8_t ctr_output[16U] = {0U};

    /* Execute CTR_DRBG */
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(
        result, token,
        mcuxClEls_Rng_DrbgTestAesCtr_Async(s_DataCTRDrbg, sizeof(s_DataCTRDrbg), s_IvKeyCTRDrbg, ctr_output));

    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_Rng_DrbgTestAesCtr_Async) != token) ||
        (MCUXCLELS_STATUS_OK_WAIT != result))
    {
        return STATUS_ERROR_GENERIC;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClEls_WaitForOperation(MCUXCLELS_ERROR_FLAGS_CLEAR));
    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_WaitForOperation) != token) || (MCUXCLELS_STATUS_OK != result))
    {
        return STATUS_ERROR_GENERIC;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    /* Compare results */
    if (!assert_equal(ctr_output, s_OutputKATCTRDrbg, sizeof(ctr_output)))
    {
        return STATUS_ERROR_GENERIC;
    }

    return STATUS_SUCCESS;
}

void execute_drbg_kat(uint64_t options, char name[])
{
    /* Execute ECB_DRBG */
    if ((bool)(options & FIPS_ECB_DRBG))
    {
        CHECK_STATUS_AND_LOG(drbg_aes_ecb(), name, "KAT");
    }
    /* Execute CTR_DRBG */
    if ((bool)(options & FIPS_CTR_DRBG))
    {
        CHECK_STATUS_AND_LOG(drbg_aes_ctr(), name, "KAT");
    }
}
