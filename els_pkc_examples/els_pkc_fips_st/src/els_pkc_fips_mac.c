/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "els_pkc_fips_mac.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* HMAC variables */
static uint8_t s_KeyHmacSha224[50U] __attribute__((__aligned__(4))) = {
    0xD2U, 0x70U, 0x6FU, 0x0EU, 0x3EU, 0x78U, 0x0AU, 0xC5U, 0x27U, 0x7AU, 0x6BU, 0x72U, 0x49U,
    0x87U, 0xBBU, 0x61U, 0x3BU, 0x1EU, 0x47U, 0xA9U, 0xBAU, 0x88U, 0xBBU, 0x5FU, 0xF6U, 0xE3U,
    0xC2U, 0x7FU, 0x13U, 0xCEU, 0xB6U, 0x72U, 0x1EU, 0x44U, 0x26U, 0x06U, 0x9AU, 0x18U, 0xBEU,
    0x87U, 0xF9U, 0x43U, 0x42U, 0xD5U, 0x7BU, 0xBAU, 0xB3U, 0x59U, 0xB6U, 0xCCU};
static uint8_t s_MsgHmacSha224[128U] __attribute__((__aligned__(4))) = {
    0xADU, 0x64U, 0xD9U, 0x93U, 0x2AU, 0x72U, 0x4CU, 0x65U, 0xE3U, 0x6BU, 0xB0U, 0x42U, 0x8BU, 0xEBU, 0xE5U, 0x14U,
    0x0BU, 0x9BU, 0x0FU, 0x8AU, 0x04U, 0xCFU, 0x4BU, 0x42U, 0xC7U, 0xA1U, 0xEAU, 0xF4U, 0x7CU, 0x86U, 0x6DU, 0x1CU,
    0xA5U, 0xA9U, 0xEEU, 0x10U, 0x2BU, 0xD9U, 0x3FU, 0xBEU, 0xA0U, 0x34U, 0x19U, 0xEFU, 0xF4U, 0x25U, 0x38U, 0xF6U,
    0xEFU, 0x97U, 0x75U, 0x7EU, 0x89U, 0x9BU, 0xFCU, 0x53U, 0xEBU, 0xC0U, 0x70U, 0xC2U, 0x08U, 0xDDU, 0xC4U, 0x64U,
    0xFEU, 0xD4U, 0x1EU, 0xD8U, 0x5FU, 0x21U, 0x07U, 0x51U, 0xCCU, 0x55U, 0xB2U, 0xD2U, 0x51U, 0x49U, 0x0EU, 0x61U,
    0xB1U, 0xE8U, 0x18U, 0xFFU, 0x04U, 0x82U, 0x3EU, 0x9CU, 0xBFU, 0x18U, 0xA6U, 0xC6U, 0x1EU, 0xADU, 0x25U, 0xFCU,
    0x99U, 0x32U, 0xEBU, 0x04U, 0x09U, 0x31U, 0x43U, 0x99U, 0xB2U, 0x78U, 0xA3U, 0x51U, 0x03U, 0x48U, 0x4DU, 0xC0U,
    0x8DU, 0xD6U, 0xDAU, 0xC0U, 0xF9U, 0x89U, 0x22U, 0xA0U, 0x23U, 0x6EU, 0xC2U, 0xF5U, 0xFBU, 0xD3U, 0xF5U, 0xA1U};

static uint8_t s_MacKatHmacSha224[28U] __attribute__((__aligned__(4))) = {
    0x75U, 0x15U, 0x12U, 0x16U, 0x93U, 0xBBU, 0x0DU, 0x4BU, 0xE5U, 0x26U, 0xF0U, 0x00U, 0x17U, 0x8AU,
    0x12U, 0x1DU, 0x88U, 0xDBU, 0x8EU, 0x54U, 0x4FU, 0xE2U, 0x2FU, 0x67U, 0x64U, 0x9CU, 0xCFU, 0xBAU};

static uint8_t s_KeyHmacSha256[64U] __attribute__((__aligned__(4))) = {
    0xAEU, 0xD2U, 0x54U, 0x02U, 0x82U, 0x32U, 0xBEU, 0xDAU, 0x29U, 0x48U, 0x23U, 0x41U, 0x03U, 0xBBU, 0xABU, 0xABU,
    0x24U, 0x36U, 0x37U, 0x28U, 0x20U, 0x00U, 0x00U, 0xABU, 0xABU, 0x22U, 0x32U, 0x32U, 0x46U, 0x23U, 0x41U, 0x12U,
    0xAEU, 0xD2U, 0x54U, 0x02U, 0x82U, 0x32U, 0xBEU, 0xDAU, 0x29U, 0x48U, 0x23U, 0x41U, 0x03U, 0xBBU, 0xABU, 0xABU,
    0x24U, 0x36U, 0x37U, 0x28U, 0x20U, 0x00U, 0x00U, 0xABU, 0xABU, 0x22U, 0x32U, 0x32U, 0x46U, 0x23U, 0x41U, 0x12U};

static uint8_t s_MsgHmacSha256[64U] __attribute__((__aligned__(4))) = {
    0xAAU, 0xAAU, 0xAAU, 0xAAU, 0xAAU, 0xAAU, 0xAAU, 0xAAU, 0xAAU, 0xAAU, 0xAAU, 0xAAU,      0xAAU,
    0xAAU, 0xAAU, 0xAAU, 0xAAU, 0xAAU, 0xAAU, 0xAAU, 0xAAU, 0xAAU, 0xAAU, 0xAAU, 0xAAU,      0xAAU,
    0xAAU, 0xAAU, 0xAAU, 0xAAU, 0xAAU, 0xAAU, 0xAAU, 0xAAU, 0xAAU, 0xAAU, 0xAAU, 0xAAU,      0xAAU,
    0xAAU, 0xAAU, 0xAAU, 0xAAU, 0xAAU, 0xAAU, 0xAAU, 0xAAU, 0xAAU, 0xAAU, 0xAAU, 0b10000000, 0x00U,
    0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x03U, 0x90U};

static uint8_t s_MacKatHmacSha256[32U] __attribute__((__aligned__(4))) = {
    0x97U, 0x48U, 0x04U, 0xD9U, 0x2CU, 0x07U, 0x1FU, 0x13U, 0xE6U, 0x53U, 0x68U, 0x35U, 0x44U, 0x5FU, 0xF2U, 0xABU,
    0x30U, 0x78U, 0x7FU, 0xC9U, 0x66U, 0xD2U, 0x7CU, 0xAAU, 0xB8U, 0x6EU, 0xE2U, 0xF6U, 0x48U, 0x90U, 0x1CU, 0xE1U};

static uint8_t s_KeyHmacSha384[50U] __attribute__((__aligned__(4))) = {
    0xC3U, 0x64U, 0x6AU, 0xE2U, 0xD6U, 0x5CU, 0x7CU, 0x2AU, 0xB2U, 0xEDU, 0x0AU, 0xC8U, 0xAEU,
    0x37U, 0xD5U, 0x39U, 0xA7U, 0x7EU, 0x68U, 0xFBU, 0x09U, 0x9CU, 0x78U, 0x99U, 0x6CU, 0x55U,
    0xD1U, 0x9EU, 0x6EU, 0x5AU, 0xD3U, 0xB6U, 0x09U, 0x58U, 0x5DU, 0x17U, 0xF9U, 0x9CU, 0x1BU,
    0xF6U, 0x6FU, 0xF1U, 0xB0U, 0x13U, 0x8FU, 0x7DU, 0x9EU, 0xBDU, 0x7FU, 0x24U};

static uint8_t s_MsgHmacSha384[128U] __attribute__((__aligned__(4))) = {
    0xB9U, 0xD4U, 0x4FU, 0x76U, 0xEDU, 0xB2U, 0x8DU, 0x83U, 0x6FU, 0xC0U, 0xDDU, 0x55U, 0xCEU, 0xE0U, 0x30U, 0x69U,
    0xF1U, 0x86U, 0x8AU, 0x5AU, 0x87U, 0xB5U, 0x12U, 0x65U, 0x4BU, 0x3AU, 0x32U, 0x0CU, 0x43U, 0xEDU, 0xD1U, 0xA2U,
    0x12U, 0xA7U, 0x43U, 0xC9U, 0xFDU, 0xFAU, 0xF7U, 0xE7U, 0x90U, 0x5EU, 0xBCU, 0xD7U, 0xF3U, 0x9AU, 0xEBU, 0xCCU,
    0x3CU, 0x6CU, 0x10U, 0xABU, 0xC5U, 0xE9U, 0x77U, 0xE8U, 0x32U, 0x8AU, 0x9CU, 0x2EU, 0x1EU, 0x92U, 0xAAU, 0x7BU,
    0xA8U, 0x9EU, 0x9DU, 0xF3U, 0x2FU, 0x17U, 0x9DU, 0xB7U, 0xB5U, 0x8BU, 0xC6U, 0x43U, 0x74U, 0xA3U, 0xA2U, 0xA7U,
    0x8EU, 0xFDU, 0x0FU, 0xC1U, 0x46U, 0x3FU, 0xD1U, 0xD3U, 0x1EU, 0x63U, 0xBCU, 0x82U, 0x1AU, 0x8FU, 0x21U, 0x7EU,
    0x9FU, 0x8CU, 0xADU, 0x2CU, 0xAAU, 0x9AU, 0xB1U, 0xC0U, 0x2DU, 0xC4U, 0x1AU, 0x84U, 0x11U, 0xD8U, 0xA2U, 0xCFU,
    0xCDU, 0x27U, 0xA8U, 0x0FU, 0xD5U, 0x52U, 0x67U, 0xBDU, 0xFFU, 0x25U, 0x65U, 0x01U, 0x1DU, 0x17U, 0x1AU, 0xCDU};

static uint8_t s_MacKatHmacSha384[48U] __attribute__((__aligned__(4))) = {
    0x51U, 0x9EU, 0x6AU, 0xB0U, 0x60U, 0xE5U, 0x4FU, 0xCBU, 0xB3U, 0x91U, 0x4DU, 0x29U, 0x7EU, 0xF3U, 0x3DU, 0xBBU,
    0x15U, 0x09U, 0x58U, 0xE9U, 0x31U, 0x0EU, 0x79U, 0x10U, 0xD4U, 0xA7U, 0x14U, 0x70U, 0x39U, 0x76U, 0x2EU, 0xBDU,
    0xDAU, 0x92U, 0x1BU, 0x7DU, 0x46U, 0x27U, 0xEFU, 0x85U, 0xB2U, 0x98U, 0x93U, 0xA7U, 0xD9U, 0x8DU, 0xD9U, 0xF7U};

static uint8_t s_KeyHmacSha512[50U] __attribute__((__aligned__(4))) = {
    0x09U, 0x57U, 0x05U, 0x7EU, 0xD8U, 0x4FU, 0x8DU, 0x6CU, 0xE6U, 0x00U, 0x58U, 0x0DU, 0x21U,
    0x83U, 0xD8U, 0xADU, 0xAFU, 0x7DU, 0x0DU, 0xEEU, 0xD9U, 0x6CU, 0x8EU, 0x42U, 0x1CU, 0x34U,
    0xAAU, 0x03U, 0x2BU, 0xF7U, 0xB7U, 0x1BU, 0x4FU, 0x12U, 0x13U, 0x8CU, 0x1AU, 0x9BU, 0x7FU,
    0x80U, 0x17U, 0x75U, 0x1DU, 0x42U, 0x24U, 0x15U, 0x01U, 0x47U, 0x63U, 0xF6U};

static uint8_t s_MsgHmacSha512[128U] __attribute__((__aligned__(4))) = {
    0xFCU, 0x28U, 0x96U, 0x33U, 0x34U, 0xB9U, 0x88U, 0x85U, 0xB2U, 0x06U, 0xDFU, 0x49U, 0x8AU, 0x56U, 0xE8U, 0x2BU,
    0x3CU, 0x54U, 0x3CU, 0x51U, 0xA5U, 0xF3U, 0x73U, 0xFFU, 0xCFU, 0xB9U, 0x1BU, 0x42U, 0x14U, 0x74U, 0x52U, 0x64U,
    0x4CU, 0xC9U, 0xF9U, 0xABU, 0x48U, 0x88U, 0x00U, 0xC1U, 0x6BU, 0x93U, 0x91U, 0xF8U, 0xDFU, 0x3EU, 0xF5U, 0x64U,
    0xEEU, 0x9CU, 0x0AU, 0x9EU, 0xBAU, 0x43U, 0x65U, 0xC5U, 0xDCU, 0xAAU, 0x12U, 0xF8U, 0x54U, 0x17U, 0xC3U, 0xE6U,
    0x91U, 0xF3U, 0xD6U, 0xF3U, 0x2EU, 0xF6U, 0x69U, 0x2EU, 0xA4U, 0x6DU, 0x5DU, 0x61U, 0x86U, 0x28U, 0x2DU, 0xE5U,
    0x81U, 0x0DU, 0x67U, 0xE6U, 0xF3U, 0x67U, 0x32U, 0x82U, 0x48U, 0xCCU, 0xCCU, 0x4FU, 0xC2U, 0x14U, 0xE4U, 0x7DU,
    0x05U, 0xD0U, 0xBFU, 0x8CU, 0xC9U, 0x69U, 0xC8U, 0xEDU, 0xDDU, 0x72U, 0x44U, 0xFCU, 0x0BU, 0xD6U, 0x65U, 0x89U,
    0xE8U, 0xF7U, 0x39U, 0x97U, 0xA6U, 0x67U, 0x8DU, 0x47U, 0x30U, 0xA4U, 0x91U, 0x40U, 0x22U, 0x3DU, 0x9AU, 0x93U};

static uint8_t s_MacKatHmacSha512[64U] __attribute__((__aligned__(4))) = {
    0x4DU, 0x16U, 0xC5U, 0x2DU, 0x32U, 0xE8U, 0xD7U, 0xB5U, 0x60U, 0xF6U, 0xC4U, 0x86U, 0x13U, 0xF9U, 0xB0U, 0x72U,
    0xB5U, 0x9EU, 0x25U, 0xE6U, 0xAFU, 0x47U, 0x35U, 0x36U, 0xD0U, 0x3EU, 0x54U, 0x51U, 0x78U, 0xDBU, 0x75U, 0x28U,
    0xB4U, 0x95U, 0xFFU, 0xD9U, 0x22U, 0x09U, 0x09U, 0xD5U, 0x22U, 0x99U, 0xC2U, 0x3FU, 0xC1U, 0x3AU, 0xC9U, 0x8DU,
    0xB6U, 0x4AU, 0x85U, 0x5FU, 0xF8U, 0xDEU, 0xDCU, 0xB6U, 0x07U, 0x26U, 0x54U, 0x71U, 0x31U, 0x99U, 0xFDU, 0xD6U};

/* CMAC variables */
static uint8_t s_KeyCmac128[16U] = {0x0EU, 0x80U, 0xFAU, 0x88U, 0x9BU, 0x1DU, 0x96U, 0xA0U,
                                    0xD2U, 0x3DU, 0x23U, 0x6DU, 0x4DU, 0x64U, 0x2AU, 0x27U};

static uint8_t s_PlaintextCmac128[32U] = {0xF6U, 0xF0U, 0x94U, 0xE4U, 0x6CU, 0xDBU, 0x2EU, 0x45U, 0xFEU, 0x49U, 0xB1U,
                                          0x8AU, 0xFFU, 0x14U, 0x27U, 0xEBU, 0xDAU, 0xC9U, 0x71U, 0x0FU, 0xA7U, 0xF4U,
                                          0x7FU, 0x75U, 0xFCU, 0x9EU, 0xC7U, 0x14U, 0x06U, 0x13U, 0xEFU, 0x3EU};

static uint8_t s_MacKatCmac128[8U] = {0xA0U, 0x97U, 0x74U, 0x00U, 0x99U, 0x34U, 0xC9U, 0xD4U};

static uint8_t s_KeyCmac256[32U] = {0xD7U, 0x1CU, 0x50U, 0xD5U, 0x58U, 0x50U, 0xD4U, 0x32U, 0xCDU, 0x8CU, 0x8FU,
                                    0xF4U, 0xEAU, 0x42U, 0x7BU, 0x3FU, 0x19U, 0xCBU, 0xE1U, 0x4CU, 0x78U, 0x5AU,
                                    0x77U, 0x04U, 0x20U, 0x2FU, 0xCBU, 0xCEU, 0xADU, 0x0DU, 0xE5U, 0xA5U};

static uint8_t s_PlaintextCmac256[32U] = {0x7FU, 0xFBU, 0xC4U, 0xA0U, 0x95U, 0x83U, 0x02U, 0x9CU, 0xBBU, 0x0AU, 0xCBU,
                                          0x6BU, 0x13U, 0xF0U, 0x8AU, 0x18U, 0x90U, 0x33U, 0xDAU, 0x22U, 0xC2U, 0xECU,
                                          0xF9U, 0x21U, 0xF0U, 0x1DU, 0x79U, 0xACU, 0x68U, 0xA9U, 0x39U, 0x7BU};

static uint8_t s_MacKatCmac256[8U] = {0x5DU, 0x00U, 0xFFU, 0xC5U, 0xF8U, 0xCFU, 0x1DU, 0xDDU};
/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Execute HMAC on ELS.
 */
static status_t hmac_sha_els(const uint8_t *plain_key,
                             const uint32_t key_size,
                             const uint8_t *plain_text,
                             const uint32_t plain_size,
                             const uint8_t *mac)
{
    uint8_t result_buffer[32U] = {0U};
    mcuxClEls_HmacOption_t options;
    options.bits.extkey = MCUXCLELS_HMAC_EXTERNAL_KEY_ENABLE;

    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(
        result, token, mcuxClEls_Hmac_Async(options, 0U, plain_key, plain_text, plain_size, result_buffer));

    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_Hmac_Async) != token) || (MCUXCLELS_STATUS_OK_WAIT != result))
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
    if (!assert_equal(result_buffer, mac, 32U))
    {
        return STATUS_ERROR_GENERIC;
    }

    return STATUS_SUCCESS;
}

/*!
 * @brief Execute HMAC.
 */
static status_t hmac_sha(uint8_t *plain_key,
                         const uint8_t *plain_text,
                         const uint8_t *mac,
                         const uint32_t mac_size,
                         mcuxClHash_Algo_t hash_algorithm)
{
    /* Key buffer for the key in memory. */
    uint32_t key_buffer[142U];

    /* Allocate and initialize session / workarea */
    mcuxClSession_Descriptor_t session_desc;
    mcuxClSession_Handle_t session = &session_desc;

    ALLOCATE_AND_INITIALIZE_SESSION(session, MCUXCLHMAC_MAX_CPU_WA_BUFFER_SIZE, 0U);

    /* Initialize the PRNG */
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(prngInit_result, prngInit_token, mcuxClRandom_ncInit(session));
    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandom_ncInit) != prngInit_token) ||
        (MCUXCLRANDOM_STATUS_OK != prngInit_result))
    {
        return STATUS_ERROR_GENERIC;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    /* Create and initialize mcuxClKey_Descriptor_t structure. */
    uint32_t key_desc[MCUXCLKEY_DESCRIPTOR_SIZE_IN_WORDS];
    mcuxClKey_Handle_t key = (mcuxClKey_Handle_t)&key_desc;

    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(
        keyInit_result, keyInit_token,
        mcuxClKey_init(
            /* mcuxClSession_Handle_t pSession:                */ session,
            /* mcuxClKey_Handle_t key:                         */ key,
            /* const mcuxClKey_Type* type:                     */ mcuxClKey_Type_Hmac_variableLength,
            /* mcuxCl_Buffer_t pKeyData:                       */ plain_key,
            /* uint32_t keyDataLength:                         */ 50U));

    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClKey_init) != keyInit_token) || (MCUXCLKEY_STATUS_OK != keyInit_result))
    {
        return STATUS_ERROR_GENERIC;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    /* Load key to memory. */
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(keyLoad_result, keyLoad_token, mcuxClKey_loadMemory(session, key, key_buffer));

    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClKey_loadMemory) != keyLoad_token) ||
        (MCUXCLKEY_STATUS_OK != keyLoad_result))
    {
        return STATUS_ERROR_GENERIC;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    uint8_t hmac_mode_desc_buffer[MCUXCLHMAC_HMAC_MODE_DESCRIPTOR_SIZE];
    mcuxClMac_CustomMode_t mode = (mcuxClMac_CustomMode_t)hmac_mode_desc_buffer;

    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(hashCreateMode_result, hashCreateMode_token,
                                     mcuxClHmac_createHmacMode(
                                         /* mcuxClMac_CustomMode_t mode:       */ mode,
                                         /* mcuxClHash_Algo_t hashAlgorithm:   */ hash_algorithm));

    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClHmac_createHmacMode) != hashCreateMode_token) ||
        (MCUXCLMAC_STATUS_OK != hashCreateMode_result))
    {
        return STATUS_ERROR_GENERIC;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    uint8_t result_buffer[128U] = {0U};

    /* Call the mcuxClMac_compute function to compute a HMAC in one shot. */
    uint32_t result_size = 0U;
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(macCompute_result, macCompute_token,
                                     mcuxClMac_compute(
                                         /* mcuxClSession_Handle_t session:  */ session,
                                         /* const mcuxClKey_Handle_t key:    */ key,
                                         /* const mcuxClMac_Mode_t mode:     */ mode,
                                         /* mcuxCl_InputBuffer_t pIn:        */ plain_text,
                                         /* uint32_t inLength:               */ 128U,
                                         /* mcuxCl_Buffer_t pMac:            */ result_buffer,
                                         /* uint32_t * const pMacLength:     */ &result_size));
    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClMac_compute) != macCompute_token) ||
        (MCUXCLMAC_STATUS_OK != macCompute_result))
    {
        return STATUS_ERROR_GENERIC;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    if (!assert_equal(result_buffer, mac, mac_size))
    {
        return STATUS_ERROR_GENERIC;
    }

    /* Flush the key. */
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(keyFlush_result, keyFlush_token, mcuxClKey_flush(session, key));
    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClKey_flush) != keyFlush_token) || (MCUXCLKEY_STATUS_OK != keyFlush_result))
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
 * @brief Execute AES-CMAC.
 */
static status_t aes_cmac(mcuxClKey_Type_t key_type,
                         uint8_t *plain_key,
                         const uint8_t *plain_text,
                         const uint8_t *mac,
                         const uint32_t mac_size)
{
    /* Key buffer for the key in memory. */
    uint32_t key_buffer[32U];

    mcuxClSession_Descriptor_t session_desc;
    mcuxClSession_Handle_t session = &session_desc;

    ALLOCATE_AND_INITIALIZE_SESSION(session, MCUXCLMAC_MAX_CPU_WA_BUFFER_SIZE, 0U);

    /* Initialize the PRNG */
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(prngInit_result, prngInit_token, mcuxClRandom_ncInit(session));
    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandom_ncInit) != prngInit_token) ||
        (MCUXCLRANDOM_STATUS_OK != prngInit_result))
    {
        return STATUS_ERROR_GENERIC;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    /* Create and initialize mcuxClKey_Descriptor_t structure. */
    uint32_t key_desc[MCUXCLKEY_DESCRIPTOR_SIZE_IN_WORDS];
    mcuxClKey_Handle_t key = (mcuxClKey_Handle_t)&key_desc;

    mcuxClEls_KeyProp_t cmac_key_properties;
    cmac_key_properties.word.value = 0U;
    cmac_key_properties.bits.ucmac = (uint32_t)MCUXCLELS_KEYPROPERTY_CMAC_TRUE;
    cmac_key_properties.bits.kactv = (uint32_t)MCUXCLELS_KEYPROPERTY_ACTIVE_TRUE;

    /* Key init and load */
    if (key_type == mcuxClKey_Type_Aes128)
    {
        if (cl_key_init_and_load(session, key, key_type, (mcuxCl_Buffer_t)plain_key, 16U, &cmac_key_properties,
                                 key_buffer, 0U) != STATUS_SUCCESS)
        {
            return STATUS_ERROR_GENERIC;
        }
    }
    else
    {
        if (cl_key_init_and_load(session, key, key_type, (mcuxCl_Buffer_t)plain_key, 32U, &cmac_key_properties,
                                 key_buffer, 0U) != STATUS_SUCCESS)
        {
            return STATUS_ERROR_GENERIC;
        }
    }

    uint32_t result_size       = 0U;
    uint8_t result_buffer[64U] = {0U};

    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token,
                                     mcuxClMac_compute(
                                         /* mcuxClSession_Handle_t session:  */ session,
                                         /* const mcuxClKey_Handle_t key:    */ key,
                                         /* const mcuxClMac_Mode_t mode:     */ mcuxClMac_Mode_CMAC,
                                         /* mcuxCl_InputBuffer_t pIn:        */ plain_text,
                                         /* uint32_t inLength:               */ 32U,
                                         /* mcuxCl_Buffer_t pMac:            */ result_buffer,
                                         /* uint32_t * const pMacLength:     */ &result_size));
    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClMac_compute) != token) || (MCUXCLMAC_STATUS_OK != result))
    {
        return STATUS_ERROR_GENERIC;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    if (!assert_equal(result_buffer, mac, mac_size))
    {
        return STATUS_ERROR_GENERIC;
    }

    /* Flush the key */
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClKey_flush(session, key));

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

void execute_hmac_kat(uint64_t options, char name[])
{
    /* Execute HMAC SHA224 */
    if ((bool)(options & FIPS_HMAC_SHA224))
    {
        CHECK_STATUS_AND_LOG(hmac_sha(s_KeyHmacSha224, s_MsgHmacSha224, s_MacKatHmacSha224, sizeof(s_MacKatHmacSha224),
                                      mcuxClHash_Algorithm_Sha224),
                             name, "KAT");
    }
    /* Execute HMAC SHA256 */
    if ((bool)(options & FIPS_HMAC_SHA256))
    {
        CHECK_STATUS_AND_LOG(hmac_sha_els(s_KeyHmacSha256, sizeof(s_KeyHmacSha256), s_MsgHmacSha256,
                                          sizeof(s_MsgHmacSha256), s_MacKatHmacSha256),
                             name, "KAT");
    }
    /* Execute HMAC SHA384 */
    if ((bool)(options & FIPS_HMAC_SHA384))
    {
        CHECK_STATUS_AND_LOG(hmac_sha(s_KeyHmacSha384, s_MsgHmacSha384, s_MacKatHmacSha384, sizeof(s_MacKatHmacSha384),
                                      mcuxClHash_Algorithm_Sha384),
                             name, "KAT");
    }
    /* Execute HMAC SHA512 */
    if ((bool)(options & FIPS_HMAC_SHA512))
    {
        CHECK_STATUS_AND_LOG(hmac_sha(s_KeyHmacSha512, s_MsgHmacSha512, s_MacKatHmacSha512, sizeof(s_MacKatHmacSha512),
                                      mcuxClHash_Algorithm_Sha512),
                             name, "KAT");
    }
}

void execute_cmac_kat(uint64_t options, char name[])
{
    /* Execute CMAC-128
     * Note: No KAT for keysize 192, since no implementation existing.
     */
    if ((bool)(options & FIPS_AES_CMAC_128))
    {
        CHECK_STATUS_AND_LOG(
            aes_cmac(mcuxClKey_Type_Aes128, s_KeyCmac128, s_PlaintextCmac128, s_MacKatCmac128, sizeof(s_MacKatCmac128)),
            name, "KAT");
    }
    /* Execute CMAC-256 */
    if ((bool)(options & FIPS_AES_CMAC_256))
    {
        CHECK_STATUS_AND_LOG(
            aes_cmac(mcuxClKey_Type_Aes256, s_KeyCmac256, s_PlaintextCmac256, s_MacKatCmac256, sizeof(s_MacKatCmac256)),
            name, "KAT");
    }
}
