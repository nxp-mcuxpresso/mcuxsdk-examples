/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "els_pkc_fips_kdf.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
static mcuxClEls_KeyProp_t s_DerivedKeyProperties = {
    .word = {.value = MCUXCLELS_KEYPROPERTY_VALUE_SECURE | MCUXCLELS_KEYPROPERTY_VALUE_PRIVILEGED |
                      MCUXCLELS_KEYPROPERTY_VALUE_KEY_SIZE_256 | MCUXCLELS_KEYPROPERTY_VALUE_ACTIVE |
                      MCUXCLELS_KEYPROPERTY_VALUE_AES}};

static mcuxClEls_KeyProp_t s_PlainKeyPropertiesHkdf = {
    .word = {.value = MCUXCLELS_KEYPROPERTY_VALUE_SECURE | MCUXCLELS_KEYPROPERTY_VALUE_PRIVILEGED |
                      MCUXCLELS_KEYPROPERTY_VALUE_KEY_SIZE_256 | MCUXCLELS_KEYPROPERTY_VALUE_HKDF}};

/* Ckdf SP800108 variables */
static uint8_t s_PlainKeyCkdfSp800108[32U] __attribute__((__aligned__(4))) = {
    0x9CU, 0xF4U, 0x83U, 0x16U, 0xE4U, 0xEEU, 0x94U, 0x0FU, 0x75U, 0xA0U, 0x8BU, 0xA6U, 0xE2U, 0xEFU, 0x58U, 0xA6U,
    0x4AU, 0x6FU, 0xD9U, 0xD9U, 0x15U, 0x2AU, 0x77U, 0x04U, 0xCCU, 0x73U, 0x43U, 0x68U, 0x07U, 0x03U, 0x1DU, 0x65U};

static mcuxClEls_KeyProp_t s_PlainKeyPropertiesCkdfSp800108 = {
    .word = {.value = MCUXCLELS_KEYPROPERTY_VALUE_SECURE | MCUXCLELS_KEYPROPERTY_VALUE_PRIVILEGED |
                      MCUXCLELS_KEYPROPERTY_VALUE_KEY_SIZE_256 | MCUXCLELS_KEYPROPERTY_VALUE_CKDF}};

static uint8_t s_DerivationDataCkdfSp800108[12U] __attribute__((__aligned__(4))) = {
    0xC8U, 0xACU, 0x48U, 0x88U, 0xA6U, 0x1BU, 0x3DU, 0x9BU, 0x56U, 0xA9U, 0x75U, 0xE7U,
};

static uint8_t s_AesInputCkdfSp800108[16U] __attribute__((__aligned__(4))) = {
    0x35U, 0xE6U, 0x2CU, 0x18U, 0x02U, 0xCAU, 0x06U, 0x5BU, 0xCDU, 0x56U, 0x1EU, 0xBFU, 0x9BU, 0xF0U, 0x2DU, 0x00U};

static uint8_t s_DerivedKeyKATCkdfSp800108[32U] __attribute__((__aligned__(4))) = {
    0xF8U, 0x84U, 0x40U, 0x89U, 0xE9U, 0x47U, 0xADU, 0x6BU, 0xACU, 0x71U, 0x2FU, 0x35U, 0x39U, 0x87U, 0xCAU, 0x4BU,
    0x9AU, 0x1DU, 0x6BU, 0xCAU, 0xCFU, 0xE2U, 0xCAU, 0xF5U, 0x1CU, 0x55U, 0xFDU, 0x56U, 0xCDU, 0x83U, 0xC3U, 0xF4U};

/* Hkdf RFC5869 variables */
static uint8_t s_PlainKeyHkdfRfc5869[32U] = {
    0x4AU, 0xDFU, 0x2DU, 0xD0U, 0x0CU, 0x88U, 0xC7U, 0x27U, 0x89U, 0x00U, 0x80U, 0xC8U, 0x65U, 0x8AU, 0x26U, 0x54U,
    0xEEU, 0x72U, 0x57U, 0x7BU, 0x51U, 0x42U, 0xCEU, 0xE7U, 0x54U, 0x9AU, 0x67U, 0xB2U, 0x96U, 0x63U, 0x4CU, 0x68U};

static uint8_t s_AES256KATOutputHkdfRfc5869[16U] = {0x94U, 0x21U, 0x64U, 0x1CU, 0xA4U, 0x9DU, 0xEBU, 0x10U,
                                                    0xFBU, 0xDAU, 0x1AU, 0x15U, 0x64U, 0xABU, 0xA4U, 0x61U};

static uint8_t s_AES256InputHkdfRfc5869[16U] = {0x6BU, 0xC1U, 0xBEU, 0xE2U, 0x2EU, 0x40U, 0x9FU, 0x96U,
                                                0xE9U, 0x3DU, 0x7EU, 0x11U, 0x73U, 0x93U, 0x17U, 0x2AU};

static uint8_t s_DerivationDataHkdfRfc5869[32U] = {0U};

/* Hkdf SP80056C variables */
static uint8_t s_PlainKeyHkdfSp80056c[32U] = {
    0x95U, 0x37U, 0xA2U, 0xE8U, 0xA3U, 0x38U, 0x27U, 0xC2U, 0xDEU, 0xF4U, 0xEAU, 0x61U, 0x67U, 0xABU, 0x36U, 0xD4U,
    0x68U, 0x3DU, 0x04U, 0x8BU, 0xD3U, 0x44U, 0x7EU, 0x52U, 0x70U, 0x60U, 0x2BU, 0x4AU, 0x52U, 0xC8U, 0xBFU, 0x0EU};

static uint8_t s_DerivationDataHkdfSp80056c[32U] = {
    0x00U, 0x00U, 0x00U, 0x01U, 0x66U, 0x2AU, 0xF2U, 0x03U, 0x79U, 0xB2U, 0x9DU, 0x5EU, 0xF8U, 0x13U, 0xE6U, 0x55U,
    0x80U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x01U, 0x80U};

static uint8_t s_DerivedKeyKatHkdfSp80056c[32U] = {
    0xF1U, 0x97U, 0xF8U, 0xD2U, 0xB5U, 0xF2U, 0x8AU, 0x87U, 0xF1U, 0x4CU, 0xEEU, 0x67U, 0x00U, 0xAEU, 0xB6U, 0x1EU,
    0x49U, 0x4FU, 0xD0U, 0x9EU, 0xCDU, 0x21U, 0x0EU, 0xB3U, 0xE6U, 0xD0U, 0xC8U, 0xD0U, 0x89U, 0x65U, 0xCDU, 0x86U};
/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Execute Ckdf SP800-108.
 */
static status_t ckdf_sp800108(void)
{
    mcuxClEls_KeyIndex_t key_index = MCUXCLELS_KEY_SLOTS;

    uint8_t aes256_kat_output[16U] = {0U};

    mcuxClEls_CipherOption_t cipher_options = {0U};
    cipher_options.bits.cphmde              = MCUXCLELS_CIPHERPARAM_ALGORITHM_AES_ECB;
    cipher_options.bits.dcrpt               = MCUXCLELS_CIPHER_ENCRYPT;
    cipher_options.bits.extkey              = MCUXCLELS_CIPHER_EXTERNAL_KEY;

    /* Execute cipher operation with KAT key */
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(
        result, token,
        mcuxClEls_Cipher_Async(cipher_options, (mcuxClEls_KeyIndex_t)0U, s_DerivedKeyKATCkdfSp800108,
                               MCUXCLELS_CIPHER_KEY_SIZE_AES_256, s_AesInputCkdfSp800108,
                               sizeof(s_AesInputCkdfSp800108), NULL, aes256_kat_output));

    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_Cipher_Async) != token) || (MCUXCLELS_STATUS_OK_WAIT != result))
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

    /* Import plain key into ELS keystore */
    if (import_plain_key_into_els(s_PlainKeyCkdfSp800108, sizeof(s_PlainKeyCkdfSp800108),
                                  s_PlainKeyPropertiesCkdfSp800108, &key_index) != STATUS_SUCCESS)
    {
        return STATUS_ERROR_GENERIC;
    }

    uint32_t key_index_derived = els_get_free_keyslot(2U);
    if (els_delete_key(key_index_derived) != STATUS_SUCCESS)
    {
        (void)els_delete_key(key_index);
        return STATUS_ERROR_GENERIC;
    }

    /* Execute CKDF with ELS */
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(resultCkdf, tokenAsync,
                                     mcuxClEls_Ckdf_Sp800108_Async(key_index, key_index_derived, s_DerivedKeyProperties,
                                                                   s_DerivationDataCkdfSp800108));

    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_Ckdf_Sp800108_Async) != tokenAsync) ||
        (MCUXCLELS_STATUS_OK_WAIT != resultCkdf))
    {
        (void)els_delete_key(key_index);
        return STATUS_ERROR_GENERIC;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(resultWait, tokenWait, mcuxClEls_WaitForOperation(MCUXCLELS_ERROR_FLAGS_CLEAR));
    if (MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_WaitForOperation) != tokenWait)
    {
        return STATUS_ERROR_GENERIC;
    }
    if (MCUXCLELS_STATUS_OK != resultWait)
    {
        (void)els_delete_key(key_index);
        return STATUS_ERROR_GENERIC;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    uint8_t aes256_output[16U] = {0U};

    mcuxClEls_CipherOption_t cipher_options_els = {0U};
    cipher_options_els.bits.cphmde              = MCUXCLELS_CIPHERPARAM_ALGORITHM_AES_ECB;
    cipher_options_els.bits.dcrpt               = MCUXCLELS_CIPHER_ENCRYPT;
    cipher_options_els.bits.extkey              = MCUXCLELS_CIPHER_INTERNAL_KEY;

    /* Execute ECB cipher operation with the generated CKDF key */
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(
        result, token,
        mcuxClEls_Cipher_Async(cipher_options_els, (mcuxClEls_KeyIndex_t)key_index_derived, NULL,
                               MCUXCLELS_CIPHER_KEY_SIZE_AES_256, s_AesInputCkdfSp800108,
                               sizeof(s_AesInputCkdfSp800108), NULL, aes256_output));

    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_Cipher_Async) != token) || (MCUXCLELS_STATUS_OK_WAIT != result))
    {
        (void)els_delete_key(key_index_derived);
        (void)els_delete_key(key_index);
        return STATUS_ERROR_GENERIC;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClEls_WaitForOperation(MCUXCLELS_ERROR_FLAGS_CLEAR));
    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_WaitForOperation) != token) || (MCUXCLELS_STATUS_OK != result))
    {
        (void)els_delete_key(key_index_derived);
        (void)els_delete_key(key_index);
        return STATUS_ERROR_GENERIC;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    if (!assert_equal(aes256_kat_output, aes256_output, 16U))
    {
        (void)els_delete_key(key_index_derived);
        (void)els_delete_key(key_index);
        return STATUS_ERROR_GENERIC;
    }

    if (els_delete_key(key_index_derived) != STATUS_SUCCESS)
    {
        (void)els_delete_key(key_index);
        return STATUS_ERROR_GENERIC;
    }
    if (els_delete_key(key_index) != STATUS_SUCCESS)
    {
        return STATUS_ERROR_GENERIC;
    }
    return STATUS_SUCCESS;
}

/*!
 * @brief Execute Hkdf rfc5869.
 */
static status_t hkdf_rfc5869(void)
{
    mcuxClEls_KeyIndex_t key_index = MCUXCLELS_KEY_SLOTS;

    /* Import plain key into ELS keystore */
    if (import_plain_key_into_els(s_PlainKeyHkdfRfc5869, sizeof(s_PlainKeyHkdfRfc5869), s_PlainKeyPropertiesHkdf,
                                  &key_index) != STATUS_SUCCESS)
    {
        return STATUS_ERROR_GENERIC;
    }

    uint32_t key_index_derived = els_get_free_keyslot(2U);
    if (els_delete_key(key_index_derived) != STATUS_SUCCESS)
    {
        (void)els_delete_key(key_index);
        return STATUS_ERROR_GENERIC;
    }

    mcuxClEls_HkdfOption_t options;
    options.bits.rtfdrvdat = 0U;
    options.bits.hkdf_algo = (uint32_t)MCUXCLELS_HKDF_ALGO_RFC5869;

    /* Execute HKDF operation */
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(resultCkdf, tokenAsync,
                                     mcuxClEls_Hkdf_Rfc5869_Async(options, key_index, key_index_derived,
                                                                  s_DerivedKeyProperties, s_DerivationDataHkdfRfc5869));
    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_Hkdf_Rfc5869_Async) != tokenAsync) ||
        (MCUXCLELS_STATUS_OK_WAIT != resultCkdf))
    {
        (void)els_delete_key(key_index);
        return STATUS_ERROR_GENERIC;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(resultWait, tokenWait, mcuxClEls_WaitForOperation(MCUXCLELS_ERROR_FLAGS_CLEAR));
    if (MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_WaitForOperation) != tokenWait)
    {
        (void)els_delete_key(key_index);
        return STATUS_ERROR_GENERIC;
    }
    if (MCUXCLELS_STATUS_OK != resultWait)
    {
        (void)els_delete_key(key_index);
        return STATUS_ERROR_GENERIC;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    uint8_t aes256_output[16U] = {0U};

    mcuxClEls_CipherOption_t cipher_options_els = {0U};
    cipher_options_els.bits.cphmde              = MCUXCLELS_CIPHERPARAM_ALGORITHM_AES_ECB;
    cipher_options_els.bits.dcrpt               = MCUXCLELS_CIPHER_ENCRYPT;
    cipher_options_els.bits.extkey              = MCUXCLELS_CIPHER_INTERNAL_KEY;

    /* Execute Cipher operation */
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(
        result, token,
        mcuxClEls_Cipher_Async(cipher_options_els, (mcuxClEls_KeyIndex_t)key_index_derived, NULL,
                               MCUXCLELS_CIPHER_KEY_SIZE_AES_256, s_AES256InputHkdfRfc5869,
                               sizeof(s_AES256InputHkdfRfc5869), NULL, aes256_output));

    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_Cipher_Async) != token) || (MCUXCLELS_STATUS_OK_WAIT != result))
    {
        (void)els_delete_key(key_index_derived);
        (void)els_delete_key(key_index);
        return STATUS_ERROR_GENERIC;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClEls_WaitForOperation(MCUXCLELS_ERROR_FLAGS_CLEAR));
    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_WaitForOperation) != token) || (MCUXCLELS_STATUS_OK != result))
    {
        (void)els_delete_key(key_index_derived);
        (void)els_delete_key(key_index);
        return STATUS_ERROR_GENERIC;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    if (!assert_equal(s_AES256KATOutputHkdfRfc5869, aes256_output, 16U))
    {
        (void)els_delete_key(key_index_derived);
        (void)els_delete_key(key_index);
        return STATUS_ERROR_GENERIC;
    }

    if (els_delete_key(key_index_derived) != STATUS_SUCCESS)
    {
        (void)els_delete_key(key_index);
        return STATUS_ERROR_GENERIC;
    }
    if (els_delete_key(key_index) != STATUS_SUCCESS)
    {
        return STATUS_ERROR_GENERIC;
    }
    return STATUS_SUCCESS;
}

/*!
 * @brief Execute Hkdf SP800-56C.
 */
static status_t hkdf_sp80056c(void)
{
    mcuxClEls_KeyIndex_t key_index = MCUXCLELS_KEY_SLOTS;

    uint8_t derived_key[32U] = {0U};

    /* Import plain key into ELS keystore */
    if (import_plain_key_into_els(s_PlainKeyHkdfSp80056c, sizeof(s_PlainKeyHkdfSp80056c), s_PlainKeyPropertiesHkdf,
                                  &key_index) != STATUS_SUCCESS)
    {
        return STATUS_ERROR_GENERIC;
    }

    /* Execute HKDF operation */
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(resultCkdf, tokenAsync,
                                     mcuxClEls_Hkdf_Sp80056c_Async(key_index, derived_key, s_DerivationDataHkdfSp80056c,
                                                                   sizeof(s_DerivationDataHkdfSp80056c)));
    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_Hkdf_Sp80056c_Async) != tokenAsync) ||
        (MCUXCLELS_STATUS_OK_WAIT != resultCkdf))
    {
        (void)els_delete_key(key_index);
        return STATUS_ERROR_GENERIC;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(resultWait, tokenWait, mcuxClEls_WaitForOperation(MCUXCLELS_ERROR_FLAGS_CLEAR));
    if (MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_WaitForOperation) != tokenWait)
    {
        (void)els_delete_key(key_index);
        return STATUS_ERROR_GENERIC;
    }
    if (MCUXCLELS_STATUS_OK != resultWait)
    {
        (void)els_delete_key(key_index);
        return STATUS_ERROR_GENERIC;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    if (!assert_equal(s_DerivedKeyKatHkdfSp80056c, derived_key, 32U))
    {
        (void)els_delete_key(key_index);
        return STATUS_ERROR_GENERIC;
    }

    if (els_delete_key(key_index) != STATUS_SUCCESS)
    {
        return STATUS_ERROR_GENERIC;
    }
    return STATUS_SUCCESS;
}

void execute_kdf_kat(uint64_t options, char name[])
{
    if ((bool)(options & FIPS_CKDF_SP800108))
    {
        CHECK_STATUS_AND_LOG(ckdf_sp800108(), name, "KAT");
    }
    if ((bool)(options & FIPS_HKDF_RFC5869))
    {
        CHECK_STATUS_AND_LOG(hkdf_rfc5869(), name, "KAT");
    }
    if ((bool)(options & FIPS_HKDF_SP80056C))
    {
        CHECK_STATUS_AND_LOG(hkdf_sp80056c(), name, "KAT");
    }
}
