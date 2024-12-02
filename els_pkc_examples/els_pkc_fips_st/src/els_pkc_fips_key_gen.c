/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "els_pkc_fips_key_gen.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* Domain parameters for ECC-Weier */
static uint8_t s_BN_P384_P[48U] = {0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU,
                                   0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU,
                                   0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFEU, 0xFFU, 0xFFU, 0xFFU, 0xFFU,
                                   0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0xFFU, 0xFFU, 0xFFU, 0xFFU};

static uint8_t s_BN_P384_A[48U] = {0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU,
                                   0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU,
                                   0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFEU, 0xFFU, 0xFFU, 0xFFU, 0xFFU,
                                   0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0xFFU, 0xFFU, 0xFFU, 0xFCU};

static uint8_t s_BN_P384_B[48U] = {0xB3U, 0x31U, 0x2FU, 0xA7U, 0xE2U, 0x3EU, 0xE7U, 0xE4U, 0x98U, 0x8EU, 0x05U, 0x6BU,
                                   0xE3U, 0xF8U, 0x2DU, 0x19U, 0x18U, 0x1DU, 0x9CU, 0x6EU, 0xFEU, 0x81U, 0x41U, 0x12U,
                                   0x03U, 0x14U, 0x08U, 0x8FU, 0x50U, 0x13U, 0x87U, 0x5AU, 0xC6U, 0x56U, 0x39U, 0x8DU,
                                   0x8AU, 0x2EU, 0xD1U, 0x9DU, 0x2AU, 0x85U, 0xC8U, 0xEDU, 0xD3U, 0xECU, 0x2AU, 0xEFU};

static uint8_t s_BN_P384_G[96U] = {
    0xAAU, 0x87U, 0xCAU, 0x22U, 0xBEU, 0x8BU, 0x05U, 0x37U, 0x8EU, 0xB1U, 0xC7U, 0x1EU, 0xF3U, 0x20U, 0xADU, 0x74U,
    0x6EU, 0x1DU, 0x3BU, 0x62U, 0x8BU, 0xA7U, 0x9BU, 0x98U, 0x59U, 0xF7U, 0x41U, 0xE0U, 0x82U, 0x54U, 0x2AU, 0x38U,
    0x55U, 0x02U, 0xF2U, 0x5DU, 0xBFU, 0x55U, 0x29U, 0x6CU, 0x3AU, 0x54U, 0x5EU, 0x38U, 0x72U, 0x76U, 0x0AU, 0xB7U,
    0x36U, 0x17U, 0xDEU, 0x4AU, 0x96U, 0x26U, 0x2CU, 0x6FU, 0x5DU, 0x9EU, 0x98U, 0xBFU, 0x92U, 0x92U, 0xDCU, 0x29U,
    0xF8U, 0xF4U, 0x1DU, 0xBDU, 0x28U, 0x9AU, 0x14U, 0x7CU, 0xE9U, 0xDAU, 0x31U, 0x13U, 0xB5U, 0xF0U, 0xB8U, 0xC0U,
    0x0AU, 0x60U, 0xB1U, 0xCEU, 0x1DU, 0x7EU, 0x81U, 0x9DU, 0x7AU, 0x43U, 0x1DU, 0x7CU, 0x90U, 0xEAU, 0x0EU, 0x5FU};

static uint8_t s_BN_P384_N[48U] = {0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU,
                                   0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU,
                                   0xC7U, 0x63U, 0x4DU, 0x81U, 0xF4U, 0x37U, 0x2DU, 0xDFU, 0x58U, 0x1AU, 0x0DU, 0xB2U,
                                   0x48U, 0xB0U, 0xA7U, 0x7AU, 0xECU, 0xECU, 0x19U, 0x6AU, 0xCCU, 0xC5U, 0x29U, 0x73U};

static uint8_t s_BN_P521_P[66U] = {0x01U, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU,
                                   0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU,
                                   0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU,
                                   0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU,
                                   0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU,
                                   0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU};

static uint8_t s_BN_P521_A[66U] = {0x01U, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU,
                                   0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU,
                                   0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU,
                                   0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU,
                                   0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU,
                                   0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFCU};

static uint8_t s_BN_P521_B[66U] = {0x00U, 0x51U, 0x95U, 0x3EU, 0xB9U, 0x61U, 0x8EU, 0x1CU, 0x9AU, 0x1FU, 0x92U,
                                   0x9AU, 0x21U, 0xA0U, 0xB6U, 0x85U, 0x40U, 0xEEU, 0xA2U, 0xDAU, 0x72U, 0x5BU,
                                   0x99U, 0xB3U, 0x15U, 0xF3U, 0xB8U, 0xB4U, 0x89U, 0x91U, 0x8EU, 0xF1U, 0x09U,
                                   0xE1U, 0x56U, 0x19U, 0x39U, 0x51U, 0xECU, 0x7EU, 0x93U, 0x7BU, 0x16U, 0x52U,
                                   0xC0U, 0xBDU, 0x3BU, 0xB1U, 0xBFU, 0x07U, 0x35U, 0x73U, 0xDFU, 0x88U, 0x3DU,
                                   0x2CU, 0x34U, 0xF1U, 0xEFU, 0x45U, 0x1FU, 0xD4U, 0x6BU, 0x50U, 0x3FU, 0x00U};

static uint8_t s_BN_P521_G[2U * 66U] = {
    0x00U, 0xC6U, 0x85U, 0x8EU, 0x06U, 0xB7U, 0x04U, 0x04U, 0xE9U, 0xCDU, 0x9EU, 0x3EU, 0xCBU, 0x66U, 0x23U,
    0x95U, 0xB4U, 0x42U, 0x9CU, 0x64U, 0x81U, 0x39U, 0x05U, 0x3FU, 0xB5U, 0x21U, 0xF8U, 0x28U, 0xAFU, 0x60U,
    0x6BU, 0x4DU, 0x3DU, 0xBAU, 0xA1U, 0x4BU, 0x5EU, 0x77U, 0xEFU, 0xE7U, 0x59U, 0x28U, 0xFEU, 0x1DU, 0xC1U,
    0x27U, 0xA2U, 0xFFU, 0xA8U, 0xDEU, 0x33U, 0x48U, 0xB3U, 0xC1U, 0x85U, 0x6AU, 0x42U, 0x9BU, 0xF9U, 0x7EU,
    0x7EU, 0x31U, 0xC2U, 0xE5U, 0xBDU, 0x66U, 0x01U, 0x18U, 0x39U, 0x29U, 0x6AU, 0x78U, 0x9AU, 0x3BU, 0xC0U,
    0x04U, 0x5CU, 0x8AU, 0x5FU, 0xB4U, 0x2CU, 0x7DU, 0x1BU, 0xD9U, 0x98U, 0xF5U, 0x44U, 0x49U, 0x57U, 0x9BU,
    0x44U, 0x68U, 0x17U, 0xAFU, 0xBDU, 0x17U, 0x27U, 0x3EU, 0x66U, 0x2CU, 0x97U, 0xEEU, 0x72U, 0x99U, 0x5EU,
    0xF4U, 0x26U, 0x40U, 0xC5U, 0x50U, 0xB9U, 0x01U, 0x3FU, 0xADU, 0x07U, 0x61U, 0x35U, 0x3CU, 0x70U, 0x86U,
    0xA2U, 0x72U, 0xC2U, 0x40U, 0x88U, 0xBEU, 0x94U, 0x76U, 0x9FU, 0xD1U, 0x66U, 0x50U};

static uint8_t s_BN_P521_N[66U] = {0x01U, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU,
                                   0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU,
                                   0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU,
                                   0xFAU, 0x51U, 0x86U, 0x87U, 0x83U, 0xBFU, 0x2FU, 0x96U, 0x6BU, 0x7FU, 0xCCU,
                                   0x01U, 0x48U, 0xF7U, 0x09U, 0xA5U, 0xD0U, 0x3BU, 0xB5U, 0xC9U, 0xB8U, 0x89U,
                                   0x9CU, 0x47U, 0xAEU, 0xBBU, 0x6FU, 0xB7U, 0x1EU, 0x91U, 0x38U, 0x64U, 0x09U};

static uint8_t s_DigestWeierKeygen[64U] __attribute__((__aligned__(4))) = {
    0x8DU, 0xE6U, 0xC2U, 0x3DU, 0x6CU, 0xFCU, 0xDEU, 0x8EU, 0x3DU, 0x30U, 0x4FU, 0xEBU, 0x56U, 0x4EU, 0x69U, 0xEFU,
    0x2EU, 0x6CU, 0xD3U, 0xF4U, 0x62U, 0x2AU, 0x6CU, 0xE5U, 0x49U, 0xA8U, 0x84U, 0xFCU, 0x36U, 0xF7U, 0xACU, 0x59U,
    0xE8U, 0xE8U, 0xCBU, 0x96U, 0xC9U, 0xBFU, 0x73U, 0x67U, 0xD2U, 0xB9U, 0x0CU, 0x03U, 0x3CU, 0x30U, 0xA1U, 0xA6U,
    0xB2U, 0x4FU, 0xB3U, 0x79U, 0x80U, 0x2EU, 0xC1U, 0x86U, 0x12U, 0xECU, 0x50U, 0xCCU, 0xACU, 0x20U, 0x48U, 0xECU};

static uint8_t s_DigestELSWeierKeygen[32U] __attribute__((__aligned__(4))) = {
    0x11U, 0x11U, 0x11U, 0x11U, 0x22U, 0x22U, 0x22U, 0x22U, 0x33U, 0x33U, 0x33U, 0x33U, 0x44U, 0x44U, 0x44U, 0x44U,
    0x55U, 0x55U, 0x55U, 0x55U, 0x66U, 0x66U, 0x66U, 0x66U, 0x77U, 0x77U, 0x77U, 0x77U, 0x88U, 0x88U, 0x88U, 0x88U};
/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Execute ECC key generation pairwise consistency test on ELS.
 */
static status_t ecc_weier_key_gen_els(void)
{
    uint8_t public_key[64U] = {0U};

    const mcuxClEls_KeyProp_t keypair_prop = {.bits = {
                                                  .upprot_priv = (uint32_t)MCUXCLELS_KEYPROPERTY_PRIVILEGED_TRUE,
                                                  .upprot_sec  = (uint32_t)MCUXCLELS_KEYPROPERTY_SECURE_TRUE,
                                                  .ksize       = (uint32_t)MCUXCLELS_KEYPROPERTY_KEY_SIZE_256,
                                              }};
    mcuxClEls_EccKeyGenOption_t options    = {0U};
    options.bits.kgsrc                     = MCUXCLELS_ECC_OUTPUTKEY_RANDOM;
    options.bits.kgtypedh                  = MCUXCLELS_ECC_OUTPUTKEY_SIGN;

    mcuxClEls_KeyIndex_t key_index = (mcuxClEls_KeyIndex_t)els_get_free_keyslot(2U);

    if (!(key_index < MCUXCLELS_KEY_SLOTS))
    {
        return STATUS_ERROR_GENERIC;
    }

    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(
        result, token,
        mcuxClEls_EccKeyGen_Async(options, (mcuxClEls_KeyIndex_t)0U, key_index, keypair_prop, NULL, public_key));
    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_EccKeyGen_Async) != token) || (MCUXCLELS_STATUS_OK_WAIT != result))
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

    mcuxClEls_EccSignOption_t sign_options                          = {0U};
    mcuxClEls_EccByte_t ecc_signature[MCUXCLELS_ECC_SIGNATURE_SIZE] = {0U};
    mcuxClEls_EccByte_t ecc_signature_and_public_key[MCUXCLELS_ECC_SIGNATURE_SIZE + MCUXCLELS_ECC_PUBLICKEY_SIZE] = {
        0U};
    mcuxClEls_EccByte_t ecc_signature_r[MCUXCLELS_ECC_SIGNATURE_R_SIZE] = {0U};

    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(
        result, token,
        mcuxClEls_EccSign_Async(sign_options, key_index, s_DigestELSWeierKeygen, NULL, (size_t)0U, ecc_signature));

    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_EccSign_Async) != token) || (MCUXCLELS_STATUS_OK_WAIT != result))
    {
        (void)els_delete_key(key_index);
        return STATUS_ERROR_GENERIC;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClEls_WaitForOperation(MCUXCLELS_ERROR_FLAGS_CLEAR));
    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_WaitForOperation) != token) || (MCUXCLELS_STATUS_OK != result))
    {
        (void)els_delete_key(key_index);
        return STATUS_ERROR_GENERIC;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();
    mcuxClEls_EccVerifyOption_t verify_options = {0U};

    (void)memcpy(&ecc_signature_and_public_key[0U], &ecc_signature[0U], MCUXCLELS_ECC_SIGNATURE_SIZE);
    (void)memcpy(&ecc_signature_and_public_key[MCUXCLELS_ECC_SIGNATURE_SIZE], &public_key[0U],
                 MCUXCLELS_ECC_PUBLICKEY_SIZE);

    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token,
                                     mcuxClEls_EccVerify_Async(verify_options, s_DigestELSWeierKeygen, NULL, 0U,
                                                               ecc_signature_and_public_key, ecc_signature_r));
    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_EccVerify_Async) != token) || (MCUXCLELS_STATUS_OK_WAIT != result))
    {
        (void)els_delete_key(key_index);
        return STATUS_ERROR_GENERIC;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClEls_WaitForOperation(MCUXCLELS_ERROR_FLAGS_CLEAR));
    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_WaitForOperation) != token) || (MCUXCLELS_STATUS_OK != result))
    {
        (void)els_delete_key(key_index);
        return STATUS_ERROR_GENERIC;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    if (els_delete_key(key_index) != STATUS_SUCCESS)
    {
        return STATUS_ERROR_GENERIC;
    }

    return STATUS_SUCCESS;
}

/*!
 * @brief Execute ECC key generation pairwise consistency test.
 */
static status_t ecc_weier_key_gen(uint32_t bit_length)
{
    const uint32_t p_byte_length = (bit_length + 7U) / 8U;
    const uint32_t n_byte_length = (bit_length + 7U) / 8U;

    mcuxClSession_Descriptor_t session_desc;
    mcuxClSession_Handle_t session = &session_desc;
    ALLOCATE_AND_INITIALIZE_SESSION(session, MCUXCLECC_KEYGEN_WACPU_SIZE, MCUXCLECC_KEYGEN_WAPKC_SIZE_512);

    /* Initialize the RNG context, with maximum size */
    uint32_t rng_ctx[MCUXCLRANDOMMODES_CTR_DRBG_AES256_CONTEXT_SIZE_IN_WORDS] = {0U};

    mcuxClRandom_Mode_t random_mode = NULL;

    uint32_t value = (uint32_t)MCUX_PKC_MIN((n_byte_length * 8U) / 2U, 256U);
    if (value <= 128U) /* 128-bit security strength */
    {
        random_mode = mcuxClRandomModes_Mode_ELS_Drbg;
    }
    else /* 256-bit security strength */
    {
        random_mode = mcuxClRandomModes_Mode_CtrDrbg_AES256_DRG3;
    }

    MCUX_CSSL_FP_FUNCTION_CALL_PROTECTED(randomInit_result, randomInit_token,
                                         mcuxClRandom_init(session, (mcuxClRandom_Context_t)rng_ctx, random_mode));
    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandom_init) != randomInit_token) ||
        (MCUXCLRANDOM_STATUS_OK != randomInit_result))
    {
        return STATUS_ERROR_GENERIC;
    }

    /* Default domain paramameters initialization */
    mcuxClEcc_DomainParam_t domain_params =
        (mcuxClEcc_DomainParam_t){.pA = NULL, .pB = NULL, .pG = NULL, .pP = NULL, .pN = NULL, .misc = 0U};

    switch (bit_length)
    {
        case WEIER384_BIT_LENGTH:
            domain_params =
                (mcuxClEcc_DomainParam_t){.pA   = s_BN_P384_A,
                                          .pB   = s_BN_P384_B,
                                          .pG   = s_BN_P384_G,
                                          .pP   = s_BN_P384_P,
                                          .pN   = s_BN_P384_N,
                                          .misc = mcuxClEcc_DomainParam_misc_Pack(n_byte_length, p_byte_length)};
            break;
        case WEIER521_BIT_LENGTH:
            domain_params =
                (mcuxClEcc_DomainParam_t){.pA   = s_BN_P521_A,
                                          .pB   = s_BN_P521_B,
                                          .pG   = s_BN_P521_G,
                                          .pP   = s_BN_P521_P,
                                          .pN   = s_BN_P521_N,
                                          .misc = mcuxClEcc_DomainParam_misc_Pack(n_byte_length, p_byte_length)};
            break;
        default:
            return STATUS_ERROR_GENERIC;
    }

    uint8_t private_key[66U]           = {0U};
    uint8_t public_key[132U]           = {0U};
    mcuxClEcc_KeyGen_Param_t key_param = (mcuxClEcc_KeyGen_Param_t){
        .curveParam = domain_params, .pPrivateKey = private_key, .pPublicKey = public_key, .optLen = 0U};

    /* Execute ECC keygen */
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result_enc, token_enc, mcuxClEcc_KeyGen(session, &key_param));
    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEcc_KeyGen) != token_enc) || (MCUXCLECC_STATUS_OK != result_enc))
    {
        return STATUS_ERROR_GENERIC;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    uint8_t signature_buffer[2U * 66U] = {0U};

    mcuxClEcc_Sign_Param_t sign_parameters;
    mcuxClEcc_ECDSA_SignatureProtocolDescriptor_t sign_mode;
    sign_mode.generateOption = MCUXCLECC_ECDSA_SIGNATURE_GENERATE_RANDOMIZED;

    sign_parameters = (mcuxClEcc_Sign_Param_t){.curveParam  = domain_params,
                                               .pHash       = s_DigestWeierKeygen,
                                               .pPrivateKey = private_key,
                                               .pSignature  = signature_buffer,
                                               .optLen      = mcuxClEcc_Sign_Param_optLen_Pack(64U),
                                               .pMode       = &sign_mode};

    /* Execute ECC sign operation */
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(sign_result, sign_token, mcuxClEcc_Sign(session, &sign_parameters));
    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEcc_Sign) != sign_token))
    {
        return STATUS_ERROR_GENERIC;
    }

    if (MCUXCLECC_STATUS_OK != sign_result)
    {
        return STATUS_ERROR_GENERIC;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    uint8_t scalar_prec_G[66U]                                  = {0U};
    uint32_t scalar_bit_index                                   = 4U * n_byte_length;
    scalar_prec_G[n_byte_length - 1U - (scalar_bit_index / 8U)] = (uint8_t)1U << (scalar_bit_index & 7U);

    uint8_t result[132U]                          = {0U};
    mcuxClEcc_PointMult_Param_t point_mult_params = {.curveParam = domain_params,
                                                     .pScalar    = scalar_prec_G,
                                                     .pPoint     = domain_params.pG,
                                                     .pResult    = result,
                                                     .optLen     = 0U};

    /* Execute ECC point multiplication */
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(ret_ecc_point_mult, token_ecc_point_mult,
                                     mcuxClEcc_PointMult(session, &point_mult_params));

    if (MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEcc_PointMult) != token_ecc_point_mult)
    {
        return STATUS_ERROR_GENERIC;
    }

    if (MCUXCLECC_STATUS_OK != ret_ecc_point_mult)
    {
        return STATUS_ERROR_GENERIC;
    }

    MCUX_CSSL_FP_FUNCTION_CALL_END();
    uint8_t output_R[66U];
    mcuxClEcc_Verify_Param_t param_verify;

    param_verify.curveParam = domain_params;
    param_verify.pPrecG     = result;
    param_verify.pHash      = s_DigestWeierKeygen;
    param_verify.pPublicKey = public_key;
    param_verify.pSignature = signature_buffer;
    param_verify.pOutputR   = output_R;
    param_verify.optLen     = mcuxClEcc_Sign_Param_optLen_Pack(64U);

    /* Execute ECC verify operation */
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(ret_ecc_verify, token_ecc_verify, mcuxClEcc_Verify(session, &param_verify));
    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEcc_Verify) != token_ecc_verify) || (MCUXCLECC_STATUS_OK != ret_ecc_verify))
    {
        return STATUS_ERROR_GENERIC;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    /* Clean-up and destroy the session. */
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(sessionCleanup_result, sessionCleanup_token, mcuxClSession_cleanup(session));
    if (MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClSession_cleanup) != sessionCleanup_token ||
        MCUXCLSESSION_STATUS_OK != sessionCleanup_result)
    {
        return STATUS_ERROR_GENERIC;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(sessionDestroy_result, sessionDestroy_token, mcuxClSession_destroy(session));
    if (MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClSession_destroy) != sessionDestroy_token ||
        MCUXCLSESSION_STATUS_OK != sessionDestroy_result)
    {
        return STATUS_ERROR_GENERIC;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    return STATUS_SUCCESS;
}

void execute_ecc_keygen_pct(uint64_t options, char name[])
{
    /* Execute ECC keygen on 256p curve */
    if ((bool)(options & FIPS_ECC_KEYGEN_256P))
    {
        CHECK_STATUS_AND_LOG(ecc_weier_key_gen_els(), name, "PCT");
    }
    /* Execute ECC keygen on 384p curve */
    if ((bool)(options & FIPS_ECC_KEYGEN_384P))
    {
        CHECK_STATUS_AND_LOG(ecc_weier_key_gen(WEIER384_BIT_LENGTH), name, "PCT");
    }
    /* Execute ECC keygen on 521p curve */
    if ((bool)(options & FIPS_ECC_KEYGEN_521P))
    {
        CHECK_STATUS_AND_LOG(ecc_weier_key_gen(WEIER521_BIT_LENGTH), name, "PCT");
    }
}
