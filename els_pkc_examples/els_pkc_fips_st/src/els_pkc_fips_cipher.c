/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "els_pkc_fips_cipher.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
static uint8_t s_Nonce[10U] = {0xCAU, 0xFEU, 0x01U, 0x01U, 0xABU, 0xCDU, 0xECU, 0xAFU, 0xE0U, 0x10U};

static uint8_t s_Adata[32U] = {0x53U, 0x47U, 0x24U, 0x76U, 0x32U, 0x78U, 0x64U, 0x87U, 0x23U, 0x67U, 0x63U,
                               0x27U, 0x86U, 0x48U, 0x23U, 0x64U, 0x72U, 0x36U, 0x84U, 0x76U, 0x32U, 0x8AU,
                               0xBEU, 0xDAU, 0xDAU, 0xE2U, 0x42U, 0x42U, 0x43U, 0x43U, 0x23U, 0x52U};

static uint8_t s_PlainText[32U] = {0x01U, 0x01U, 0x01U, 0x01U, 0x01U, 0x01U, 0x02U, 0x34U, 0x42U, 0x34U, 0x32U,
                                   0x14U, 0x24U, 0x12U, 0x32U, 0x13U, 0x37U, 0x24U, 0x67U, 0x32U, 0x74U, 0x89U,
                                   0x73U, 0x28U, 0x74U, 0x32U, 0x87U, 0x48U, 0x93U, 0x26U, 0x49U, 0x82U};

static uint8_t s_Key128[16U] = {0x53U, 0x47U, 0x24U, 0x76U, 0x32U, 0x78U, 0x64U, 0x87U,
                                0x23U, 0x67U, 0x63U, 0x27U, 0x86U, 0x48U, 0x23U, 0x62U};

static uint8_t s_Key192[24U] = {0x53U, 0x47U, 0x24U, 0x76U, 0x32U, 0x78U, 0x64U, 0x87U, 0x23U, 0x67U, 0x63U, 0x27U,
                                0x86U, 0x48U, 0x23U, 0x62U, 0x12U, 0x46U, 0x88U, 0x53U, 0x32U, 0x32U, 0x42U, 0x31U};

static uint8_t s_Key256[32U] = {0x53U, 0x47U, 0x24U, 0x76U, 0x32U, 0x78U, 0x64U, 0x87U, 0x23U, 0x67U, 0x63U,
                                0x27U, 0x86U, 0x48U, 0x23U, 0x62U, 0x12U, 0x46U, 0x88U, 0x53U, 0x32U, 0x32U,
                                0x42U, 0x31U, 0xAAU, 0xAAU, 0xAAU, 0xAAU, 0xAAU, 0xAAU, 0xAAU, 0xAAU};

static uint8_t s_Iv[16U] = {0xF0U, 0xF1U, 0xF2U, 0xF3U, 0xF4U, 0xF5U, 0xF6U, 0xF7U,
                            0xF8U, 0xF9U, 0xFAU, 0xFBU, 0xFCU, 0xFDU, 0xFEU, 0xFFU};

static uint8_t s_CipherKatCBC128[32U] = {0x7AU, 0x24U, 0x33U, 0x67U, 0x25U, 0x84U, 0x01U, 0xCEU, 0x47U, 0x76U, 0xADU,
                                         0xDFU, 0x7AU, 0x4EU, 0x04U, 0xFCU, 0x01U, 0xB0U, 0xDDU, 0xC2U, 0x8CU, 0xECU,
                                         0x98U, 0x18U, 0xA9U, 0xB7U, 0xCEU, 0xA9U, 0xCCU, 0xE2U, 0x2BU, 0xACU};

static uint8_t s_CipherKatCBC192[32U] = {0x32U, 0x2AU, 0x67U, 0x14U, 0x0BU, 0xC8U, 0xADU, 0x99U, 0xEAU, 0x8DU, 0x94U,
                                         0x09U, 0x67U, 0x36U, 0xBDU, 0x81U, 0x20U, 0xEBU, 0xEAU, 0xCDU, 0x8CU, 0xC0U,
                                         0x63U, 0xABU, 0x70U, 0x9FU, 0xEFU, 0xE7U, 0x66U, 0x33U, 0x44U, 0x6CU};

static uint8_t s_CipherKatCBC256[32U] = {0x35U, 0x3CU, 0x12U, 0x92U, 0x06U, 0x52U, 0xDEU, 0x8AU, 0xCEU, 0x6CU, 0xF3U,
                                         0x3CU, 0x41U, 0x6BU, 0xD5U, 0x66U, 0xADU, 0x08U, 0x75U, 0x4EU, 0xECU, 0x46U,
                                         0x08U, 0xD4U, 0x26U, 0xB5U, 0x45U, 0x5DU, 0xB1U, 0x93U, 0x71U, 0x75U};

static uint8_t s_CipherKatECB128[32U] = {0x88U, 0x13U, 0x78U, 0x41U, 0x6DU, 0x56U, 0x6AU, 0x90U, 0x1CU, 0xDFU, 0x5BU,
                                         0xD0U, 0xA8U, 0xABU, 0x6FU, 0xECU, 0x2DU, 0x63U, 0x37U, 0xB5U, 0xF0U, 0x4AU,
                                         0x95U, 0xF4U, 0xF4U, 0x64U, 0x7DU, 0x82U, 0x62U, 0x5BU, 0xFEU, 0x62U};

static uint8_t s_CipherKatECB192[32U] = {0x99U, 0xCFU, 0x4BU, 0xFBU, 0xACU, 0xAFU, 0x47U, 0xB6U, 0x5EU, 0x3DU, 0x9DU,
                                         0x59U, 0x48U, 0x67U, 0xC8U, 0x2FU, 0xDDU, 0x67U, 0x64U, 0xAEU, 0x33U, 0x20U,
                                         0xF5U, 0x3EU, 0x97U, 0x94U, 0xB2U, 0x29U, 0xA3U, 0xEBU, 0x0FU, 0x3CU};

static uint8_t s_CipherKatECB256[32U] = {0x4EU, 0xCFU, 0xF8U, 0x56U, 0x93U, 0x9FU, 0xB1U, 0xCCU, 0x02U, 0x05U, 0x99U,
                                         0x2BU, 0x84U, 0xD4U, 0x1EU, 0x9EU, 0x26U, 0x7FU, 0x81U, 0x6DU, 0x29U, 0x97U,
                                         0xE8U, 0x58U, 0xFFU, 0x30U, 0xFDU, 0x76U, 0x4AU, 0x70U, 0x8FU, 0x12U};

static uint8_t s_CipherKatCCM128[32U] = {0x65U, 0x4AU, 0xA5U, 0x4CU, 0xBDU, 0xA3U, 0x8EU, 0xE2U, 0x70U, 0x61U, 0x9EU,
                                         0xD1U, 0xBAU, 0x96U, 0x59U, 0x0BU, 0x2BU, 0x12U, 0x28U, 0x7CU, 0x0CU, 0x7EU,
                                         0x1AU, 0x51U, 0x97U, 0x4CU, 0x8FU, 0x80U, 0x8AU, 0xBEU, 0x0FU, 0x40U};

static uint8_t s_CipherKatCCM256[32U] = {0x32U, 0xC3U, 0xCEU, 0xC2U, 0xFAU, 0x15U, 0xC7U, 0xCFU, 0xAAU, 0x8FU, 0xC3U,
                                         0x92U, 0x83U, 0x41U, 0xD8U, 0x3DU, 0x22U, 0x1DU, 0x69U, 0xA5U, 0xEBU, 0x34U,
                                         0xA1U, 0xD7U, 0x07U, 0xA9U, 0x90U, 0x3FU, 0xA1U, 0x27U, 0x61U, 0xC4U};

static uint8_t s_CipherKatGCM128[32U] = {0x98U, 0x0CU, 0x51U, 0x1EU, 0xAAU, 0x89U, 0xAAU, 0xC0U, 0xCBU, 0x34U, 0xB8U,
                                         0x2FU, 0xDDU, 0xDAU, 0xA7U, 0x2FU, 0xC0U, 0x99U, 0x34U, 0x15U, 0x59U, 0xA9U,
                                         0x69U, 0x7FU, 0x6DU, 0x42U, 0x37U, 0xDCU, 0x1BU, 0x71U, 0xF0U, 0xF0U};

static uint8_t s_CipherKatGCM192[32U] = {0x79U, 0xA1U, 0xC2U, 0xDBU, 0x1EU, 0x4AU, 0xC7U, 0xF2U, 0x13U, 0xA0U, 0x8BU,
                                         0x78U, 0xCCU, 0xEFU, 0x7CU, 0x17U, 0x3EU, 0x15U, 0x18U, 0x00U, 0xFAU, 0x34U,
                                         0xB3U, 0x06U, 0x00U, 0x71U, 0xC6U, 0x28U, 0xE8U, 0x0FU, 0x45U, 0x38U};

static uint8_t s_CipherKatGCM256[32U] = {0x81U, 0x70U, 0xD1U, 0x73U, 0x22U, 0xF5U, 0x08U, 0xEFU, 0xD8U, 0xD5U, 0x43U,
                                         0xDFU, 0x6EU, 0xA0U, 0x84U, 0x50U, 0xF2U, 0x72U, 0x9BU, 0x7AU, 0x81U, 0x30U,
                                         0x92U, 0xE5U, 0xBEU, 0x04U, 0xC3U, 0xECU, 0x07U, 0xB8U, 0x82U, 0x70U};

static uint8_t s_CipherKatCTR128[32U] = {0xEDU, 0x8DU, 0xDEU, 0x72U, 0x99U, 0x61U, 0x7EU, 0x84U, 0xB0U, 0xE6U, 0x24U,
                                         0x61U, 0xCEU, 0x8CU, 0x93U, 0xF7U, 0x01U, 0x0FU, 0x1BU, 0x0EU, 0x13U, 0xFAU,
                                         0x22U, 0x4BU, 0x6CU, 0x92U, 0xF0U, 0x9FU, 0x6FU, 0x76U, 0x3AU, 0x2CU};

static uint8_t s_CipherKatCTR192[32U] = {0x70U, 0x7CU, 0x2CU, 0xC7U, 0x38U, 0x13U, 0x81U, 0x00U, 0xE4U, 0x22U, 0x48U,
                                         0x5CU, 0xA9U, 0xFFU, 0x4BU, 0x32U, 0x90U, 0x0AU, 0xD4U, 0x89U, 0x60U, 0x2CU,
                                         0x25U, 0x5BU, 0x3FU, 0x49U, 0x2AU, 0x22U, 0x22U, 0x47U, 0x49U, 0x47U};

static uint8_t s_CipherKatCTR256[32U] = {0x0AU, 0xDEU, 0x7CU, 0xF0U, 0x58U, 0x16U, 0x14U, 0x07U, 0x1CU, 0xAEU, 0xB9U,
                                         0x01U, 0xECU, 0x72U, 0xF7U, 0x11U, 0x6DU, 0x4AU, 0x0EU, 0xAFU, 0x27U, 0xE8U,
                                         0x6AU, 0x2EU, 0x20U, 0x01U, 0x01U, 0x74U, 0x1CU, 0x43U, 0x32U, 0x16U};

static uint8_t s_KeyCTR128[16U] = {0x2BU, 0x7EU, 0x15U, 0x16U, 0x28U, 0xAEU, 0xD2U, 0xA6U,
                                   0xABU, 0xF7U, 0x15U, 0x88U, 0x09U, 0xCFU, 0x4FU, 0x3CU};

static uint8_t s_KeyCTR192[24U] = {0x8EU, 0x73U, 0xB0U, 0xF7U, 0xDAU, 0x0EU, 0x64U, 0x52U, 0xC8U, 0x10U, 0xF3U, 0x2BU,
                                   0x80U, 0x90U, 0x79U, 0xE5U, 0x62U, 0xF8U, 0xEAU, 0xD2U, 0x52U, 0x2CU, 0x6BU, 0x7BU};

static uint8_t s_KeyCTR256[32U] = {0x60U, 0x3DU, 0xEBU, 0x10U, 0x15U, 0xCAU, 0x71U, 0xBEU, 0x2BU, 0x73U, 0xAEU,
                                   0xF0U, 0x85U, 0x7DU, 0x77U, 0x81U, 0x1FU, 0x35U, 0x2CU, 0x07U, 0x3BU, 0x61U,
                                   0x08U, 0xD7U, 0x2DU, 0x98U, 0x10U, 0xA3U, 0x09U, 0x14U, 0xDFU, 0xF4U};
/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Execute AES decrypt/encrypt, depending on encrypt flag.
 */
static status_t aes_crypt(mcuxClCipher_Mode_t mode,
                          mcuxClKey_Type_t key_type,
                          uint8_t *plaintext,
                          uint32_t plaintext_size,
                          uint8_t *cipher,
                          uint32_t cipher_size,
                          uint8_t *key,
                          uint32_t key_size,
                          bool encrypt)
{
    /* Initialize session */
    mcuxClSession_Descriptor_t session_desc;
    mcuxClSession_Handle_t session = &session_desc;

    ALLOCATE_AND_INITIALIZE_SESSION(session, MCUXCLCIPHER_AES_CRYPT_CPU_WA_BUFFER_SIZE, 0U);

    /* Initialize key */
    uint32_t key_desc[MCUXCLKEY_DESCRIPTOR_SIZE_IN_WORDS];
    mcuxClKey_Handle_t key_handle = (mcuxClKey_Handle_t)&key_desc;

    /* Set key properties */
    mcuxClEls_KeyProp_t key_properties;

    key_properties.word.value = 0U;
    key_properties.bits.kactv = (uint32_t)MCUXCLELS_KEYPROPERTY_ACTIVE_TRUE;

    uint32_t dst_data[32U];

    /* Load and init key depending on key size of AES mode operation */
    if (cl_key_init_and_load(session, key_handle, key_type, key, key_size, &key_properties, dst_data, 0U) !=
        STATUS_SUCCESS)
    {
        return STATUS_ERROR_GENERIC;
    }

    uint32_t output_size = 0U;
    uint8_t output[64U]  = {0U};
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(
        result_enc, token_enc,
        mcuxClCipher_crypt(
            /* mcuxClSession_Handle_t session: */ session,
            /* mcuxClKey_Handle_t key:         */ key_handle,
            /* mcuxClCipher_Mode_t mode:       */ mode,
            /* mcuxCl_InputBuffer_t pIv:       */
            (mode == mcuxClCipher_Mode_AES_ECB_Enc_NoPadding || mode == mcuxClCipher_Mode_AES_ECB_Dec_NoPadding) ?
                NULL :
                s_Iv,
            /* uint32_t ivLength:              */
            (mode == mcuxClCipher_Mode_AES_ECB_Enc_NoPadding || mode == mcuxClCipher_Mode_AES_ECB_Dec_NoPadding) ?
                0U :
                sizeof(s_Iv),
            /* mcuxCl_InputBuffer_t pIn:       */ encrypt ? plaintext : cipher,
            /* uint32_t inLength:              */ plaintext_size,
            /* mcuxCl_Buffer_t pOut:           */ output,
            /* uint32_t * const pOutLength:    */ &output_size));

    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClCipher_crypt) != token_enc) || (MCUXCLCIPHER_STATUS_OK != result_enc))
    {
        return STATUS_ERROR_GENERIC;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    /* Check results */
    if (!assert_equal(output, encrypt ? cipher : plaintext, plaintext_size))
    {
        return STATUS_ERROR_GENERIC;
    }

    /* Flush the key */
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClKey_flush(session, key_handle));

    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClKey_flush) != token) || (MCUXCLKEY_STATUS_OK != result))
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

/*!
 * @brief Execute AES AEAD decrypt/encrypt, depending on encrypt flag.
 */
static status_t aes_aead_crypt(mcuxClAead_Mode_t mode,
                               mcuxClKey_Type_t key_type,
                               uint8_t *plaintext,
                               uint32_t plaintext_size,
                               uint8_t *cipher,
                               uint32_t cipher_size,
                               uint8_t *key,
                               uint32_t key_size,
                               bool encrypt)
{
    /* Initialize session */
    mcuxClSession_Descriptor_t session_desc;
    mcuxClSession_Handle_t session = &session_desc;

    ALLOCATE_AND_INITIALIZE_SESSION(session, MCUXCLAEAD_CRYPT_CPU_WA_BUFFER_SIZE, 0U);

    /* Initialize the PRNG */
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(prngInit_result, prngInit_token, mcuxClRandom_ncInit(session));
    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandom_ncInit) != prngInit_token) ||
        (MCUXCLRANDOM_STATUS_OK != prngInit_result))
    {
        return STATUS_ERROR_GENERIC;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    /* Initialize key */
    uint32_t key_desc[MCUXCLKEY_DESCRIPTOR_SIZE_IN_WORDS];
    mcuxClKey_Handle_t key_handle = (mcuxClKey_Handle_t)&key_desc;

    /* Set key properties */
    mcuxClEls_KeyProp_t key_properties;
    key_properties.word.value = 0U;
    key_properties.bits.kactv = (uint32_t)MCUXCLELS_KEYPROPERTY_ACTIVE_TRUE;

    uint32_t dst_data[32U] = {0U};

    /* Load and init key depending on key size of AES mode operation */
    if (cl_key_init_and_load(session, key_handle, key_type, key, key_size, &key_properties, dst_data, 0U) !=
        STATUS_SUCCESS)
    {
        return STATUS_ERROR_GENERIC;
    }

    uint32_t output_size   = 0U;
    uint8_t output[64U]    = {0U};
    uint8_t tag_output[8U] = {0U};

    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result_enc, token_enc,
                                     mcuxClAead_crypt(
                                         /* mcuxClSession_Handle_t session, */ session,
                                         /* mcuxClKey_Handle_t key,         */ key_handle,
                                         /* mcuxClAead_Mode_t mode,         */ mode,
                                         /* mcuxCl_InputBuffer_t pNonce,    */ s_Nonce,
                                         /* uint32_t nonceSize,             */ sizeof(s_Nonce),
                                         /* mcuxCl_InputBuffer_t pIn,       */ encrypt ? plaintext : cipher,
                                         /* uint32_t inSize,                */ plaintext_size,
                                         /* mcuxCl_InputBuffer_t pAdata,    */ s_Adata,
                                         /* uint32_t adataSize,             */ sizeof(s_Adata),
                                         /* mcuxCl_Buffer_t pOut,           */ output,
                                         /* uint32_t * const pOutSize       */ &output_size,
                                         /* mcuxCl_Buffer_t pTag,           */ tag_output,
                                         /* uint32_t tagSize                */ sizeof(tag_output)));
    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClAead_crypt) != token_enc) || (MCUXCLAEAD_STATUS_OK != result_enc))
    {
        return STATUS_ERROR_GENERIC;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    if (!assert_equal(output, encrypt ? cipher : plaintext, plaintext_size))
    {
        return STATUS_ERROR_GENERIC;
    }

    /* Flush the key */
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClKey_flush(session, key_handle));

    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClKey_flush) != token) || (MCUXCLKEY_STATUS_OK != result))
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

void execute_cbc_kat(uint64_t options, char name[])
{
    /* Execute KAT for CBC-128 */
    if ((bool)(options & FIPS_AES_CBC_128))
    {
        CHECK_STATUS_AND_LOG(
            aes_crypt(mcuxClCipher_Mode_AES_CBC_Enc_NoPadding, mcuxClKey_Type_Aes128, s_PlainText, sizeof(s_PlainText),
                      s_CipherKatCBC128, sizeof(s_CipherKatCBC128), s_Key128, sizeof(s_Key128), true),
            name, "ENCRYPT KAT");
        CHECK_STATUS_AND_LOG(
            aes_crypt(mcuxClCipher_Mode_AES_CBC_Dec_NoPadding, mcuxClKey_Type_Aes128, s_PlainText, sizeof(s_PlainText),
                      s_CipherKatCBC128, sizeof(s_CipherKatCBC128), s_Key128, sizeof(s_Key128), false),
            name, "DECRYPT KAT");
    }
    /* Execute KAT for CBC-192 */
    if ((bool)(options & FIPS_AES_CBC_192))
    {
        CHECK_STATUS_AND_LOG(
            aes_crypt(mcuxClCipher_Mode_AES_CBC_Enc_NoPadding, mcuxClKey_Type_Aes192, s_PlainText, sizeof(s_PlainText),
                      s_CipherKatCBC192, sizeof(s_CipherKatCBC192), s_Key192, sizeof(s_Key192), true),
            name, "ENCRYPT KAT");
        CHECK_STATUS_AND_LOG(
            aes_crypt(mcuxClCipher_Mode_AES_CBC_Dec_NoPadding, mcuxClKey_Type_Aes192, s_PlainText, sizeof(s_PlainText),
                      s_CipherKatCBC192, sizeof(s_CipherKatCBC192), s_Key192, sizeof(s_Key192), false),
            name, "DECRYPT KAT");
    }
    /* Execute KAT for CBC-256 */
    if ((bool)(options & FIPS_AES_CBC_256))
    {
        CHECK_STATUS_AND_LOG(
            aes_crypt(mcuxClCipher_Mode_AES_CBC_Enc_NoPadding, mcuxClKey_Type_Aes256, s_PlainText, sizeof(s_PlainText),
                      s_CipherKatCBC256, sizeof(s_CipherKatCBC256), s_Key256, sizeof(s_Key256), true),
            name, "ENCRYPT KAT");
        CHECK_STATUS_AND_LOG(
            aes_crypt(mcuxClCipher_Mode_AES_CBC_Dec_NoPadding, mcuxClKey_Type_Aes256, s_PlainText, sizeof(s_PlainText),
                      s_CipherKatCBC256, sizeof(s_CipherKatCBC256), s_Key256, sizeof(s_Key256), false),
            name, "DECRYPT KAT");
    }
}

void execute_ecb_kat(uint64_t options, char name[])
{
    /* Execute KAT for ECB-128 */
    if ((bool)(options & FIPS_AES_ECB_128))
    {
        CHECK_STATUS_AND_LOG(
            aes_crypt(mcuxClCipher_Mode_AES_ECB_Enc_NoPadding, mcuxClKey_Type_Aes128, s_PlainText, sizeof(s_PlainText),
                      s_CipherKatECB128, sizeof(s_CipherKatECB128), s_Key128, sizeof(s_Key128), true),
            name, "ENCRYPT KAT");
        CHECK_STATUS_AND_LOG(
            aes_crypt(mcuxClCipher_Mode_AES_ECB_Dec_NoPadding, mcuxClKey_Type_Aes128, s_PlainText, sizeof(s_PlainText),
                      s_CipherKatECB128, sizeof(s_CipherKatECB128), s_Key128, sizeof(s_Key128), false),
            name, "DECRYPT KAT");
    }
    /* Execute KAT for ECB-192 */
    if ((bool)(options & FIPS_AES_ECB_192))
    {
        CHECK_STATUS_AND_LOG(
            aes_crypt(mcuxClCipher_Mode_AES_ECB_Enc_NoPadding, mcuxClKey_Type_Aes192, s_PlainText, sizeof(s_PlainText),
                      s_CipherKatECB192, sizeof(s_CipherKatECB192), s_Key192, sizeof(s_Key192), true),
            name, "ENCRYPT KAT");
        CHECK_STATUS_AND_LOG(
            aes_crypt(mcuxClCipher_Mode_AES_ECB_Dec_NoPadding, mcuxClKey_Type_Aes192, s_PlainText, sizeof(s_PlainText),
                      s_CipherKatECB192, sizeof(s_CipherKatECB192), s_Key192, sizeof(s_Key192), false),
            name, "DECRYPT KAT");
    }
    /* Execute KAT for ECB-256 */
    if ((bool)(options & FIPS_AES_ECB_256))
    {
        CHECK_STATUS_AND_LOG(
            aes_crypt(mcuxClCipher_Mode_AES_ECB_Enc_NoPadding, mcuxClKey_Type_Aes256, s_PlainText, sizeof(s_PlainText),
                      s_CipherKatECB256, sizeof(s_CipherKatECB256), s_Key256, sizeof(s_Key256), true),
            name, "ENCRYPT KAT");
        CHECK_STATUS_AND_LOG(
            aes_crypt(mcuxClCipher_Mode_AES_ECB_Dec_NoPadding, mcuxClKey_Type_Aes256, s_PlainText, sizeof(s_PlainText),
                      s_CipherKatECB256, sizeof(s_CipherKatECB256), s_Key256, sizeof(s_Key256), false),
            name, "DECRYPT KAT");
    }
}

void execute_ctr_kat(uint64_t options, char name[])
{
    /* Execute KAT for CTR-128 */
    if ((bool)(options & FIPS_AES_CTR_128))
    {
        CHECK_STATUS_AND_LOG(
            aes_crypt(mcuxClCipher_Mode_AES_CTR, mcuxClKey_Type_Aes128, s_PlainText, sizeof(s_PlainText),
                      s_CipherKatCTR128, sizeof(s_CipherKatCTR128), s_KeyCTR128, sizeof(s_KeyCTR128), true),
            name, "ENCRYPT KAT");
        CHECK_STATUS_AND_LOG(
            aes_crypt(mcuxClCipher_Mode_AES_CTR, mcuxClKey_Type_Aes128, s_PlainText, sizeof(s_PlainText),
                      s_CipherKatCTR128, sizeof(s_CipherKatCTR128), s_KeyCTR128, sizeof(s_KeyCTR128), false),
            name, "DECRYPT KAT");
    }
    /* Execute KAT for CTR-192 */
    if ((bool)(options & FIPS_AES_CTR_192))
    {
        CHECK_STATUS_AND_LOG(
            aes_crypt(mcuxClCipher_Mode_AES_CTR, mcuxClKey_Type_Aes192, s_PlainText, sizeof(s_PlainText),
                      s_CipherKatCTR192, sizeof(s_CipherKatCTR192), s_KeyCTR192, sizeof(s_KeyCTR192), true),
            name, "ENCRYPT KAT");
        CHECK_STATUS_AND_LOG(
            aes_crypt(mcuxClCipher_Mode_AES_CTR, mcuxClKey_Type_Aes192, s_PlainText, sizeof(s_PlainText),
                      s_CipherKatCTR192, sizeof(s_CipherKatCTR192), s_KeyCTR192, sizeof(s_KeyCTR192), false),
            name, "DECRYPT KAT");
    }
    /* Execute KAT for CTR-256 */
    if ((bool)(options & FIPS_AES_CTR_256))
    {
        CHECK_STATUS_AND_LOG(
            aes_crypt(mcuxClCipher_Mode_AES_CTR, mcuxClKey_Type_Aes256, s_PlainText, sizeof(s_PlainText),
                      s_CipherKatCTR256, sizeof(s_CipherKatCTR256), s_KeyCTR256, sizeof(s_KeyCTR256), true),
            name, "ENCRYPT KAT");
        CHECK_STATUS_AND_LOG(
            aes_crypt(mcuxClCipher_Mode_AES_CTR, mcuxClKey_Type_Aes256, s_PlainText, sizeof(s_PlainText),
                      s_CipherKatCTR256, sizeof(s_CipherKatCTR256), s_KeyCTR256, sizeof(s_KeyCTR256), false),
            name, "DECRYPT KAT");
    }
}

void execute_ccm_kat(uint64_t options, char name[])
{
    /* Execute KAT for CCM-128
     * Note: No KAT for keysize 192, since no implementation existing.
     */
    if ((bool)(options & FIPS_AES_CCM_128))
    {
        CHECK_STATUS_AND_LOG(
            aes_aead_crypt(mcuxClAead_Mode_AES_CCM_ENC, mcuxClKey_Type_Aes128, s_PlainText, sizeof(s_PlainText),
                           s_CipherKatCCM128, sizeof(s_CipherKatCCM128), s_Key128, sizeof(s_Key128), true),
            name, "ENCRYPT KAT");
        CHECK_STATUS_AND_LOG(
            aes_aead_crypt(mcuxClAead_Mode_AES_CCM_DEC, mcuxClKey_Type_Aes128, s_PlainText, sizeof(s_PlainText),
                           s_CipherKatCCM128, sizeof(s_CipherKatCCM128), s_Key128, sizeof(s_Key128), false),
            name, "DECRYPT KAT");
    }
    /* Execute KAT for CCM-256 */
    if ((bool)(options & FIPS_AES_CCM_256))
    {
        CHECK_STATUS_AND_LOG(
            aes_aead_crypt(mcuxClAead_Mode_AES_CCM_ENC, mcuxClKey_Type_Aes256, s_PlainText, sizeof(s_PlainText),
                           s_CipherKatCCM256, sizeof(s_CipherKatCCM256), s_Key256, sizeof(s_Key256), true),
            name, "ENCRYPT KAT");
        CHECK_STATUS_AND_LOG(
            aes_aead_crypt(mcuxClAead_Mode_AES_CCM_DEC, mcuxClKey_Type_Aes256, s_PlainText, sizeof(s_PlainText),
                           s_CipherKatCCM256, sizeof(s_CipherKatCCM256), s_Key256, sizeof(s_Key256), false),
            name, "DECRYPT KAT");
    }
}

void execute_gcm_kat(uint64_t options, char name[])
{
    /* Execute KAT for GCM-128 */
    if ((bool)(options & FIPS_AES_GCM_128))
    {
        CHECK_STATUS_AND_LOG(
            aes_aead_crypt(mcuxClAead_Mode_AES_GCM_ENC, mcuxClKey_Type_Aes128, s_PlainText, sizeof(s_PlainText),
                           s_CipherKatGCM128, sizeof(s_CipherKatGCM128), s_Key128, sizeof(s_Key128), true),
            name, "ENCRYPT KAT");
        CHECK_STATUS_AND_LOG(
            aes_aead_crypt(mcuxClAead_Mode_AES_GCM_DEC, mcuxClKey_Type_Aes128, s_PlainText, sizeof(s_PlainText),
                           s_CipherKatGCM128, sizeof(s_CipherKatGCM128), s_Key128, sizeof(s_Key128), false),
            name, "DECRYPT KAT");
    }
    /* Execute KAT for GCM-192 */
    if ((bool)(options & FIPS_AES_GCM_192))
    {
        CHECK_STATUS_AND_LOG(
            aes_aead_crypt(mcuxClAead_Mode_AES_GCM_ENC, mcuxClKey_Type_Aes192, s_PlainText, sizeof(s_PlainText),
                           s_CipherKatGCM192, sizeof(s_CipherKatGCM192), s_Key192, sizeof(s_Key192), true),
            name, "ENCRYPT KAT");
        CHECK_STATUS_AND_LOG(
            aes_aead_crypt(mcuxClAead_Mode_AES_GCM_DEC, mcuxClKey_Type_Aes192, s_PlainText, sizeof(s_PlainText),
                           s_CipherKatGCM192, sizeof(s_CipherKatGCM192), s_Key192, sizeof(s_Key192), false),
            name, "DECRYPT KAT");
    }
    /* Execute KAT for GCM-256 */
    if ((bool)(options & FIPS_AES_GCM_256))
    {
        CHECK_STATUS_AND_LOG(
            aes_aead_crypt(mcuxClAead_Mode_AES_GCM_ENC, mcuxClKey_Type_Aes256, s_PlainText, sizeof(s_PlainText),
                           s_CipherKatGCM256, sizeof(s_CipherKatGCM256), s_Key256, sizeof(s_Key256), true),
            name, "ENCRYPT KAT");
        CHECK_STATUS_AND_LOG(
            aes_aead_crypt(mcuxClAead_Mode_AES_GCM_ENC, mcuxClKey_Type_Aes256, s_PlainText, sizeof(s_PlainText),
                           s_CipherKatGCM256, sizeof(s_CipherKatGCM256), s_Key256, sizeof(s_Key256), false),
            name, "DECRYPT KAT");
    }
}
