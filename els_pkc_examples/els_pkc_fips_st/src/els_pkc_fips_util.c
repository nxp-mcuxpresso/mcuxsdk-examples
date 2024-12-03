/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "els_pkc_fips_util.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
static const mcuxClEls_KeyProp_t s_KeypairProp = {.bits = {
                                                      .upprot_priv = MCUXCLELS_KEYPROPERTY_PRIVILEGED_TRUE,
                                                      .upprot_sec  = MCUXCLELS_KEYPROPERTY_SECURE_TRUE,
                                                      .ksize       = MCUXCLELS_KEYPROPERTY_KEY_SIZE_256,
                                                  }};

static const mcuxClEls_KeyProp_t s_SharedSecretProp = {
    .bits =
        {
            .upprot_priv = MCUXCLELS_KEYPROPERTY_PRIVILEGED_TRUE,
            .upprot_sec  = MCUXCLELS_KEYPROPERTY_SECURE_TRUE,
            .uckdf       = MCUXCLELS_KEYPROPERTY_CKDF_TRUE,
            .ksize       = MCUXCLELS_KEYPROPERTY_KEY_SIZE_128,
        },
};

static const mcuxClEls_KeyProp_t s_WrapInKeyProp = {
    .bits =
        {
            .upprot_priv = MCUXCLELS_KEYPROPERTY_PRIVILEGED_TRUE,
            .upprot_sec  = MCUXCLELS_KEYPROPERTY_SECURE_TRUE,
            .ukuok       = MCUXCLELS_KEYPROPERTY_KUOK_TRUE,
            .ksize       = MCUXCLELS_KEYPROPERTY_KEY_SIZE_128,
            .kactv       = MCUXCLELS_KEYPROPERTY_ACTIVE_TRUE,
        },
};

static const uint8_t s_CkdfDerivationDataWrapIn[12U] = {
    0xC8U, 0xACU, 0x48U, 0x88U, 0xA6U, 0x1BU, 0x3DU, 0x9BU, 0x56U, 0xA9U, 0x75U, 0xE7U,
};

static const uint8_t s_ImportDieIntEcdhSk[32] __attribute__((__aligned__(4))) = {
    0x82U, 0x9BU, 0xB4U, 0x4AU, 0x3BU, 0x6DU, 0x73U, 0x35U, 0x09U, 0x5EU, 0xD9U, 0x8DU, 0xF6U, 0x09U, 0x89U, 0x98U,
    0xACU, 0x63U, 0xABU, 0x4EU, 0x4EU, 0x78U, 0xF6U, 0x0AU, 0x70U, 0xEAU, 0x64U, 0x92U, 0xD4U, 0xFCU, 0xE4U, 0x92U};

static const uint8_t s_ImportDieIntEcdhPk[64] __attribute__((__aligned__(4))) = {
    0x8CU, 0xE2U, 0x3AU, 0x89U, 0xE7U, 0xC5U, 0xE9U, 0xB1U, 0x3EU, 0x89U, 0xEDU, 0xDBU, 0x69U, 0xB9U, 0x22U, 0xF8U,
    0xC2U, 0x8FU, 0x5DU, 0xCCU, 0x59U, 0x3EU, 0x5FU, 0x7BU, 0x6EU, 0x5AU, 0x6CU, 0xB3U, 0x62U, 0xC0U, 0x17U, 0x8AU,
    0x2FU, 0xDAU, 0xE8U, 0x72U, 0x67U, 0x7BU, 0xDFU, 0xFEU, 0xDBU, 0x4AU, 0x6EU, 0x39U, 0x2AU, 0x1BU, 0xAEU, 0xF8U,
    0x88U, 0x8FU, 0xC5U, 0x11U, 0xC3U, 0x67U, 0x85U, 0x5AU, 0xC5U, 0x54U, 0xBBU, 0xEBU, 0x19U, 0xF6U, 0x52U, 0x66U};
/*******************************************************************************
 * Code
 ******************************************************************************/
status_t cl_key_init_and_load(mcuxClSession_Handle_t session,
                              mcuxClKey_Handle_t key,
                              mcuxClKey_Type_t type,
                              uint8_t *data,
                              uint32_t key_data_length,
                              mcuxClEls_KeyProp_t *key_properties,
                              uint32_t *dst,
                              uint8_t key_loading_option)
{
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token,
                                     mcuxClKey_init(
                                         /* mcuxClSession_Handle_t session        */ session,
                                         /* mcuxClKey_Handle_t key                */ key,
                                         /* mcuxClKey_Type_t type                 */ type,
                                         /* uint8_t * pKeyData                    */ data,
                                         /* uint32_t keyDataLength                */ key_data_length));

    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClKey_init) != token) || (MCUXCLKEY_STATUS_OK != result))
    {
        return STATUS_ERROR_GENERIC;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    /* Set the key properties. */
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClKey_setKeyproperties(key, key_properties));

    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClKey_setKeyproperties) != token) || (MCUXCLKEY_STATUS_OK != result))
    {
        return STATUS_ERROR_GENERIC;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    if (key_loading_option == 0U)
    {
        MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token,
                                         mcuxClKey_loadMemory(
                                             /* mcuxClSession_Handle_t pSession:  */ session,
                                             /* mcuxClKey_Handle_t key:           */ key,
                                             /* uint32_t * dstData:               */ dst));

        if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClKey_loadMemory) != token) || (MCUXCLKEY_STATUS_OK != result))
        {
            return STATUS_ERROR_GENERIC;
        }
        MCUX_CSSL_FP_FUNCTION_CALL_END();
    }
    else
    {
        MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token,
                                         mcuxClKey_loadCopro(
                                             /* mcuxClSession_Handle_t pSession:  */ session,
                                             /* mcuxClKey_Handle_t key:           */ key,
                                             /* uint32_t dstSlot:                 */ *dst));

        if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClKey_loadCopro) != token) || (MCUXCLKEY_STATUS_OK != result))
        {
            return STATUS_ERROR_GENERIC;
        }
        MCUX_CSSL_FP_FUNCTION_CALL_END();
    }
    return STATUS_SUCCESS;
}

bool assert_equal(const uint8_t *const x, const uint8_t *const y, uint32_t length)
{
    for (uint32_t i = 0U; i < length; ++i)
    {
        if (x[i] != y[i])
        {
            return false;
        }
    }

    return true;
}

/*!
 * @brief Function to write a 32-bit unsigned integer in big-endian format to a byte array.
 */
static void write_uint32_msb_first(uint8_t *pos, uint32_t data)
{
    /* Write the most significant byte of the data to the first position in the byte array */
    pos[0U] = (uint8_t)(((data) >> 24U) & 0xFFU);

    /* Write the second most significant byte of the data to the second position in the byte array.
     * And so on..
     */
    pos[1U] = (uint8_t)(((data) >> 16U) & 0xFFU);
    pos[2U] = (uint8_t)(((data) >> 8U) & 0xFFU);
    pos[3U] = (uint8_t)(((data) >> 0U) & 0xFFU);
}

/*!
 * @brief Get amount of required keyslots given the propertiy.
 */
static uint32_t get_required_keyslots(mcuxClEls_KeyProp_t prop)
{
    /* Return the amount of required keyslots, depending on key size */
    return prop.bits.ksize == MCUXCLELS_KEYPROPERTY_KEY_SIZE_128 ? 1U : 2U;
}

/*!
 * @brief Check if keyslot is active.
 */
static bool els_is_active_keyslot(mcuxClEls_KeyIndex_t key_idx)
{
    /* Check if key has active property set */
    mcuxClEls_KeyProp_t key_properties;
    key_properties.word.value = ((const volatile uint32_t *)(&ELS->ELS_KS0))[key_idx];
    return (bool)key_properties.bits.kactv;
}

/*!
 * @brief Get ELS key properties.
 */
static status_t els_get_key_properties(mcuxClEls_KeyIndex_t key_index, mcuxClEls_KeyProp_t *key_properties)
{
    /* Wrapper function to return the key properties of a key in ELS keystore */
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClEls_GetKeyProperties(key_index, key_properties));
    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_GetKeyProperties) != token) || (MCUXCLELS_STATUS_OK != result))
    {
        return STATUS_ERROR_GENERIC;
    }

    MCUX_CSSL_FP_FUNCTION_CALL_END();
    return STATUS_SUCCESS;
}

mcuxClEls_KeyIndex_t els_get_free_keyslot(uint32_t required_keyslots)
{
    /* Return a free key slot of ELS keystore */
    for (mcuxClEls_KeyIndex_t keyIdx = 0U; keyIdx <= (MCUXCLELS_KEY_SLOTS - required_keyslots); keyIdx++)
    {
        bool is_valid_keyslot = true;
        for (uint32_t i = 0U; i < required_keyslots; i++)
        {
            /* Check if active, if yes continue searching */
            if (els_is_active_keyslot(keyIdx + i))
            {
                is_valid_keyslot = false;
                break;
            }
        }

        if (is_valid_keyslot)
        {
            return keyIdx;
        }
    }
    return MCUXCLELS_KEY_SLOTS;
}

/*!
 * @brief Generate keypair using ELS.
 */
static status_t els_generate_keypair(mcuxClEls_KeyIndex_t *dst_key_index, uint8_t *public_key, size_t *public_key_size)
{
    if (*public_key_size < (size_t)64U)
    {
        return STATUS_ERROR_GENERIC;
    }

    /* Initialize key gen options to do a key exchange, generating random output keys */
    mcuxClEls_EccKeyGenOption_t options = {0};
    options.bits.kgsrc                  = MCUXCLELS_ECC_OUTPUTKEY_RANDOM;
    options.bits.kgtypedh               = MCUXCLELS_ECC_OUTPUTKEY_KEYEXCHANGE;

    uint32_t keypair_required_keyslots = get_required_keyslots(s_KeypairProp);
    *dst_key_index                     = (mcuxClEls_KeyIndex_t)els_get_free_keyslot(keypair_required_keyslots);

    if (!(*dst_key_index < MCUXCLELS_KEY_SLOTS))
    {
        return STATUS_ERROR_GENERIC;
    }

    /* Generate private/public key pair */
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(
        result, token,
        mcuxClEls_EccKeyGen_Async(options, (mcuxClEls_KeyIndex_t)0U, *dst_key_index, s_KeypairProp, NULL, public_key));
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

    /* Public key size can only be 64 bytes */
    *public_key_size = (size_t)64U;
    return STATUS_SUCCESS;
}

status_t els_keygen(mcuxClEls_KeyIndex_t key_index, uint8_t *public_key, size_t *public_key_size)
{
    mcuxClEls_EccKeyGenOption_t key_gen_options;
    key_gen_options.word.value    = 0u;
    key_gen_options.bits.kgsign   = MCUXCLELS_ECC_PUBLICKEY_SIGN_DISABLE;
    key_gen_options.bits.kgsrc    = MCUXCLELS_ECC_OUTPUTKEY_DETERMINISTIC;
    key_gen_options.bits.skip_pbk = MCUXCLELS_ECC_GEN_PUBLIC_KEY;

    mcuxClEls_KeyProp_t key_properties;
    if (els_get_key_properties(key_index, &key_properties) != STATUS_SUCCESS)
    {
        return STATUS_ERROR_GENERIC;
    }

    /* Execute key gen operation */
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token,
                                     mcuxClEls_EccKeyGen_Async(key_gen_options, (mcuxClEls_KeyIndex_t)0, key_index,
                                                               key_properties, NULL, &public_key[0]));

    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_EccKeyGen_Async) != token) || (MCUXCLELS_STATUS_OK_WAIT != result))
    {
        return STATUS_ERROR_GENERIC;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    /* Execute wait operation */
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClEls_WaitForOperation(MCUXCLELS_ERROR_FLAGS_CLEAR));
    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_WaitForOperation) != token) || (MCUXCLELS_STATUS_OK != result))
    {
        return STATUS_ERROR_GENERIC;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    return STATUS_SUCCESS;
}

/*!
 * @brief Perform ECDH using ELS.
 */
static status_t els_perform_key_agreement(mcuxClEls_KeyIndex_t keypair_index,
                                          mcuxClEls_KeyProp_t shared_secret_prop,
                                          mcuxClEls_KeyIndex_t *dst_key_index,
                                          const uint8_t *public_key,
                                          size_t public_key_size)
{
    /* Get free key slot for keys */
    uint32_t shared_secret_required_keyslots = get_required_keyslots(shared_secret_prop);
    *dst_key_index                           = els_get_free_keyslot(shared_secret_required_keyslots);

    if (!(*dst_key_index < MCUXCLELS_KEY_SLOTS))
    {
        return STATUS_ERROR_GENERIC;
    }

    /* Perform ecc key exchange */
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(
        result, token, mcuxClEls_EccKeyExchange_Async(keypair_index, public_key, *dst_key_index, shared_secret_prop));

    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_EccKeyExchange_Async) != token) || (MCUXCLELS_STATUS_OK_WAIT != result))
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

    return STATUS_SUCCESS;
}

status_t els_delete_key(mcuxClEls_KeyIndex_t key_index)
{
    /* Execute delete key operation, to delete key in ELS keystore */
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClEls_KeyDelete_Async(key_index));
    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_KeyDelete_Async) != token) || (MCUXCLELS_STATUS_OK_WAIT != result))
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
    return STATUS_SUCCESS;
}

/*!
 * @brief Derive key using CKDF with ELS.
 */
static status_t els_derive_key(mcuxClEls_KeyIndex_t src_key_index,
                               mcuxClEls_KeyProp_t key_prop,
                               const uint8_t *dd,
                               mcuxClEls_KeyIndex_t *dst_key_index)
{
    /* Ger free key slot for key to derive */
    uint32_t required_keyslots = get_required_keyslots(key_prop);
    *dst_key_index             = els_get_free_keyslot(required_keyslots);

    if (!(*dst_key_index < MCUXCLELS_KEY_SLOTS))
    {
        return STATUS_ERROR_GENERIC;
    }
    /* Execute ELS key derivation using CKDF */
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token,
                                     mcuxClEls_Ckdf_Sp800108_Async(src_key_index, *dst_key_index, key_prop, dd));

    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_Ckdf_Sp800108_Async) != token) || (MCUXCLELS_STATUS_OK_WAIT != result))
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
    return STATUS_SUCCESS;
}

/*!
 * @brief Import key to key index using ELS.
 */
static status_t els_import_key(const uint8_t *wrapped_key,
                               size_t wrapped_key_size,
                               mcuxClEls_KeyProp_t key_prop,
                               mcuxClEls_KeyIndex_t unwrap_key_index,
                               mcuxClEls_KeyIndex_t *dst_key_index)
{
    /* Ger free key slot for key */
    uint32_t required_keyslots = get_required_keyslots(key_prop);
    *dst_key_index             = els_get_free_keyslot(required_keyslots);

    if (!(*dst_key_index < MCUXCLELS_KEY_SLOTS))
    {
        return STATUS_ERROR_GENERIC;
    }

    mcuxClEls_KeyImportOption_t options;
    options.bits.kfmt = MCUXCLELS_KEYIMPORT_KFMT_RFC3394;

    /* Execute ELS import key operation */
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(
        result, token,
        mcuxClEls_KeyImport_Async(options, wrapped_key, wrapped_key_size, unwrap_key_index, *dst_key_index));
    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_KeyImport_Async) != token) || (MCUXCLELS_STATUS_OK_WAIT != result))
    {
        return STATUS_ERROR_GENERIC;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    /* Wait for ELS import key operation */
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClEls_WaitForOperation(MCUXCLELS_ERROR_FLAGS_CLEAR));
    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_WaitForOperation) != token) || (MCUXCLELS_STATUS_OK != result))
    {
        return STATUS_ERROR_GENERIC;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    return STATUS_SUCCESS;
}

/*!
 * @brief Generate random bytes using ELS.
 */
static status_t els_get_random(unsigned char *out, size_t out_size)
{
    /* Get random IV for sector metadata encryption. */
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClCss_Rng_DrbgRequest_Async(out, out_size));
    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClCss_Rng_DrbgRequest_Async) != token) ||
        (MCUXCLELS_STATUS_OK_WAIT != result))
    {
        return STATUS_ERROR_GENERIC;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    /* Wait for DRBG operation */
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClCss_WaitForOperation(MCUXCLCSS_ERROR_FLAGS_CLEAR));
    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_WaitForOperation) != token) || (MCUXCLELS_STATUS_OK != result))
    {
        return STATUS_ERROR_GENERIC;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    return STATUS_SUCCESS;
}

/*!
 * @brief Derive key with mbedTLS using CKDF operation.
 */
static status_t host_derive_key(const uint8_t *input_key,
                                const uint8_t *derivation_data,
                                uint32_t key_properties,
                                uint8_t *output,
                                size_t *output_size)
{
    status_t status = STATUS_SUCCESS;

    int ret          = 0;
    uint32_t counter = 1U;
    mbedtls_cipher_context_t ctx;
    (void)memset(&ctx, 0, sizeof(ctx));

    /* Asserts for input */
    assert(*output_size == 32U);

    uint32_t lsbit         = key_properties & 0x01U;
    uint32_t length_blocks = 1U + lsbit;
    uint32_t length_bytes  = length_blocks * AES_BLOCK_SIZE;
    assert(*output_size >= length_bytes);
    *output_size = length_bytes;

    /*
     * KDF in counter mode implementation as described in Section 5.1
     * of NIST SP 800-108, Recommendation for Key Derivation Using Pseudorandom Functions
     * Derivation data[191:0](sic!) = software_derivation_data[95:0] || 64'h0 || requested_
     * properties[31:0 || length[31:0] || counter[31:0]
     */
    uint8_t dd[32U] = {0U};
    (void)memcpy(&dd[0U], derivation_data, 12U);
    (void)memset(&dd[12], 0, 8U);
    write_uint32_msb_first(&dd[20], key_properties);
    write_uint32_msb_first(&dd[24], length_bytes * (uint32_t)8U); /* expected in bits! */
    write_uint32_msb_first(&dd[28], counter);

    mbedtls_cipher_type_t mbedtls_cipher_type = MBEDTLS_CIPHER_AES_256_ECB;
    const mbedtls_cipher_info_t *cipher_info  = mbedtls_cipher_info_from_type(mbedtls_cipher_type);

    uint8_t *pos = output;
    do
    {
        mbedtls_cipher_init(&ctx);

        ret = mbedtls_cipher_setup(&ctx, cipher_info);
        CHECK_MBEDTLS_SUCCESS();

        ret = mbedtls_cipher_cmac_starts(&ctx, input_key, 32U * 8U);
        CHECK_MBEDTLS_SUCCESS();

        ret = mbedtls_cipher_cmac_update(&ctx, dd, sizeof(dd));
        CHECK_MBEDTLS_SUCCESS();

        ret = mbedtls_cipher_cmac_finish(&ctx, pos);
        CHECK_MBEDTLS_SUCCESS();

        mbedtls_cipher_free(&ctx);

        write_uint32_msb_first(&dd[28U], ++counter);
        pos += AES_BLOCK_SIZE;
    } while (counter * AES_BLOCK_SIZE <= length_bytes);
    if (status != STATUS_SUCCESS)
    {
        mbedtls_cipher_free(&ctx);
    }

    return status;
}

/*!
 * @brief Get random mbedTLS callback.
 */
static int get_random_mbedtls_callback(void *ctx, unsigned char *out, size_t out_size)
{
    /* Call els_get_random and check status */
    status_t status = els_get_random(out, out_size);
    if (status != STATUS_SUCCESS)
    {
        status = MBEDTLS_ERR_ENTROPY_SOURCE_FAILED;
    }
    return status;
}

/*!
 * @brief ECDH on host with mbedTLS.
 */
static status_t host_perform_key_agreement(const uint8_t *public_key,
                                           size_t public_key_size,
                                           uint8_t *shared_secret,
                                           size_t *shared_secret_size)
{
    /* Asserts on input */
    assert(public_key != NULL);
    assert(public_key_size == 64U);
    assert(shared_secret != NULL);
    assert(*shared_secret_size >= 32U);

    status_t status                    = STATUS_SUCCESS;
    uint8_t public_key_compressed[65U] = {0U};
    unsigned char strbuf[128U]         = {0U};
    size_t strlen                      = sizeof(strbuf);

    int ret = 0;

    /* Set correct domain parameters */
    mbedtls_ecp_group grp;
    mbedtls_ecp_point qB;
    mbedtls_mpi dA, zA;
    mbedtls_ecp_group_init(&grp);
    mbedtls_ecp_point_init(&qB);
    mbedtls_mpi_init(&dA);
    mbedtls_mpi_init(&zA);

    /* Perform key agreement using mbedTLS */
    *shared_secret_size = 32U;
    ret                 = mbedtls_ecp_group_load(&grp, MBEDTLS_ECP_DP_SECP256R1);
    CHECK_MBEDTLS_SUCCESS();

    ret = mbedtls_mpi_read_binary(&dA, s_ImportDieIntEcdhSk, sizeof(s_ImportDieIntEcdhSk));
    CHECK_MBEDTLS_SUCCESS();

    public_key_compressed[0U] = 0x04U;
    (void)memcpy(&public_key_compressed[1], public_key, public_key_size);

    ret = mbedtls_ecp_point_read_binary(&grp, &qB, public_key_compressed, sizeof(public_key_compressed));
    CHECK_MBEDTLS_SUCCESS();

    ret = mbedtls_ecdh_compute_shared(&grp, &zA, &qB, &dA, &get_random_mbedtls_callback, NULL);
    CHECK_MBEDTLS_SUCCESS();

    ret = mbedtls_ecp_point_write_binary(&grp, &qB, MBEDTLS_ECP_PF_UNCOMPRESSED, &strlen, &strbuf[0U], sizeof(strbuf));
    CHECK_MBEDTLS_SUCCESS();

    ret = mbedtls_mpi_write_binary(&zA, shared_secret, *shared_secret_size);
    CHECK_MBEDTLS_SUCCESS();

    return status;
}

/*!
 * @brief Wrap key on host with mbedTLS.
 */
static status_t host_wrap_key(
    const uint8_t *data, size_t data_size, const uint8_t *key, uint8_t *output, size_t *output_size)
{
    status_t status = STATUS_SUCCESS;
    int ret         = 0;

    /* Instantiate key wrap context */
    mbedtls_nist_kw_context ctx;
    mbedtls_nist_kw_init(&ctx);
    ret = mbedtls_nist_kw_setkey(&ctx, MBEDTLS_CIPHER_ID_AES, key, 16U * 8U, (int)true);
    CHECK_MBEDTLS_SUCCESS();

    /* Key wrap using mbedTLS */
    ret = mbedtls_nist_kw_wrap(&ctx, MBEDTLS_KW_MODE_KW, data, data_size, output, output_size, *output_size);
    CHECK_MBEDTLS_SUCCESS();

    if (status != STATUS_SUCCESS)
    {
        mbedtls_nist_kw_free(&ctx);
    }

    return status;
}

/*!
 * @brief Create ELS import key blob.
 */
static status_t create_els_import_keyblob(const uint8_t *plain_key,
                                          mcuxClEls_KeyProp_t plain_key_prop,
                                          const uint8_t *key_wrap_in,
                                          uint8_t *blob,
                                          size_t *blob_size)
{
    uint8_t buffer[ELS_BLOB_METADATA_SIZE + MAX_ELS_KEY_SIZE] = {0U};
    size_t buffer_size                                        = ELS_BLOB_METADATA_SIZE + 32U;

    /* Enforce the wrpok bit - the key needs to be re-wrappable! */
    plain_key_prop.bits.wrpok = (uint32_t)MCUXCLELS_KEYPROPERTY_WRAP_TRUE;

    /* This is what ELS documentation says, it does not work though.
     * memset(&buffer[0], 0xA6, 8);
     * write_uint32_msb_first(&buffer[8], plain_key_prop.word.value);
     * memset(&buffer[12], 0, 4);
     * memcpy(&buffer[16], plain_key, plain_key_size);
     */

    write_uint32_msb_first(&buffer[0U], plain_key_prop.word.value);

    (void)memset(&buffer[4U], 0, 4U);
    (void)memcpy(&buffer[8U], plain_key, 32U);

    status_t status = host_wrap_key(buffer, buffer_size, key_wrap_in, blob, blob_size);
    return status;
}

status_t import_plain_key_into_els(const uint8_t *plain_key,
                                   size_t plain_key_size,
                                   mcuxClEls_KeyProp_t key_properties,
                                   mcuxClEls_KeyIndex_t *index_output)
{
    uint8_t shared_secret[32U] = {0U};
    uint8_t key_wrap_in[32U];
    uint8_t els_key_in_blob[ELS_BLOB_METADATA_SIZE + MAX_ELS_KEY_SIZE + ELS_WRAP_OVERHEAD];

    size_t shared_secret_len    = sizeof(shared_secret);
    size_t key_wrap_in_size     = sizeof(key_wrap_in);
    size_t els_key_in_blob_size = sizeof(els_key_in_blob);

    mcuxClEls_KeyIndex_t index_plain         = MCUXCLELS_KEY_SLOTS;
    mcuxClEls_KeyIndex_t index_shared_secret = MCUXCLELS_KEY_SLOTS;
    mcuxClEls_KeyIndex_t index_unwrap        = MCUXCLELS_KEY_SLOTS;

    uint8_t public_key[64U] = {0U};
    size_t public_key_size  = sizeof(public_key);

    /* Generate private/public key */
    if (els_generate_keypair(&index_plain, &public_key[0U], &public_key_size) != STATUS_SUCCESS)
    {
        (void)els_delete_key(index_plain);
        return STATUS_ERROR_GENERIC;
    }

    if (host_perform_key_agreement(public_key, public_key_size, &shared_secret[0U], &shared_secret_len) !=
        STATUS_SUCCESS)
    {
        (void)els_delete_key(index_plain);
        return STATUS_ERROR_GENERIC;
    }

    /* CKDF with mbedTLS */
    if (host_derive_key(shared_secret, s_CkdfDerivationDataWrapIn, s_WrapInKeyProp.word.value, &key_wrap_in[0],
                        &key_wrap_in_size) != STATUS_SUCCESS)
    {
        (void)els_delete_key(index_plain);
        return STATUS_ERROR_GENERIC;
    }

    /* Import 256bit key into ELS key store */
    if (create_els_import_keyblob(plain_key, key_properties, key_wrap_in, &els_key_in_blob[0], &els_key_in_blob_size) !=
        STATUS_SUCCESS)
    {
        (void)els_delete_key(index_plain);
        return STATUS_ERROR_GENERIC;
    }

    /* Key exchange */
    if (els_perform_key_agreement(index_plain, s_SharedSecretProp, &index_shared_secret, s_ImportDieIntEcdhPk,
                                  sizeof(s_ImportDieIntEcdhPk)) != STATUS_SUCCESS)
    {
        (void)els_delete_key(index_plain);
        return STATUS_ERROR_GENERIC;
    }

    if (els_delete_key(index_plain) != STATUS_SUCCESS)
    {
        (void)els_delete_key(index_shared_secret);
        return STATUS_ERROR_GENERIC;
    }

    /* CKDF with ELS */
    if (els_derive_key(index_shared_secret, s_WrapInKeyProp, s_CkdfDerivationDataWrapIn, &index_unwrap) !=
        STATUS_SUCCESS)
    {
        (void)els_delete_key(index_shared_secret);
        return STATUS_ERROR_GENERIC;
    }

    if (els_delete_key(index_shared_secret) != STATUS_SUCCESS)
    {
        (void)els_delete_key(index_unwrap);
        return STATUS_ERROR_GENERIC;
    }

    if (els_import_key(els_key_in_blob, els_key_in_blob_size, key_properties, index_unwrap, index_output) !=
        STATUS_SUCCESS)
    {
        (void)els_delete_key(index_unwrap);
        return STATUS_ERROR_GENERIC;
    }

    if (els_delete_key(index_unwrap) != STATUS_SUCCESS)
    {
        return STATUS_ERROR_GENERIC;
    }

    return STATUS_SUCCESS;
}
