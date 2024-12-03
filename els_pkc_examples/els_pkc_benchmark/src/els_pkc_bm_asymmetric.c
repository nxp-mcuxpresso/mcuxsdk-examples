/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDx-License-Identifier: BSD-3-Clause
 */

#include "els_pkc_bm_asymmetric.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define RAM_START_ADDRESS MCUXCLPKC_RAM_START_ADDRESS
#define MAX_CPUWA_SIZE                                                 \
    MCUXCLCORE_MAX(MCUXCLECC_EDDSA_GENERATEKEYPAIR_ED25519_WACPU_SIZE, \
                   MCUXCLECC_EDDSA_GENERATESIGNATURE_ED25519_WACPU_SIZE)
#define MAX_PKCWA_SIZE                                                 \
    MCUXCLCORE_MAX(MCUXCLECC_EDDSA_GENERATEKEYPAIR_ED25519_WAPKC_SIZE, \
                   MCUXCLECC_EDDSA_GENERATESIGNATURE_ED25519_WAPKC_SIZE)
#define MESSAGE_SMALL 64U
#define MESSAGE_LARGE 2048U

#define RSA_KEY_BIT_LENGTH        (2048U)                   /* The example uses a 2048-bit key */
#define RSA_KEY_BYTE_LENGTH       (RSA_KEY_BIT_LENGTH / 8U) /* Converting the key-bitlength to bytelength */
#define RSA_PSS_SALT_LENGTH       (0U)                      /* The salt length is set to 0 in this example */
#define RSA_MESSAGE_DIGEST_LENGTH (32U) /* The example uses a Sha2-256 digest, which is 32 bytes long */

#define WEIER256_BIT_LENGTH (256U)
#define WEIER384_BIT_LENGTH (384U)
#define WEIER521_BIT_LENGTH (521U)

#define MCUXCLECC_STATUS_POINTMULT_INVALID_PARAMS MCUXCLECC_STATUS_INVALID_PARAMS
#define MCUXCLECC_STATUS_SIGN_INVALID_PARAMS      MCUXCLECC_STATUS_INVALID_PARAMS
#define MCUXCLECC_STATUS_SIGN_RNG_ERROR           MCUXCLECC_STATUS_RNG_ERROR
#define MCUXCLECC_STATUS_SIGN_OK                  MCUXCLECC_STATUS_OK
#define MCUXCLECC_STATUS_VERIFY_OK                MCUXCLECC_STATUS_OK
#define MCUXCLECC_STATUS_POINTMULT_OK             MCUXCLECC_STATUS_OK
#define MCUX_PKC_MIN(a, b)                        ((a) < (b) ? (a) : (b))

#define GENERATE_RSA_SIGNATURE(data_from_ram, session, private_key, m_length)                                          \
    do                                                                                                                 \
    {                                                                                                                  \
        if (data_from_ram)                                                                                             \
        {                                                                                                              \
            MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(                                                                          \
                sign_result, sign_token,                                                                               \
                mcuxClRsa_sign(session, &private_key, m_length == 32U ? s_MessageDigest32Byte : s_MessageDigest64Byte, \
                               m_length == 32U ? sizeof(s_MessageDigest32Byte) : sizeof(s_MessageDigest64Byte),        \
                               m_length == 32U ? (mcuxClRsa_SignVerifyMode_t *)&mcuxClRsa_Mode_Sign_Pss_Sha2_256 :     \
                                                 (mcuxClRsa_SignVerifyMode_t *)&mcuxClRsa_Mode_Sign_Pss_Sha2_512,      \
                               RSA_PSS_SALT_LENGTH, MCUXCLRSA_OPTION_MESSAGE_DIGEST, s_SignatureBuffer));              \
            if (MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRsa_sign) != sign_token || MCUXCLRSA_STATUS_SIGN_OK != sign_result) \
            {                                                                                                          \
                PRINTF("[Error] RSA signature generation failed\r\n");                                                 \
                return MCUXCLEXAMPLE_STATUS_ERROR;                                                                     \
            }                                                                                                          \
            MCUX_CSSL_FP_FUNCTION_CALL_END();                                                                          \
        }                                                                                                              \
        else                                                                                                           \
        {                                                                                                              \
            MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(                                                                          \
                sign_result, sign_token,                                                                               \
                mcuxClRsa_sign(                                                                                        \
                    session, &private_key, m_length == 32U ? s_MessageDigest32ByteFlash : s_MessageDigest64ByteFlash,  \
                    m_length == 32U ? sizeof(s_MessageDigest32ByteFlash) : sizeof(s_MessageDigest64ByteFlash),         \
                    m_length == 32U ? (mcuxClRsa_SignVerifyMode_t *)&mcuxClRsa_Mode_Sign_Pss_Sha2_256 :                \
                                      (mcuxClRsa_SignVerifyMode_t *)&mcuxClRsa_Mode_Sign_Pss_Sha2_512,                 \
                    RSA_PSS_SALT_LENGTH, MCUXCLRSA_OPTION_MESSAGE_DIGEST, s_SignatureBuffer));                         \
            if (MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRsa_sign) != sign_token || MCUXCLRSA_STATUS_SIGN_OK != sign_result) \
            {                                                                                                          \
                PRINTF("[Error] RSA signature generation failed\r\n");                                                 \
                return MCUXCLEXAMPLE_STATUS_ERROR;                                                                     \
            }                                                                                                          \
            MCUX_CSSL_FP_FUNCTION_CALL_END();                                                                          \
        }                                                                                                              \
    } while (0);

#define RSA_VERIFY(data_from_ram, session, public_key, m_length)                                                       \
    do                                                                                                                 \
    {                                                                                                                  \
        uint8_t encodedMessage[32];                                                                                    \
        if (data_from_ram)                                                                                             \
        {                                                                                                              \
            MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(                                                                          \
                verify_result, verify_token,                                                                           \
                mcuxClRsa_verify(session, &public_key,                                                                 \
                                 m_length == 32U ? s_MessageDigest32Byte : s_MessageDigest64Byte,                      \
                                 m_length == 32U ? sizeof(s_MessageDigest32Byte) : sizeof(s_MessageDigest64Byte),      \
                                 s_SignatureBuffer,                                                                    \
                                 m_length == 32U ? (mcuxClRsa_SignVerifyMode_t *)&mcuxClRsa_Mode_Verify_Pss_Sha2_256 : \
                                                   (mcuxClRsa_SignVerifyMode_t *)&mcuxClRsa_Mode_Verify_Pss_Sha2_512,  \
                                 RSA_PSS_SALT_LENGTH, MCUXCLRSA_OPTION_MESSAGE_DIGEST, encodedMessage));               \
            if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRsa_verify) != verify_token) ||                                    \
                (MCUXCLRSA_STATUS_VERIFY_OK != verify_result))                                                         \
            {                                                                                                          \
                PRINTF("[Error] RSA signature verification failed\r\n");                                               \
                return MCUXCLEXAMPLE_STATUS_ERROR;                                                                     \
            }                                                                                                          \
            MCUX_CSSL_FP_FUNCTION_CALL_END();                                                                          \
        }                                                                                                              \
        else                                                                                                           \
        {                                                                                                              \
            MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(                                                                          \
                verify_result, verify_token,                                                                           \
                mcuxClRsa_verify(                                                                                      \
                    session, &public_key, m_length == 32U ? s_MessageDigest32ByteFlash : s_MessageDigest64ByteFlash,   \
                    m_length == 32U ? sizeof(s_MessageDigest32ByteFlash) : sizeof(s_MessageDigest64ByteFlash),         \
                    s_SignatureBuffer,                                                                                 \
                    m_length == 32U ? (mcuxClRsa_SignVerifyMode_t *)&mcuxClRsa_Mode_Verify_Pss_Sha2_256 :              \
                                      (mcuxClRsa_SignVerifyMode_t *)&mcuxClRsa_Mode_Verify_Pss_Sha2_512,               \
                    RSA_PSS_SALT_LENGTH, MCUXCLRSA_OPTION_MESSAGE_DIGEST, NULL));                                      \
            if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRsa_verify) != verify_token) ||                                    \
                (MCUXCLRSA_STATUS_VERIFY_OK != verify_result))                                                         \
            {                                                                                                          \
                PRINTF("[Error] RSA signature verification failed\r\n");                                               \
                return MCUXCLEXAMPLE_STATUS_ERROR;                                                                     \
            }                                                                                                          \
            MCUX_CSSL_FP_FUNCTION_CALL_END();                                                                          \
        }                                                                                                              \
    } while (0);

#define GENERATE_ECC_ED25519_SIGNATURE(data_from_ram, session, privKey, m_length)                                    \
    do                                                                                                               \
    {                                                                                                                \
        if (data_from_ram)                                                                                           \
        {                                                                                                            \
            uint32_t signatureSize = 0U;                                                                             \
            MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(                                                                        \
                sign_result, sign_token,                                                                             \
                mcuxClEcc_EdDSA_GenerateSignature(                                                                   \
                    &session, privKey, &mcuxClEcc_EdDsa_Ed25519ProtocolDescriptor,                                   \
                    m_length == MESSAGE_SMALL ? s_MessageSmallEccEd25519 : s_MessageLargeEccEd25519,                 \
                    m_length == MESSAGE_SMALL ? sizeof(s_MessageSmallEccEd25519) : sizeof(s_MessageLargeEccEd25519), \
                    s_SignatureBuffer, &signatureSize));                                                             \
            if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEcc_EdDSA_GenerateSignature) != sign_token) ||                   \
                (MCUXCLECC_EDDSA_ED25519_SIZE_SIGNATURE != signatureSize) || (MCUXCLECC_STATUS_OK != sign_result))   \
            {                                                                                                        \
                PRINTF("[Error] ECC signature generation failed\r\n");                                               \
                return MCUXCLEXAMPLE_STATUS_ERROR;                                                                   \
            }                                                                                                        \
            MCUX_CSSL_FP_FUNCTION_CALL_END();                                                                        \
        }                                                                                                            \
        else                                                                                                         \
        {                                                                                                            \
            uint32_t signatureSize = 0U;                                                                             \
            MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(                                                                        \
                sign_result, sign_token,                                                                             \
                mcuxClEcc_EdDSA_GenerateSignature(                                                                   \
                    &session, privKey, &mcuxClEcc_EdDsa_Ed25519ProtocolDescriptor,                                   \
                    m_length == MESSAGE_SMALL ? s_MessageSmallEccEd25519Flash : s_MessageLargeEccEd25519Flash,       \
                    m_length == MESSAGE_SMALL ? sizeof(s_MessageSmallEccEd25519Flash) :                              \
                                                sizeof(s_MessageLargeEccEd25519Flash),                               \
                    s_SignatureBuffer, &signatureSize));                                                             \
            if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEcc_EdDSA_GenerateSignature) != sign_token) ||                   \
                (MCUXCLECC_EDDSA_ED25519_SIZE_SIGNATURE != signatureSize) || (MCUXCLECC_STATUS_OK != sign_result))   \
            {                                                                                                        \
                PRINTF("[Error] ECC signature generation failed\r\n");                                               \
                return MCUXCLEXAMPLE_STATUS_ERROR;                                                                   \
            }                                                                                                        \
            MCUX_CSSL_FP_FUNCTION_CALL_END();                                                                        \
        }                                                                                                            \
    } while (0);

#define ECC_ED25519_VERIFY(data_from_ram, session, pubKeyHandler, m_length)                                          \
    do                                                                                                               \
    {                                                                                                                \
        if (data_from_ram)                                                                                           \
        {                                                                                                            \
            MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(                                                                        \
                verify_result, verify_token,                                                                         \
                mcuxClEcc_EdDSA_VerifySignature(                                                                     \
                    &session, pubKeyHandler, &mcuxClEcc_EdDsa_Ed25519ProtocolDescriptor,                             \
                    m_length == MESSAGE_SMALL ? s_MessageSmallEccEd25519 : s_MessageLargeEccEd25519,                 \
                    m_length == MESSAGE_SMALL ? sizeof(s_MessageSmallEccEd25519) : sizeof(s_MessageLargeEccEd25519), \
                    s_SignatureBuffer, 64U));                                                                        \
            if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEcc_EdDSA_VerifySignature) != verify_token) ||                   \
                (MCUXCLECC_STATUS_OK != verify_result))                                                              \
            {                                                                                                        \
                PRINTF("[Error] ECC signature verification failed\r\n");                                             \
                return MCUXCLEXAMPLE_STATUS_ERROR;                                                                   \
            }                                                                                                        \
            MCUX_CSSL_FP_FUNCTION_CALL_END();                                                                        \
        }                                                                                                            \
        else                                                                                                         \
        {                                                                                                            \
            MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(                                                                        \
                verify_result, verify_token,                                                                         \
                mcuxClEcc_EdDSA_VerifySignature(                                                                     \
                    &session, pubKeyHandler, &mcuxClEcc_EdDsa_Ed25519ProtocolDescriptor,                             \
                    m_length == MESSAGE_SMALL ? s_MessageSmallEccEd25519Flash : s_MessageLargeEccEd25519Flash,       \
                    m_length == MESSAGE_SMALL ? sizeof(s_MessageSmallEccEd25519Flash) :                              \
                                                sizeof(s_MessageLargeEccEd25519Flash),                               \
                    s_SignatureBuffer, 64U));                                                                        \
            if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEcc_EdDSA_VerifySignature) != verify_token) ||                   \
                (MCUXCLECC_STATUS_OK != verify_result))                                                              \
            {                                                                                                        \
                PRINTF("[Error] ECC signature verification failed\r\n");                                             \
                return MCUXCLEXAMPLE_STATUS_ERROR;                                                                   \
            }                                                                                                        \
            MCUX_CSSL_FP_FUNCTION_CALL_END();                                                                        \
        }                                                                                                            \
    } while (0);

/******************************************************************************* \
 * Prototypes                                                                    \
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* Buffer for generated signature */
static uint8_t s_SignatureBuffer[128U];

/* Buffer for generated signature */
static uint8_t s_SignatureBufferWeier[2U * 66U];

/* Buffer for generated public key in ECC-Ed25519 */
static uint8_t s_PublicKeyBufferEcc[MCUXCLECC_EDDSA_ED25519_SIZE_PUBLICKEY] __attribute__((aligned(4U)));

/* Variables stored in RAM */

/* Private key input for ECC-Ed25519 */
static uint8_t s_PrivKeyInputEccEd25519[32U] = {
    0x83U, 0x3FU, 0xE6U, 0x24U, 0x09U, 0x23U, 0x7BU, 0x9DU, 0x62U, 0xECU, 0x77U, 0x58U, 0x75U, 0x20U, 0x91U, 0x1EU,
    0x9AU, 0x75U, 0x9CU, 0xECU, 0x1DU, 0x19U, 0x75U, 0x5BU, 0x7DU, 0xA9U, 0x01U, 0xB9U, 0x6DU, 0xCAU, 0x3DU, 0x42U};

/* Private key input for ECC-Weier */
static uint8_t s_PrivateKeyInputWeier256[32U] = {
    0xE9U, 0x46U, 0xFFU, 0x12U, 0xFFU, 0xB2U, 0xE7U, 0xBAU, 0x2CU, 0x5DU, 0x3AU, 0xAFU, 0x7DU, 0x9AU, 0xEEU, 0xE2U,
    0x00U, 0x59U, 0x7AU, 0xABU, 0x20U, 0xCAU, 0xB0U, 0xF9U, 0x6BU, 0xD4U, 0x84U, 0x75U, 0x3DU, 0x78U, 0xFEU, 0xF4U};

static uint8_t s_PrivateKeyInputWeier384[48U] = {
    0x8EU, 0x49U, 0xBFU, 0x1CU, 0x5DU, 0x9CU, 0xBEU, 0x73U, 0xD5U, 0xD3U, 0xDCU, 0xD7U, 0xBBU, 0x57U, 0x6AU, 0x2BU,
    0xDEU, 0x17U, 0xB1U, 0xAAU, 0xA7U, 0xCCU, 0x31U, 0xD0U, 0x24U, 0x10U, 0xB0U, 0xE6U, 0x9FU, 0xF7U, 0x42U, 0x4BU,
    0xA6U, 0x58U, 0x87U, 0x41U, 0x6AU, 0x04U, 0x14U, 0x43U, 0x4CU, 0x25U, 0x5CU, 0xECU, 0x9DU, 0x84U, 0x36U, 0x88U};

static uint8_t s_PrivateKeyInputWeier521[66U] = {
    0x00U, 0xA8U, 0x14U, 0x1AU, 0xE2U, 0xF5U, 0x5FU, 0xFCU, 0x6EU, 0x4AU, 0x39U, 0xF2U, 0x0FU, 0x3DU,
    0x53U, 0x47U, 0x19U, 0xB0U, 0x6BU, 0x32U, 0xC7U, 0xBDU, 0xEAU, 0x46U, 0x40U, 0x58U, 0xE2U, 0xC6U,
    0x73U, 0xD4U, 0xE2U, 0x35U, 0x73U, 0x8FU, 0x0FU, 0x49U, 0x08U, 0x2AU, 0x8FU, 0xE7U, 0xAAU, 0x47U,
    0x1DU, 0x2AU, 0x73U, 0x61U, 0xCAU, 0x2CU, 0xF7U, 0x60U, 0x6EU, 0x85U, 0xDBU, 0xD7U, 0x03U, 0xBEU,
    0xA6U, 0x3FU, 0xB3U, 0xCDU, 0x8CU, 0x78U, 0x72U, 0xA9U, 0x4BU, 0x20U};

/* Public key input for ECC-Weier */
static uint8_t s_PublicKeyInputWeier256[64U] = {
    0x52U, 0x03U, 0x46U, 0xA7U, 0x4AU, 0x71U, 0xE0U, 0x4DU, 0x39U, 0xFEU, 0x4BU, 0x20U, 0x1BU, 0xF7U, 0x4CU, 0x92U,
    0xB6U, 0xBEU, 0x9FU, 0x88U, 0x11U, 0x1EU, 0x7CU, 0x31U, 0x63U, 0x13U, 0xB3U, 0xFCU, 0x94U, 0x85U, 0xDAU, 0xD9U,
    0x70U, 0x7AU, 0xBDU, 0x51U, 0x8EU, 0x51U, 0xC2U, 0xD6U, 0x56U, 0x54U, 0xC4U, 0xD9U, 0x86U, 0xE7U, 0x76U, 0x9FU,
    0x4EU, 0xA1U, 0xD9U, 0x37U, 0x39U, 0xF7U, 0xC3U, 0xABU, 0x73U, 0x89U, 0xBDU, 0x30U, 0x03U, 0x17U, 0x9BU, 0xD9U};

static uint8_t s_PublicKeyInputWeier384[96U] = {
    0x89U, 0xF1U, 0xB7U, 0x32U, 0x2DU, 0x68U, 0xEFU, 0x8AU, 0x73U, 0x17U, 0xB2U, 0x98U, 0x72U, 0xF0U, 0xE1U, 0x10U,
    0x8AU, 0xFFU, 0xF7U, 0x19U, 0x53U, 0x83U, 0x79U, 0x4AU, 0x1CU, 0x94U, 0x08U, 0xA2U, 0x16U, 0xE6U, 0x18U, 0x0AU,
    0xF3U, 0xC3U, 0x7FU, 0x69U, 0x6AU, 0xE8U, 0xCBU, 0xF0U, 0x34U, 0x8DU, 0x14U, 0x8AU, 0x9AU, 0x22U, 0x75U, 0x1DU,
    0x57U, 0x39U, 0x14U, 0x3EU, 0xE8U, 0xAFU, 0xB6U, 0x51U, 0x35U, 0x83U, 0x6CU, 0xBDU, 0x35U, 0x97U, 0x4DU, 0x67U,
    0x53U, 0xB7U, 0x12U, 0x7DU, 0xAAU, 0xDDU, 0xB2U, 0xEEU, 0x0AU, 0x60U, 0x39U, 0xFBU, 0xF0U, 0xE5U, 0x77U, 0x8CU,
    0x76U, 0xD0U, 0x6CU, 0x28U, 0xBBU, 0x66U, 0xEAU, 0xA9U, 0x4EU, 0xA3U, 0x14U, 0x6BU, 0x53U, 0xA6U, 0xA6U, 0x22U};

static uint8_t s_PublicKeyInputWeier521[132U] = {
    0x00U, 0x4BU, 0x29U, 0xF5U, 0xEFU, 0x68U, 0xBBU, 0x53U, 0x47U, 0xA5U, 0x4AU, 0x76U, 0x6AU, 0x09U, 0x80U,
    0xD6U, 0x1FU, 0x45U, 0xA1U, 0x90U, 0xD8U, 0xBBU, 0x4EU, 0xFDU, 0x88U, 0x90U, 0x5FU, 0xA6U, 0xABU, 0x6AU,
    0x6DU, 0x6BU, 0x5EU, 0xFAU, 0x5BU, 0x3EU, 0xB4U, 0xBCU, 0x4CU, 0xB4U, 0x98U, 0x6BU, 0xF0U, 0xB5U, 0x99U,
    0xACU, 0xB1U, 0xAAU, 0xD8U, 0x62U, 0xADU, 0xE0U, 0xCAU, 0x7AU, 0x22U, 0x4AU, 0xE0U, 0xC5U, 0xAEU, 0x6DU,
    0x6EU, 0x9EU, 0x97U, 0x88U, 0xDDU, 0xA0U, 0x01U, 0x01U, 0x08U, 0x21U, 0x53U, 0x9BU, 0xDAU, 0x45U, 0x0FU,
    0xCBU, 0x07U, 0x93U, 0x8EU, 0xFCU, 0x8EU, 0xE5U, 0x56U, 0xF8U, 0x8AU, 0xE0U, 0xC8U, 0x06U, 0xA8U, 0x7CU,
    0xD2U, 0x1AU, 0x1EU, 0x82U, 0x8EU, 0x3AU, 0xECU, 0x00U, 0x5EU, 0x0DU, 0x90U, 0x5FU, 0x13U, 0xF5U, 0x50U,
    0xE1U, 0xA1U, 0x95U, 0x6DU, 0x76U, 0x80U, 0xEEU, 0x9AU, 0xC5U, 0x88U, 0xBEU, 0x42U, 0x85U, 0x5CU, 0x15U,
    0xDDU, 0xCBU, 0x97U, 0xA9U, 0xFAU, 0x1BU, 0x24U, 0x91U, 0x98U, 0xA5U, 0x49U, 0x8EU};

/* Small input message */
static uint8_t s_MessageSmallEccEd25519[MESSAGE_SMALL] __attribute__((aligned(4U))) = {
    0xDDU, 0xAFU, 0x35U, 0xA1U, 0x93U, 0x61U, 0x7AU, 0xBAU, 0xCCU, 0x41U, 0x73U, 0x49U, 0xAEU, 0x20U, 0x41U, 0x31U,
    0x12U, 0xE6U, 0xFAU, 0x4EU, 0x89U, 0xA9U, 0x7EU, 0xA2U, 0x0AU, 0x9EU, 0xEEU, 0xE6U, 0x4BU, 0x55U, 0xD3U, 0x9AU,
    0x21U, 0x92U, 0x99U, 0x2AU, 0x27U, 0x4FU, 0xC1U, 0xA8U, 0x36U, 0xBAU, 0x3CU, 0x23U, 0xA3U, 0xFEU, 0xEBU, 0xBDU,
    0x45U, 0x4DU, 0x44U, 0x23U, 0x64U, 0x3CU, 0xE8U, 0x0EU, 0x2AU, 0x9AU, 0xC9U, 0x4FU, 0xA5U, 0x4CU, 0xA4U, 0x9FU};

/* Larger input message */
static uint8_t s_MessageLargeEccEd25519[MESSAGE_LARGE] __attribute__((aligned(4U)));

/* Example value for private RSA exponent d */
static uint8_t s_ExponentDRSA[RSA_KEY_BYTE_LENGTH] __attribute__((aligned(4U))) = {
    0x15U, 0x5FU, 0xE6U, 0x60U, 0xCDU, 0xDEU, 0xAAU, 0x17U, 0x1BU, 0x5EU, 0xD6U, 0xBDU, 0xD0U, 0x3BU, 0xB3U, 0x56U,
    0xE0U, 0xF6U, 0xE8U, 0x6BU, 0x5AU, 0x3CU, 0x26U, 0xF3U, 0xCEU, 0x7DU, 0xAEU, 0x00U, 0x8CU, 0x4EU, 0x38U, 0xA9U,
    0xA9U, 0x7FU, 0xA5U, 0x97U, 0xB2U, 0xB9U, 0x0AU, 0x45U, 0x10U, 0xD2U, 0x23U, 0x8DU, 0x3FU, 0x15U, 0x8AU, 0xB8U,
    0x91U, 0x97U, 0xFBU, 0x08U, 0xA5U, 0xB7U, 0x4CU, 0xFEU, 0x5CU, 0xC8U, 0xF1U, 0x3DU, 0x47U, 0x09U, 0x62U, 0x91U,
    0xD0U, 0x05U, 0x38U, 0xAAU, 0x58U, 0x93U, 0xD8U, 0x2DU, 0xCEU, 0x55U, 0xB3U, 0x64U, 0x8CU, 0x6AU, 0x71U, 0x9AU,
    0xE3U, 0x87U, 0xDEU, 0xE5U, 0x5EU, 0xC5U, 0xBEU, 0xF0U, 0x89U, 0x76U, 0x3DU, 0xE7U, 0x1EU, 0x47U, 0x61U, 0xB7U,
    0x03U, 0xADU, 0x69U, 0x2EU, 0xD6U, 0x2DU, 0x7CU, 0x1FU, 0x4FU, 0x0FU, 0xF0U, 0x03U, 0xC1U, 0x67U, 0xEBU, 0x62U,
    0xD2U, 0xC6U, 0x79U, 0xCCU, 0x6FU, 0x13U, 0xB9U, 0x87U, 0xA1U, 0x42U, 0xF1U, 0x37U, 0x7AU, 0x40U, 0xBDU, 0xC0U,
    0xA0U, 0x36U, 0x60U, 0x72U, 0x94U, 0x40U, 0x14U, 0x63U, 0xA3U, 0x0EU, 0x82U, 0x91U, 0x2BU, 0x42U, 0x8AU, 0x1DU,
    0x3FU, 0x80U, 0xB5U, 0xD0U, 0xD3U, 0x3EU, 0xA8U, 0x4EU, 0x8BU, 0xB6U, 0x4CU, 0x36U, 0x22U, 0xB9U, 0xBEU, 0xE3U,
    0x56U, 0xF1U, 0x2CU, 0x6AU, 0x19U, 0x0EU, 0x55U, 0x7BU, 0xBFU, 0x25U, 0xE1U, 0x10U, 0x80U, 0x7BU, 0x85U, 0xCAU,
    0xD5U, 0x1BU, 0x39U, 0x87U, 0x57U, 0x08U, 0x06U, 0xBEU, 0x81U, 0xF3U, 0x71U, 0x3FU, 0x5DU, 0x17U, 0x40U, 0x74U,
    0x99U, 0xA5U, 0xDEU, 0xDAU, 0xC0U, 0xF3U, 0xE3U, 0xBCU, 0x79U, 0x96U, 0x35U, 0x95U, 0xF8U, 0xE0U, 0xCFU, 0x01U,
    0x29U, 0x1DU, 0xC1U, 0x02U, 0x09U, 0xC0U, 0x6EU, 0xB6U, 0x0EU, 0x2EU, 0x9CU, 0x47U, 0xECU, 0x91U, 0x42U, 0xEDU,
    0xA5U, 0xF3U, 0xB7U, 0x0AU, 0xC6U, 0x7FU, 0x72U, 0xBFU, 0x52U, 0xB3U, 0x31U, 0x37U, 0xD1U, 0x49U, 0xB6U, 0xF6U,
    0x06U, 0xE4U, 0x59U, 0x61U, 0x7DU, 0xAAU, 0x8EU, 0x10U, 0x18U, 0xA8U, 0x14U, 0x1DU, 0x89U, 0x4EU, 0xCAU, 0xFFU};

/* Example value for public RSA exponent e */
static uint8_t s_ExponentERSA[3U] __attribute__((aligned(4))) = {0x01U, 0x00U, 0x01U};

/* Example value for public RSA modulus N */
static uint8_t s_ModulusRSA[RSA_KEY_BYTE_LENGTH] __attribute__((aligned(4U))) = {
    0xD3U, 0x24U, 0x96U, 0xE6U, 0x2DU, 0x16U, 0x34U, 0x6EU, 0x06U, 0xE7U, 0xA3U, 0x1CU, 0x12U, 0x0AU, 0x21U, 0xB5U,
    0x45U, 0x32U, 0x32U, 0x35U, 0xEEU, 0x1DU, 0x90U, 0x72U, 0x1DU, 0xCEU, 0xAAU, 0xD4U, 0x6DU, 0xC4U, 0xCEU, 0xBDU,
    0x80U, 0xC1U, 0x34U, 0x5AU, 0xFFU, 0x95U, 0xB1U, 0xDDU, 0xF8U, 0x71U, 0xEBU, 0xB7U, 0xF2U, 0x0FU, 0xEDU, 0xB6U,
    0xE4U, 0x2EU, 0x67U, 0xA0U, 0xCCU, 0x59U, 0xB3U, 0x9FU, 0xFDU, 0x31U, 0xE9U, 0x83U, 0x42U, 0xF4U, 0x0AU, 0xD9U,
    0xAFU, 0xF9U, 0x3CU, 0x3CU, 0x51U, 0xCFU, 0x5FU, 0x3CU, 0x8AU, 0xD0U, 0x64U, 0xB8U, 0x33U, 0xF9U, 0xACU, 0x34U,
    0x22U, 0x9AU, 0x3EU, 0xD3U, 0xDDU, 0x29U, 0x41U, 0xBEU, 0x12U, 0x5BU, 0xC5U, 0xA2U, 0x0CU, 0xB6U, 0xD2U, 0x31U,
    0xB6U, 0xD1U, 0x84U, 0x7EU, 0xC4U, 0xFEU, 0xAEU, 0x2BU, 0x88U, 0x46U, 0xCFU, 0x00U, 0xC4U, 0xC6U, 0xE7U, 0x5AU,
    0x51U, 0x32U, 0x65U, 0x7AU, 0x68U, 0xECU, 0x04U, 0x38U, 0x36U, 0x46U, 0x34U, 0xEAU, 0xF8U, 0x27U, 0xF9U, 0xBBU,
    0x51U, 0x6CU, 0x93U, 0x27U, 0x48U, 0x1DU, 0x58U, 0xB8U, 0xFFU, 0x1EU, 0xA4U, 0xC0U, 0x1FU, 0xA1U, 0xA2U, 0x57U,
    0xA9U, 0x4EU, 0xA6U, 0xD4U, 0x72U, 0x60U, 0x3BU, 0x3FU, 0xB3U, 0x24U, 0x53U, 0x22U, 0x88U, 0xEAU, 0x3AU, 0x97U,
    0x43U, 0x53U, 0x59U, 0x15U, 0x33U, 0xA0U, 0xEBU, 0xBEU, 0xF2U, 0x9DU, 0xF4U, 0xF8U, 0xBCU, 0x4DU, 0xDBU, 0xF8U,
    0x8EU, 0x47U, 0x1FU, 0x1DU, 0xA5U, 0x00U, 0xB8U, 0xF5U, 0x7BU, 0xB8U, 0xC3U, 0x7CU, 0xA5U, 0xEAU, 0x17U, 0x7CU,
    0x4EU, 0x8AU, 0x39U, 0x06U, 0xB7U, 0xC1U, 0x42U, 0xF7U, 0x78U, 0x8CU, 0x45U, 0xEAU, 0xD0U, 0xC9U, 0xBCU, 0x36U,
    0x92U, 0x48U, 0x3AU, 0xD8U, 0x13U, 0x61U, 0x11U, 0x45U, 0xB4U, 0x1FU, 0x9CU, 0x01U, 0x2EU, 0xF2U, 0x87U, 0xBEU,
    0x8BU, 0xBFU, 0x93U, 0x19U, 0xCFU, 0x4BU, 0x91U, 0x84U, 0xDCU, 0x8EU, 0xFFU, 0x83U, 0x58U, 0x9BU, 0xE9U, 0x0CU,
    0x54U, 0x81U, 0x14U, 0xACU, 0xFAU, 0x5AU, 0xBFU, 0x79U, 0x54U, 0xBFU, 0x9FU, 0x7AU, 0xE5U, 0xB4U, 0x38U, 0xB5U};

/* Example value for Sha2-256 message digest */
static uint8_t s_MessageDigest32Byte[RSA_MESSAGE_DIGEST_LENGTH] __attribute__((aligned(4U))) = {
    0xF4U, 0x45U, 0x80U, 0x1EU, 0x0CU, 0xB8U, 0x99U, 0x26U, 0x2CU, 0x9BU, 0x9EU, 0x21U, 0x98U, 0x36U, 0x88U, 0x0DU,
    0x73U, 0xCAU, 0x2DU, 0x1BU, 0x0BU, 0x9CU, 0x15U, 0xFBU, 0x95U, 0x9CU, 0x90U, 0xEBU, 0x12U, 0x12U, 0x34U, 0xE3U};

/* Example value for Sha2-512 message digest */
static uint8_t s_MessageDigest64Byte[RSA_MESSAGE_DIGEST_LENGTH * 2U] __attribute__((aligned(4U))) = {
    0xF4U, 0x45U, 0x80U, 0x1EU, 0x0CU, 0xB8U, 0x99U, 0x26U, 0x2CU, 0x9BU, 0x9EU, 0x21U, 0x98U, 0x36U, 0x88U, 0x0DU,
    0x73U, 0xCAU, 0x2DU, 0x1BU, 0x0BU, 0x9CU, 0x15U, 0xFBU, 0x95U, 0x9CU, 0x90U, 0xEBU, 0x12U, 0x12U, 0x34U, 0xE3U,
    0xF4U, 0x45U, 0x80U, 0x1EU, 0x0CU, 0xB8U, 0x99U, 0x26U, 0x2CU, 0x9BU, 0x9EU, 0x21U, 0x98U, 0x36U, 0x88U, 0x0DU,
    0x73U, 0xCAU, 0x2DU, 0x1BU, 0x0BU, 0x9CU, 0x15U, 0xFBU, 0x95U, 0x9CU, 0x90U, 0xEBU, 0x12U, 0x12U, 0x34U, 0xE3U};

/* Domain parameters for ECC-Weier */
static uint8_t s_BN_P256_P[WEIER256_BIT_LENGTH / 8U] = {
    0xFFU, 0xFFU, 0xFFU, 0xFFU, 0x00U, 0x00U, 0x00U, 0x01U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,
    0x00U, 0x00U, 0x00U, 0x00U, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU};

static uint8_t s_BN_P256_A[WEIER256_BIT_LENGTH / 8U] = {
    0xFFU, 0xFFU, 0xFFU, 0xFFU, 0x00U, 0x00U, 0x00U, 0x01U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,
    0x00U, 0x00U, 0x00U, 0x00U, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFCU};

static uint8_t s_BN_P256_B[WEIER256_BIT_LENGTH / 8U] = {
    0x5AU, 0xC6U, 0x35U, 0xD8U, 0xAAU, 0x3AU, 0x93U, 0xE7U, 0xB3U, 0xEBU, 0xBDU, 0x55U, 0x76U, 0x98U, 0x86U, 0xBCU,
    0x65U, 0x1DU, 0x06U, 0xB0U, 0xCCU, 0x53U, 0xB0U, 0xF6U, 0x3BU, 0xCEU, 0x3CU, 0x3EU, 0x27U, 0xD2U, 0x60U, 0x4BU};

static uint8_t s_BN_P256_G[2U * WEIER256_BIT_LENGTH / 8U] = {
    0x6BU, 0x17U, 0xD1U, 0xF2U, 0xE1U, 0x2CU, 0x42U, 0x47U, 0xF8U, 0xBCU, 0xE6U, 0xE5U, 0x63U, 0xA4U, 0x40U, 0xF2U,
    0x77U, 0x03U, 0x7DU, 0x81U, 0x2DU, 0xEBU, 0x33U, 0xA0U, 0xF4U, 0xA1U, 0x39U, 0x45U, 0xD8U, 0x98U, 0xC2U, 0x96U,
    0x4FU, 0xE3U, 0x42U, 0xE2U, 0xFEU, 0x1AU, 0x7FU, 0x9BU, 0x8EU, 0xE7U, 0xEBU, 0x4AU, 0x7CU, 0x0FU, 0x9EU, 0x16U,
    0x2BU, 0xCEU, 0x33U, 0x57U, 0x6BU, 0x31U, 0x5EU, 0xCEU, 0xCBU, 0xB6U, 0x40U, 0x68U, 0x37U, 0xBFU, 0x51U, 0xF5U};

static uint8_t s_BN_P256_N[WEIER256_BIT_LENGTH / 8U] = {
    0xFFU, 0xFFU, 0xFFU, 0xFFU, 0x00U, 0x00U, 0x00U, 0x00U, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU,
    0xBCU, 0xE6U, 0xFAU, 0xADU, 0xA7U, 0x17U, 0x9EU, 0x84U, 0xF3U, 0xB9U, 0xCAU, 0xC2U, 0xFCU, 0x63U, 0x25U, 0x51U};

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

/* Variables stored in flash */

/* Private key input for ECC-Ed25519 from flash */
static const uint8_t s_PrivKeyInputEccEd25519Flash[MCUXCLECC_EDDSA_ED25519_SIZE_PRIVATEKEY]
    __attribute__((aligned(4U))) = {0x83U, 0x3FU, 0xE6U, 0x24U, 0x09U, 0x23U, 0x7BU, 0x9DU, 0x62U, 0xECU, 0x77U,
                                    0x58U, 0x75U, 0x20U, 0x91U, 0x1EU, 0x9AU, 0x75U, 0x9CU, 0xECU, 0x1DU, 0x19U,
                                    0x75U, 0x5BU, 0x7DU, 0xA9U, 0x01U, 0xB9U, 0x6DU, 0xCAU, 0x3DU, 0x42U};

/* Small input message from flash */
static const uint8_t s_MessageSmallEccEd25519Flash[MESSAGE_SMALL] __attribute__((aligned(4U))) = {
    0xDDU, 0xAFU, 0x35U, 0xA1U, 0x93U, 0x61U, 0x7AU, 0xBAU, 0xCCU, 0x41U, 0x73U, 0x49U, 0xAEU, 0x20U, 0x41U, 0x31U,
    0x12U, 0xE6U, 0xFAU, 0x4EU, 0x89U, 0xA9U, 0x7EU, 0xA2U, 0x0AU, 0x9EU, 0xEEU, 0xE6U, 0x4BU, 0x55U, 0xD3U, 0x9AU,
    0x21U, 0x92U, 0x99U, 0x2AU, 0x27U, 0x4FU, 0xC1U, 0xA8U, 0x36U, 0xBAU, 0x3CU, 0x23U, 0xA3U, 0xFEU, 0xEBU, 0xBDU,
    0x45U, 0x4DU, 0x44U, 0x23U, 0x64U, 0x3CU, 0xE8U, 0x0EU, 0x2AU, 0x9AU, 0xC9U, 0x4FU, 0xA5U, 0x4CU, 0xA4U, 0x9FU};

/* Large input message from flash */
static const uint8_t s_MessageLargeEccEd25519Flash[MESSAGE_LARGE] __attribute__((aligned(4U)));

/* Example value for private RSA exponent d stored in flash */
static const uint8_t s_ExponentDRSAFlash[RSA_KEY_BYTE_LENGTH] __attribute__((aligned(4U))) = {
    0x15U, 0x5FU, 0xE6U, 0x60U, 0xCDU, 0xDEU, 0xAAU, 0x17U, 0x1BU, 0x5EU, 0xD6U, 0xBDU, 0xD0U, 0x3BU, 0xB3U, 0x56U,
    0xE0U, 0xF6U, 0xE8U, 0x6BU, 0x5AU, 0x3CU, 0x26U, 0xF3U, 0xCEU, 0x7DU, 0xAEU, 0x00U, 0x8CU, 0x4EU, 0x38U, 0xA9U,
    0xA9U, 0x7FU, 0xA5U, 0x97U, 0xB2U, 0xB9U, 0x0AU, 0x45U, 0x10U, 0xD2U, 0x23U, 0x8DU, 0x3FU, 0x15U, 0x8AU, 0xB8U,
    0x91U, 0x97U, 0xFBU, 0x08U, 0xA5U, 0xB7U, 0x4CU, 0xFEU, 0x5CU, 0xC8U, 0xF1U, 0x3DU, 0x47U, 0x09U, 0x62U, 0x91U,
    0xD0U, 0x05U, 0x38U, 0xAAU, 0x58U, 0x93U, 0xD8U, 0x2DU, 0xCEU, 0x55U, 0xB3U, 0x64U, 0x8CU, 0x6AU, 0x71U, 0x9AU,
    0xE3U, 0x87U, 0xDEU, 0xE5U, 0x5EU, 0xC5U, 0xBEU, 0xF0U, 0x89U, 0x76U, 0x3DU, 0xE7U, 0x1EU, 0x47U, 0x61U, 0xB7U,
    0x03U, 0xADU, 0x69U, 0x2EU, 0xD6U, 0x2DU, 0x7CU, 0x1FU, 0x4FU, 0x0FU, 0xF0U, 0x03U, 0xC1U, 0x67U, 0xEBU, 0x62U,
    0xD2U, 0xC6U, 0x79U, 0xCCU, 0x6FU, 0x13U, 0xB9U, 0x87U, 0xA1U, 0x42U, 0xF1U, 0x37U, 0x7AU, 0x40U, 0xBDU, 0xC0U,
    0xA0U, 0x36U, 0x60U, 0x72U, 0x94U, 0x40U, 0x14U, 0x63U, 0xA3U, 0x0EU, 0x82U, 0x91U, 0x2BU, 0x42U, 0x8AU, 0x1DU,
    0x3FU, 0x80U, 0xB5U, 0xD0U, 0xD3U, 0x3EU, 0xA8U, 0x4EU, 0x8BU, 0xB6U, 0x4CU, 0x36U, 0x22U, 0xB9U, 0xBEU, 0xE3U,
    0x56U, 0xF1U, 0x2CU, 0x6AU, 0x19U, 0x0EU, 0x55U, 0x7BU, 0xBFU, 0x25U, 0xE1U, 0x10U, 0x80U, 0x7BU, 0x85U, 0xCAU,
    0xD5U, 0x1BU, 0x39U, 0x87U, 0x57U, 0x08U, 0x06U, 0xBEU, 0x81U, 0xF3U, 0x71U, 0x3FU, 0x5DU, 0x17U, 0x40U, 0x74U,
    0x99U, 0xA5U, 0xDEU, 0xDAU, 0xC0U, 0xF3U, 0xE3U, 0xBCU, 0x79U, 0x96U, 0x35U, 0x95U, 0xF8U, 0xE0U, 0xCFU, 0x01U,
    0x29U, 0x1DU, 0xC1U, 0x02U, 0x09U, 0xC0U, 0x6EU, 0xB6U, 0x0EU, 0x2EU, 0x9CU, 0x47U, 0xECU, 0x91U, 0x42U, 0xEDU,
    0xA5U, 0xF3U, 0xB7U, 0x0AU, 0xC6U, 0x7FU, 0x72U, 0xBFU, 0x52U, 0xB3U, 0x31U, 0x37U, 0xD1U, 0x49U, 0xB6U, 0xF6U,
    0x06U, 0xE4U, 0x59U, 0x61U, 0x7DU, 0xAAU, 0x8EU, 0x10U, 0x18U, 0xA8U, 0x14U, 0x1DU, 0x89U, 0x4EU, 0xCAU, 0xFFU};

/* Example value for public RSA exponent e stored in flash */
static const uint8_t s_ExponentERSAFlash[3U] __attribute__((aligned(4))) = {0x01U, 0x00U, 0x01U};

/* Example value for public RSA modulus N stored in flash */
static const uint8_t s_ModulusRSAFlash[RSA_KEY_BYTE_LENGTH] __attribute__((aligned(4U))) = {
    0xD3U, 0x24U, 0x96U, 0xE6U, 0x2DU, 0x16U, 0x34U, 0x6EU, 0x06U, 0xE7U, 0xA3U, 0x1CU, 0x12U, 0x0AU, 0x21U, 0xB5U,
    0x45U, 0x32U, 0x32U, 0x35U, 0xEEU, 0x1DU, 0x90U, 0x72U, 0x1DU, 0xCEU, 0xAAU, 0xD4U, 0x6DU, 0xC4U, 0xCEU, 0xBDU,
    0x80U, 0xC1U, 0x34U, 0x5AU, 0xFFU, 0x95U, 0xB1U, 0xDDU, 0xF8U, 0x71U, 0xEBU, 0xB7U, 0xF2U, 0x0FU, 0xEDU, 0xB6U,
    0xE4U, 0x2EU, 0x67U, 0xA0U, 0xCCU, 0x59U, 0xB3U, 0x9FU, 0xFDU, 0x31U, 0xE9U, 0x83U, 0x42U, 0xF4U, 0x0AU, 0xD9U,
    0xAFU, 0xF9U, 0x3CU, 0x3CU, 0x51U, 0xCFU, 0x5FU, 0x3CU, 0x8AU, 0xD0U, 0x64U, 0xB8U, 0x33U, 0xF9U, 0xACU, 0x34U,
    0x22U, 0x9AU, 0x3EU, 0xD3U, 0xDDU, 0x29U, 0x41U, 0xBEU, 0x12U, 0x5BU, 0xC5U, 0xA2U, 0x0CU, 0xB6U, 0xD2U, 0x31U,
    0xB6U, 0xD1U, 0x84U, 0x7EU, 0xC4U, 0xFEU, 0xAEU, 0x2BU, 0x88U, 0x46U, 0xCFU, 0x00U, 0xC4U, 0xC6U, 0xE7U, 0x5AU,
    0x51U, 0x32U, 0x65U, 0x7AU, 0x68U, 0xECU, 0x04U, 0x38U, 0x36U, 0x46U, 0x34U, 0xEAU, 0xF8U, 0x27U, 0xF9U, 0xBBU,
    0x51U, 0x6CU, 0x93U, 0x27U, 0x48U, 0x1DU, 0x58U, 0xB8U, 0xFFU, 0x1EU, 0xA4U, 0xC0U, 0x1FU, 0xA1U, 0xA2U, 0x57U,
    0xA9U, 0x4EU, 0xA6U, 0xD4U, 0x72U, 0x60U, 0x3BU, 0x3FU, 0xB3U, 0x24U, 0x53U, 0x22U, 0x88U, 0xEAU, 0x3AU, 0x97U,
    0x43U, 0x53U, 0x59U, 0x15U, 0x33U, 0xA0U, 0xEBU, 0xBEU, 0xF2U, 0x9DU, 0xF4U, 0xF8U, 0xBCU, 0x4DU, 0xDBU, 0xF8U,
    0x8EU, 0x47U, 0x1FU, 0x1DU, 0xA5U, 0x00U, 0xB8U, 0xF5U, 0x7BU, 0xB8U, 0xC3U, 0x7CU, 0xA5U, 0xEAU, 0x17U, 0x7CU,
    0x4EU, 0x8AU, 0x39U, 0x06U, 0xB7U, 0xC1U, 0x42U, 0xF7U, 0x78U, 0x8CU, 0x45U, 0xEAU, 0xD0U, 0xC9U, 0xBCU, 0x36U,
    0x92U, 0x48U, 0x3AU, 0xD8U, 0x13U, 0x61U, 0x11U, 0x45U, 0xB4U, 0x1FU, 0x9CU, 0x01U, 0x2EU, 0xF2U, 0x87U, 0xBEU,
    0x8BU, 0xBFU, 0x93U, 0x19U, 0xCFU, 0x4BU, 0x91U, 0x84U, 0xDCU, 0x8EU, 0xFFU, 0x83U, 0x58U, 0x9BU, 0xE9U, 0x0CU,
    0x54U, 0x81U, 0x14U, 0xACU, 0xFAU, 0x5AU, 0xBFU, 0x79U, 0x54U, 0xBFU, 0x9FU, 0x7AU, 0xE5U, 0xB4U, 0x38U, 0xB5U};

/* Example value for Sha2-256 message digest stored in flash */
static const uint8_t s_MessageDigest32ByteFlash[RSA_MESSAGE_DIGEST_LENGTH] __attribute__((aligned(4U))) = {
    0xF4U, 0x45U, 0x80U, 0x1EU, 0x0CU, 0xB8U, 0x99U, 0x26U, 0x2CU, 0x9BU, 0x9EU, 0x21U, 0x98U, 0x36U, 0x88U, 0x0DU,
    0x73U, 0xCAU, 0x2DU, 0x1BU, 0x0BU, 0x9CU, 0x15U, 0xFBU, 0x95U, 0x9CU, 0x90U, 0xEBU, 0x12U, 0x12U, 0x34U, 0xE3U};

/* Example value for Sha2-512 message digest stored in flash */
static const uint8_t s_MessageDigest64ByteFlash[RSA_MESSAGE_DIGEST_LENGTH * 2U] __attribute__((aligned(4U))) = {
    0xF4U, 0x45U, 0x80U, 0x1EU, 0x0CU, 0xB8U, 0x99U, 0x26U, 0x2CU, 0x9BU, 0x9EU, 0x21U, 0x98U, 0x36U, 0x88U, 0x0DU,
    0x73U, 0xCAU, 0x2DU, 0x1BU, 0x0BU, 0x9CU, 0x15U, 0xFBU, 0x95U, 0x9CU, 0x90U, 0xEBU, 0x12U, 0x12U, 0x34U, 0xE3U,
    0xF4U, 0x45U, 0x80U, 0x1EU, 0x0CU, 0xB8U, 0x99U, 0x26U, 0x2CU, 0x9BU, 0x9EU, 0x21U, 0x98U, 0x36U, 0x88U, 0x0DU,
    0x73U, 0xCAU, 0x2DU, 0x1BU, 0x0BU, 0x9CU, 0x15U, 0xFBU, 0x95U, 0x9CU, 0x90U, 0xEBU, 0x12U, 0x12U, 0x34U, 0xE3U};

/* Domain parameters for ECC-Weier stored in flash */
static const uint8_t s_BN_P256_P_Flash[WEIER256_BIT_LENGTH / 8U] = {
    0xFFU, 0xFFU, 0xFFU, 0xFFU, 0x00U, 0x00U, 0x00U, 0x01U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,
    0x00U, 0x00U, 0x00U, 0x00U, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU};

static const uint8_t s_BN_P256_A_Flash[WEIER256_BIT_LENGTH / 8U] = {
    0xFFU, 0xFFU, 0xFFU, 0xFFU, 0x00U, 0x00U, 0x00U, 0x01U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,
    0x00U, 0x00U, 0x00U, 0x00U, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFCU};

static const uint8_t s_BN_P256_B_Flash[WEIER256_BIT_LENGTH / 8U] = {
    0x5AU, 0xC6U, 0x35U, 0xD8U, 0xAAU, 0x3AU, 0x93U, 0xE7U, 0xB3U, 0xEBU, 0xBDU, 0x55U, 0x76U, 0x98U, 0x86U, 0xBCU,
    0x65U, 0x1DU, 0x06U, 0xB0U, 0xCCU, 0x53U, 0xB0U, 0xF6U, 0x3BU, 0xCEU, 0x3CU, 0x3EU, 0x27U, 0xD2U, 0x60U, 0x4BU};

static const uint8_t s_BN_P256_G_Flash[2U * WEIER256_BIT_LENGTH / 8U] = {
    0x6BU, 0x17U, 0xD1U, 0xF2U, 0xE1U, 0x2CU, 0x42U, 0x47U, 0xF8U, 0xBCU, 0xE6U, 0xE5U, 0x63U, 0xA4U, 0x40U, 0xF2U,
    0x77U, 0x03U, 0x7DU, 0x81U, 0x2DU, 0xEBU, 0x33U, 0xA0U, 0xF4U, 0xA1U, 0x39U, 0x45U, 0xD8U, 0x98U, 0xC2U, 0x96U,
    0x4FU, 0xE3U, 0x42U, 0xE2U, 0xFEU, 0x1AU, 0x7FU, 0x9BU, 0x8EU, 0xE7U, 0xEBU, 0x4AU, 0x7CU, 0x0FU, 0x9EU, 0x16U,
    0x2BU, 0xCEU, 0x33U, 0x57U, 0x6BU, 0x31U, 0x5EU, 0xCEU, 0xCBU, 0xB6U, 0x40U, 0x68U, 0x37U, 0xBFU, 0x51U, 0xF5U};

static const uint8_t s_BN_P256_N_Flash[WEIER256_BIT_LENGTH / 8U] = {
    0xFFU, 0xFFU, 0xFFU, 0xFFU, 0x00U, 0x00U, 0x00U, 0x00U, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU,
    0xBCU, 0xE6U, 0xFAU, 0xADU, 0xA7U, 0x17U, 0x9EU, 0x84U, 0xF3U, 0xB9U, 0xCAU, 0xC2U, 0xFCU, 0x63U, 0x25U, 0x51U};

static const uint8_t s_BN_P384_P_Flash[48U] = {
    0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU,
    0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFEU,
    0xFFU, 0xFFU, 0xFFU, 0xFFU, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0xFFU, 0xFFU, 0xFFU, 0xFFU};

static const uint8_t s_BN_P384_A_Flash[48U] = {
    0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU,
    0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFEU,
    0xFFU, 0xFFU, 0xFFU, 0xFFU, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0xFFU, 0xFFU, 0xFFU, 0xFCU};

static const uint8_t s_BN_P384_B_Flash[48U] = {
    0xB3U, 0x31U, 0x2FU, 0xA7U, 0xE2U, 0x3EU, 0xE7U, 0xE4U, 0x98U, 0x8EU, 0x05U, 0x6BU, 0xE3U, 0xF8U, 0x2DU, 0x19U,
    0x18U, 0x1DU, 0x9CU, 0x6EU, 0xFEU, 0x81U, 0x41U, 0x12U, 0x03U, 0x14U, 0x08U, 0x8FU, 0x50U, 0x13U, 0x87U, 0x5AU,
    0xC6U, 0x56U, 0x39U, 0x8DU, 0x8AU, 0x2EU, 0xD1U, 0x9DU, 0x2AU, 0x85U, 0xC8U, 0xEDU, 0xD3U, 0xECU, 0x2AU, 0xEFU};

static const uint8_t s_BN_P384_G_Flash[96U] = {
    0xAAU, 0x87U, 0xCAU, 0x22U, 0xBEU, 0x8BU, 0x05U, 0x37U, 0x8EU, 0xB1U, 0xC7U, 0x1EU, 0xF3U, 0x20U, 0xADU, 0x74U,
    0x6EU, 0x1DU, 0x3BU, 0x62U, 0x8BU, 0xA7U, 0x9BU, 0x98U, 0x59U, 0xF7U, 0x41U, 0xE0U, 0x82U, 0x54U, 0x2AU, 0x38U,
    0x55U, 0x02U, 0xF2U, 0x5DU, 0xBFU, 0x55U, 0x29U, 0x6CU, 0x3AU, 0x54U, 0x5EU, 0x38U, 0x72U, 0x76U, 0x0AU, 0xB7U,
    0x36U, 0x17U, 0xDEU, 0x4AU, 0x96U, 0x26U, 0x2CU, 0x6FU, 0x5DU, 0x9EU, 0x98U, 0xBFU, 0x92U, 0x92U, 0xDCU, 0x29U,
    0xF8U, 0xF4U, 0x1DU, 0xBDU, 0x28U, 0x9AU, 0x14U, 0x7CU, 0xE9U, 0xDAU, 0x31U, 0x13U, 0xB5U, 0xF0U, 0xB8U, 0xC0U,
    0x0AU, 0x60U, 0xB1U, 0xCEU, 0x1DU, 0x7EU, 0x81U, 0x9DU, 0x7AU, 0x43U, 0x1DU, 0x7CU, 0x90U, 0xEAU, 0x0EU, 0x5FU};

static const uint8_t s_BN_P384_N_Flash[48U] = {
    0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU,
    0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xC7U, 0x63U, 0x4DU, 0x81U, 0xF4U, 0x37U, 0x2DU, 0xDFU,
    0x58U, 0x1AU, 0x0DU, 0xB2U, 0x48U, 0xB0U, 0xA7U, 0x7AU, 0xECU, 0xECU, 0x19U, 0x6AU, 0xCCU, 0xC5U, 0x29U, 0x73U};

static const uint8_t s_BN_P521_P_Flash[66U] = {
    0x01U, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU,
    0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU,
    0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU,
    0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU,
    0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU};

static const uint8_t s_BN_P521_A_Flash[66U] = {
    0x01U, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU,
    0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU,
    0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU,
    0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU,
    0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFCU};

static const uint8_t s_BN_P521_B_Flash[66U] = {
    0x00U, 0x51U, 0x95U, 0x3EU, 0xB9U, 0x61U, 0x8EU, 0x1CU, 0x9AU, 0x1FU, 0x92U, 0x9AU, 0x21U, 0xA0U,
    0xB6U, 0x85U, 0x40U, 0xEEU, 0xA2U, 0xDAU, 0x72U, 0x5BU, 0x99U, 0xB3U, 0x15U, 0xF3U, 0xB8U, 0xB4U,
    0x89U, 0x91U, 0x8EU, 0xF1U, 0x09U, 0xE1U, 0x56U, 0x19U, 0x39U, 0x51U, 0xECU, 0x7EU, 0x93U, 0x7BU,
    0x16U, 0x52U, 0xC0U, 0xBDU, 0x3BU, 0xB1U, 0xBFU, 0x07U, 0x35U, 0x73U, 0xDFU, 0x88U, 0x3DU, 0x2CU,
    0x34U, 0xF1U, 0xEFU, 0x45U, 0x1FU, 0xD4U, 0x6BU, 0x50U, 0x3FU, 0x00U};

static const uint8_t s_BN_P521_G_Flash[2U * 66U] = {
    0x00U, 0xC6U, 0x85U, 0x8EU, 0x06U, 0xB7U, 0x04U, 0x04U, 0xE9U, 0xCDU, 0x9EU, 0x3EU, 0xCBU, 0x66U, 0x23U,
    0x95U, 0xB4U, 0x42U, 0x9CU, 0x64U, 0x81U, 0x39U, 0x05U, 0x3FU, 0xB5U, 0x21U, 0xF8U, 0x28U, 0xAFU, 0x60U,
    0x6BU, 0x4DU, 0x3DU, 0xBAU, 0xA1U, 0x4BU, 0x5EU, 0x77U, 0xEFU, 0xE7U, 0x59U, 0x28U, 0xFEU, 0x1DU, 0xC1U,
    0x27U, 0xA2U, 0xFFU, 0xA8U, 0xDEU, 0x33U, 0x48U, 0xB3U, 0xC1U, 0x85U, 0x6AU, 0x42U, 0x9BU, 0xF9U, 0x7EU,
    0x7EU, 0x31U, 0xC2U, 0xE5U, 0xBDU, 0x66U, 0x01U, 0x18U, 0x39U, 0x29U, 0x6AU, 0x78U, 0x9AU, 0x3BU, 0xC0U,
    0x04U, 0x5CU, 0x8AU, 0x5FU, 0xB4U, 0x2CU, 0x7DU, 0x1BU, 0xD9U, 0x98U, 0xF5U, 0x44U, 0x49U, 0x57U, 0x9BU,
    0x44U, 0x68U, 0x17U, 0xAFU, 0xBDU, 0x17U, 0x27U, 0x3EU, 0x66U, 0x2CU, 0x97U, 0xEEU, 0x72U, 0x99U, 0x5EU,
    0xF4U, 0x26U, 0x40U, 0xC5U, 0x50U, 0xB9U, 0x01U, 0x3FU, 0xADU, 0x07U, 0x61U, 0x35U, 0x3CU, 0x70U, 0x86U,
    0xA2U, 0x72U, 0xC2U, 0x40U, 0x88U, 0xBEU, 0x94U, 0x76U, 0x9FU, 0xD1U, 0x66U, 0x50U};

static const uint8_t s_BN_P521_N_Flash[66U] = {
    0x01U, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU,
    0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU,
    0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFAU, 0x51U, 0x86U, 0x87U, 0x83U, 0xBFU, 0x2FU, 0x96U, 0x6BU,
    0x7FU, 0xCCU, 0x01U, 0x48U, 0xF7U, 0x09U, 0xA5U, 0xD0U, 0x3BU, 0xB5U, 0xC9U, 0xB8U, 0x89U, 0x9CU,
    0x47U, 0xAEU, 0xBBU, 0x6FU, 0xB7U, 0x1EU, 0x91U, 0x38U, 0x64U, 0x09U};

/* Private key input for ECC-Weier stored in flash */
static const uint8_t s_PrivateKeyInputWeier256Flash[32U] = {
    0xE9U, 0x46U, 0xFFU, 0x12U, 0xFFU, 0xB2U, 0xE7U, 0xBAU, 0x2CU, 0x5DU, 0x3AU, 0xAFU, 0x7DU, 0x9AU, 0xEEU, 0xE2U,
    0x00U, 0x59U, 0x7AU, 0xABU, 0x20U, 0xCAU, 0xB0U, 0xF9U, 0x6BU, 0xD4U, 0x84U, 0x75U, 0x3DU, 0x78U, 0xFEU, 0xF4U};

static const uint8_t s_PrivateKeyInputWeier384Flash[48U] = {
    0x8EU, 0x49U, 0xBFU, 0x1CU, 0x5DU, 0x9CU, 0xBEU, 0x73U, 0xD5U, 0xD3U, 0xDCU, 0xD7U, 0xBBU, 0x57U, 0x6AU, 0x2BU,
    0xDEU, 0x17U, 0xB1U, 0xAAU, 0xA7U, 0xCCU, 0x31U, 0xD0U, 0x24U, 0x10U, 0xB0U, 0xE6U, 0x9FU, 0xF7U, 0x42U, 0x4BU,
    0xA6U, 0x58U, 0x87U, 0x41U, 0x6AU, 0x04U, 0x14U, 0x43U, 0x4CU, 0x25U, 0x5CU, 0xECU, 0x9DU, 0x84U, 0x36U, 0x88U};

static const uint8_t s_PrivateKeyInputWeier521Flash[66U] = {
    0x00U, 0xA8U, 0x14U, 0x1AU, 0xE2U, 0xF5U, 0x5FU, 0xFCU, 0x6EU, 0x4AU, 0x39U, 0xF2U, 0x0FU, 0x3DU,
    0x53U, 0x47U, 0x19U, 0xB0U, 0x6BU, 0x32U, 0xC7U, 0xBDU, 0xEAU, 0x46U, 0x40U, 0x58U, 0xE2U, 0xC6U,
    0x73U, 0xD4U, 0xE2U, 0x35U, 0x73U, 0x8FU, 0x0FU, 0x49U, 0x08U, 0x2AU, 0x8FU, 0xE7U, 0xAAU, 0x47U,
    0x1DU, 0x2AU, 0x73U, 0x61U, 0xCAU, 0x2CU, 0xF7U, 0x60U, 0x6EU, 0x85U, 0xDBU, 0xD7U, 0x03U, 0xBEU,
    0xA6U, 0x3FU, 0xB3U, 0xCDU, 0x8CU, 0x78U, 0x72U, 0xA9U, 0x4BU, 0x20U};

/* Public key input for ECC-Weier stored in flash */
static const uint8_t s_PublicKeyInputWeier256Flash[64U] = {
    0x52U, 0x03U, 0x46U, 0xA7U, 0x4AU, 0x71U, 0xE0U, 0x4DU, 0x39U, 0xFEU, 0x4BU, 0x20U, 0x1BU, 0xF7U, 0x4CU, 0x92U,
    0xB6U, 0xBEU, 0x9FU, 0x88U, 0x11U, 0x1EU, 0x7CU, 0x31U, 0x63U, 0x13U, 0xB3U, 0xFCU, 0x94U, 0x85U, 0xDAU, 0xD9U,
    0x70U, 0x7AU, 0xBDU, 0x51U, 0x8EU, 0x51U, 0xC2U, 0xD6U, 0x56U, 0x54U, 0xC4U, 0xD9U, 0x86U, 0xE7U, 0x76U, 0x9FU,
    0x4EU, 0xA1U, 0xD9U, 0x37U, 0x39U, 0xF7U, 0xC3U, 0xABU, 0x73U, 0x89U, 0xBDU, 0x30U, 0x03U, 0x17U, 0x9BU, 0xD9U};

static const uint8_t s_PublicKeyInputWeier384Flash[96U] = {
    0x89U, 0xF1U, 0xB7U, 0x32U, 0x2DU, 0x68U, 0xEFU, 0x8AU, 0x73U, 0x17U, 0xB2U, 0x98U, 0x72U, 0xF0U, 0xE1U, 0x10U,
    0x8AU, 0xFFU, 0xF7U, 0x19U, 0x53U, 0x83U, 0x79U, 0x4AU, 0x1CU, 0x94U, 0x08U, 0xA2U, 0x16U, 0xE6U, 0x18U, 0x0AU,
    0xF3U, 0xC3U, 0x7FU, 0x69U, 0x6AU, 0xE8U, 0xCBU, 0xF0U, 0x34U, 0x8DU, 0x14U, 0x8AU, 0x9AU, 0x22U, 0x75U, 0x1DU,
    0x57U, 0x39U, 0x14U, 0x3EU, 0xE8U, 0xAFU, 0xB6U, 0x51U, 0x35U, 0x83U, 0x6CU, 0xBDU, 0x35U, 0x97U, 0x4DU, 0x67U,
    0x53U, 0xB7U, 0x12U, 0x7DU, 0xAAU, 0xDDU, 0xB2U, 0xEEU, 0x0AU, 0x60U, 0x39U, 0xFBU, 0xF0U, 0xE5U, 0x77U, 0x8CU,
    0x76U, 0xD0U, 0x6CU, 0x28U, 0xBBU, 0x66U, 0xEAU, 0xA9U, 0x4EU, 0xA3U, 0x14U, 0x6BU, 0x53U, 0xA6U, 0xA6U, 0x22U};

static const uint8_t s_PublicKeyInputWeier521Flash[132U] = {
    0x00U, 0x4BU, 0x29U, 0xF5U, 0xEFU, 0x68U, 0xBBU, 0x53U, 0x47U, 0xA5U, 0x4AU, 0x76U, 0x6AU, 0x09U, 0x80U,
    0xD6U, 0x1FU, 0x45U, 0xA1U, 0x90U, 0xD8U, 0xBBU, 0x4EU, 0xFDU, 0x88U, 0x90U, 0x5FU, 0xA6U, 0xABU, 0x6AU,
    0x6DU, 0x6BU, 0x5EU, 0xFAU, 0x5BU, 0x3EU, 0xB4U, 0xBCU, 0x4CU, 0xB4U, 0x98U, 0x6BU, 0xF0U, 0xB5U, 0x99U,
    0xACU, 0xB1U, 0xAAU, 0xD8U, 0x62U, 0xADU, 0xE0U, 0xCAU, 0x7AU, 0x22U, 0x4AU, 0xE0U, 0xC5U, 0xAEU, 0x6DU,
    0x6EU, 0x9EU, 0x97U, 0x88U, 0xDDU, 0xA0U, 0x01U, 0x01U, 0x08U, 0x21U, 0x53U, 0x9BU, 0xDAU, 0x45U, 0x0FU,
    0xCBU, 0x07U, 0x93U, 0x8EU, 0xFCU, 0x8EU, 0xE5U, 0x56U, 0xF8U, 0x8AU, 0xE0U, 0xC8U, 0x06U, 0xA8U, 0x7CU,
    0xD2U, 0x1AU, 0x1EU, 0x82U, 0x8EU, 0x3AU, 0xECU, 0x00U, 0x5EU, 0x0DU, 0x90U, 0x5FU, 0x13U, 0xF5U, 0x50U,
    0xE1U, 0xA1U, 0x95U, 0x6DU, 0x76U, 0x80U, 0xEEU, 0x9AU, 0xC5U, 0x88U, 0xBEU, 0x42U, 0x85U, 0x5CU, 0x15U,
    0xDDU, 0xCBU, 0x97U, 0xA9U, 0xFAU, 0x1BU, 0x24U, 0x91U, 0x98U, 0xA5U, 0x49U, 0x8EU};
/*******************************************************************************
 * Code
 ******************************************************************************/
bool exec_rsa_sign_pss_sha(char *data_from, uint32_t m_length, signature_algorithm_result *a_result)
{
    /**************************************************************************/
    /* Preparation                                                            */
    /**************************************************************************/
    bool data_from_ram = !strcmp(data_from, "RAM");

    /* Create session handle to be used by mcuxClRsa_sign */
    mcuxClSession_Descriptor_t sessionDesc;
    mcuxClSession_Handle_t session = &sessionDesc;

    MCUXCLEXAMPLE_ALLOCATE_AND_INITIALIZE_SESSION(session, MCUXCLRSA_SIGN_PLAIN_PSSENCODE_2048_WACPU_SIZE,
                                                  MCUXCLRSA_SIGN_PLAIN_PSSENCODE_2048_WACPU_SIZE);

    /* Initialize the PRNG */
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(prngInit_result, prngInit_token, mcuxClRandom_ncInit(session));
    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandom_ncInit) != prngInit_token) ||
        (MCUXCLRANDOM_STATUS_OK != prngInit_result))
    {
        PRINTF("[Error] PRNG initialization failed\r\n");
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    /* Create key struct of type MCUXCLRSA_KEY_PRIVATEPLAIN */
    const mcuxClRsa_KeyEntry_t Mod1 = {
        .pKeyEntryData  = data_from_ram ? (uint8_t *)s_ModulusRSA : (uint8_t *)s_ModulusRSAFlash,
        .keyEntryLength = RSA_KEY_BYTE_LENGTH};

    const mcuxClRsa_KeyEntry_t Exp1 = {
        .pKeyEntryData  = data_from_ram ? (uint8_t *)s_ExponentDRSA : (uint8_t *)s_ExponentDRSAFlash,
        .keyEntryLength = data_from_ram ? sizeof(s_ExponentDRSA) : sizeof(s_ExponentDRSAFlash)};

    const mcuxClRsa_Key private_key = {.keytype = MCUXCLRSA_KEY_PRIVATEPLAIN,
                                       .pMod1   = (mcuxClRsa_KeyEntry_t *)&Mod1,
                                       .pMod2   = NULL,
                                       .pQInv   = NULL,
                                       .pExp1   = (mcuxClRsa_KeyEntry_t *)&Exp1,
                                       .pExp2   = NULL,
                                       .pExp3   = NULL};

    /**************************************************************************/
    /* RSA signature generation call                                          */
    /**************************************************************************/
    a_result->signPerS = TIME_PUBLIC(GENERATE_RSA_SIGNATURE(data_from_ram, session, private_key, m_length));

    /**************************************************************************/
    /* Session clean-up                                                       */
    /**************************************************************************/
    /** Destroy Session and cleanup Session **/
    if (!mcuxClExample_Session_Clean(session))
    {
        PRINTF("[Error] Session cleaning failed\r\n");
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    return MCUXCLEXAMPLE_STATUS_OK;
}

bool exec_rsa_verify_pss_sha(char *data_from, uint32_t m_length, signature_algorithm_result *a_result)
{
    /**************************************************************************/
    /* Preparation                                                            */
    /**************************************************************************/
    bool data_from_ram = !strcmp(data_from, "RAM");

    /* Create session handle to be used by verify function */
    mcuxClSession_Descriptor_t sessionDesc;
    mcuxClSession_Handle_t session = &sessionDesc;

    MCUXCLEXAMPLE_ALLOCATE_AND_INITIALIZE_SESSION(session, MCUXCLRSA_VERIFY_PSSVERIFY_WACPU_SIZE,
                                                  MCUXCLRSA_VERIFY_2048_WAPKC_SIZE);

    /* Initialize the PRNG */
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(prngInit_result, prngInit_token, mcuxClRandom_ncInit(session));
    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandom_ncInit) != prngInit_token) ||
        (MCUXCLRANDOM_STATUS_OK != prngInit_result))
    {
        PRINTF("[Error] PRNG initialization failed\r\n");
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    /* Create key struct of type MCUXCLRSA_KEY_PUBLIC */
    const mcuxClRsa_KeyEntry_t Mod1 = {
        .pKeyEntryData  = data_from_ram ? (uint8_t *)s_ModulusRSA : (uint8_t *)s_ModulusRSAFlash,
        .keyEntryLength = RSA_KEY_BYTE_LENGTH};

    const mcuxClRsa_KeyEntry_t Exp1 = {
        .pKeyEntryData  = data_from_ram ? (uint8_t *)s_ExponentERSA : (uint8_t *)s_ExponentERSAFlash,
        .keyEntryLength = data_from_ram ? sizeof(s_ExponentERSA) : sizeof(s_ExponentERSAFlash)};

    const mcuxClRsa_Key public_key = {.keytype = MCUXCLRSA_KEY_PUBLIC,
                                      .pMod1   = (mcuxClRsa_KeyEntry_t *)&Mod1,
                                      .pMod2   = NULL,
                                      .pQInv   = NULL,
                                      .pExp1   = (mcuxClRsa_KeyEntry_t *)&Exp1,
                                      .pExp2   = NULL,
                                      .pExp3   = NULL};

    /**************************************************************************/
    /* RSA verification call                                                  */
    /**************************************************************************/
    a_result->verifyPerS = TIME_PUBLIC(RSA_VERIFY(data_from_ram, session, public_key, m_length));

    /**************************************************************************/
    /* Session clean-up                                                       */
    /**************************************************************************/

    /** Destroy Session and cleanup Session **/
    if (!mcuxClExample_Session_Clean(session))
    {
        PRINTF("[Error] Session cleaning failed\r\n");
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    return MCUXCLEXAMPLE_STATUS_OK;
}

bool exec_EdDSA_generate_signature_Ed25519(char *data_from, uint32_t m_length, signature_algorithm_result *a_result)
{
    /******************************************/
    /* Set Up the environment                 */
    /******************************************/
    bool data_from_ram = !strcmp(data_from, "RAM");

    /* Setup one session to be used by all functions called */
    mcuxClSession_Descriptor_t session;

    /* Allocate and initialize PKC workarea */
    MCUXCLEXAMPLE_ALLOCATE_AND_INITIALIZE_SESSION(&session, MAX_CPUWA_SIZE, MAX_PKCWA_SIZE);

    /* Initialize the RNG context and Initialize the PRNG */
    MCUXCLEXAMPLE_ALLOCATE_AND_INITIALIZE_RNG(&session, 0U, mcuxClRandomModes_Mode_ELS_Drbg);

    /******************************************/
    /* Initialize the private and public keys */
    /******************************************/

    /* Allocate space for and initialize private key handle for an Ed25519 private key */
    uint8_t privKeyDesc[MCUXCLKEY_DESCRIPTOR_SIZE];
    mcuxClKey_Handle_t privKey = (mcuxClKey_Handle_t)&privKeyDesc;
    uint8_t pPrivKeyData[MCUXCLECC_EDDSA_ED25519_SIZE_PRIVATEKEYDATA];

    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(privkeyinit_result, privkeyinit_token,
                                     mcuxClKey_init(
                                         /* mcuxClSession_Handle_t session         */ &session,
                                         /* mcuxClKey_Handle_t key                 */ privKey,
                                         /* mcuxClKey_Type_t type                  */ mcuxClKey_Type_EdDSA_Ed25519_Priv,
                                         /* mcuxCl_Buffer_t pKeyData               */ (mcuxCl_Buffer_t)pPrivKeyData,
                                         /* uint32_t keyDataLength                 */ sizeof(pPrivKeyData)));

    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClKey_init) != privkeyinit_token) ||
        (MCUXCLKEY_STATUS_OK != privkeyinit_result))
    {
        PRINTF("[Error] Private key initialization failed\r\n");
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    /* Allocate space for and initialize pbulic key handle for an Ed25519 public key */
    uint8_t pubKeyDesc[MCUXCLKEY_DESCRIPTOR_SIZE];
    mcuxClKey_Handle_t pubKey = (mcuxClKey_Handle_t)&pubKeyDesc;

    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(
        pubkeyinit_result, pubkeyinit_token,
        mcuxClKey_init(
            /* mcuxClSession_Handle_t session         */ &session,
            /* mcuxClKey_Handle_t key                 */ pubKey,
            /* mcuxClKey_Type_t type                  */ mcuxClKey_Type_EdDSA_Ed25519_Pub,
            /* mcuxCl_Buffer_t pKeyData               */ (mcuxCl_Buffer_t)s_PublicKeyBufferEcc,
            /* uint32_t keyDataLength                 */ sizeof(s_PublicKeyBufferEcc)));

    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClKey_init) != pubkeyinit_token) ||
        (MCUXCLKEY_STATUS_OK != pubkeyinit_result))
    {
        PRINTF("[Error] Public key initialization failed\r\n");
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    /* Allocate space for and initialize EdDSA key pair generation descriptor for private key input */
    uint8_t privKeyInputDescriptor[MCUXCLECC_EDDSA_GENERATEKEYPAIR_DESCRIPTOR_SIZE];
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(initmode_result, initmode_token,
                                     mcuxClEcc_EdDSA_InitPrivKeyInputMode(
                                         /* mcuxClSession_Handle_t pSession                   */ &session,
                                         /* mcuxClEcc_EdDSA_GenerateKeyPairDescriptor_t *mode */
                                         (mcuxClEcc_EdDSA_GenerateKeyPairDescriptor_t *)&privKeyInputDescriptor,
                                         /* const uint8_t *pPrivKey                          */
                                         data_from_ram ? s_PrivKeyInputEccEd25519 : s_PrivKeyInputEccEd25519Flash));

    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEcc_EdDSA_InitPrivKeyInputMode) != initmode_token) ||
        (MCUXCLECC_STATUS_OK != initmode_result))
    {
        PRINTF("[Error] Key pair generation failed\r\n");
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    /**************************************************************************/
    /* Key pair generation for EdDSA on Ed25519                               */
    /**************************************************************************/

    /* Call mcuxClEcc_EdDSA_GenerateKeyPair to derive the public key from the private one. */
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(keygen_result, keygen_token,
                                     mcuxClEcc_EdDSA_GenerateKeyPair(
                                         /*  mcuxClSession_Handle_t pSession                          */ &session,
                                         /*  const mcuxClEcc_EdDSA_GenerateKeyPairDescriptor_t *mode  */
                                         (mcuxClEcc_EdDSA_GenerateKeyPairDescriptor_t *)&privKeyInputDescriptor,
                                         /*  mcuxClKey_Handle_t privKey                               */ privKey,
                                         /*  mcuxClKey_Handle_t pubKey                                */ pubKey));

    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEcc_EdDSA_GenerateKeyPair) != keygen_token) ||
        (MCUXCLECC_STATUS_OK != keygen_result))
    {
        PRINTF("[Error] Public key derivation failed\r\n");
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    /**************************************************************************/
    /* Ed25519 signature generation                                           */
    /**************************************************************************/
    a_result->signPerS = TIME_PUBLIC(GENERATE_ECC_ED25519_SIGNATURE(data_from_ram, session, privKey, m_length));

    /******************************************/
    /* Clean Up                               */
    /******************************************/

    /* Destroy Session and cleanup Session */
    if (!mcuxClExample_Session_Clean(&session))
    {
        PRINTF("[Error] Session cleaning failed\r\n");
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    return MCUXCLEXAMPLE_STATUS_OK;
}

bool exec_EdDSA_verify_signature_Ed25519(char *data_from, uint32_t m_length, signature_algorithm_result *a_result)
{
    /******************************************/
    /* Set up the environment                 */
    /******************************************/
    bool data_from_ram = !strcmp(data_from, "RAM");

    /* Setup one session to be used by all functions called */
    mcuxClSession_Descriptor_t session;

    /* Allocate and initialize PKC workarea */
    MCUXCLEXAMPLE_ALLOCATE_AND_INITIALIZE_SESSION(&session, MAX_CPUWA_SIZE, MAX_PKCWA_SIZE);

    /******************************************/
    /* Initialize the public key              */
    /******************************************/

    /* Initialize public key */
    uint8_t pubKeyDesc[MCUXCLKEY_DESCRIPTOR_SIZE];
    mcuxClKey_Handle_t pubKeyHandler = (mcuxClKey_Handle_t)&pubKeyDesc;

    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(
        keyInit_status, keyInit_token,
        mcuxClKey_init(
            /* mcuxClSession_Handle_t session         */ &session,
            /* mcuxClKey_Handle_t key                 */ pubKeyHandler,
            /* mcuxClKey_Type_t type                  */ mcuxClKey_Type_EdDSA_Ed25519_Pub,
            /* mcuxCl_Buffer_t pKeyData               */ (mcuxCl_Buffer_t)s_PublicKeyBufferEcc,
            /* uint32_t keyDataLength                 */ sizeof(s_PublicKeyBufferEcc)));

    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClKey_init) != keyInit_token) || (MCUXCLKEY_STATUS_OK != keyInit_status))
    {
        PRINTF("[Error] Public key initialization failed\r\n");
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    /**************************************************************************/
    /* Ed25519 signature verification                                         */
    /**************************************************************************/
    a_result->verifyPerS = TIME_PUBLIC(ECC_ED25519_VERIFY(data_from_ram, session, pubKeyHandler, m_length));

    /* Destroy Session and cleanup Session */
    if (!mcuxClExample_Session_Clean(&session))
    {
        PRINTF("[Error] Session cleaning failed\r\n");
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    return MCUXCLEXAMPLE_STATUS_OK;
}

static mcuxClEcc_DomainParam_t get_domain_param_by_mode(uint32_t bit_length, bool data_from_ram)
{
    const uint32_t pByteLength = (bit_length + 7U) / 8U;
    const uint32_t nByteLength = (bit_length + 7U) / 8U;
    switch (bit_length)
    {
        case WEIER256_BIT_LENGTH:
            return (mcuxClEcc_DomainParam_t){.pA   = data_from_ram ? s_BN_P256_A : s_BN_P256_A_Flash,
                                             .pB   = data_from_ram ? s_BN_P256_B : s_BN_P256_B_Flash,
                                             .pG   = data_from_ram ? s_BN_P256_G : s_BN_P256_G_Flash,
                                             .pP   = data_from_ram ? s_BN_P256_P : s_BN_P256_P_Flash,
                                             .pN   = data_from_ram ? s_BN_P256_N : s_BN_P256_N_Flash,
                                             .misc = mcuxClEcc_DomainParam_misc_Pack(nByteLength, pByteLength)};
        case WEIER384_BIT_LENGTH:
            return (mcuxClEcc_DomainParam_t){.pA   = data_from_ram ? s_BN_P384_A : s_BN_P384_A_Flash,
                                             .pB   = data_from_ram ? s_BN_P384_B : s_BN_P384_B_Flash,
                                             .pG   = data_from_ram ? s_BN_P384_G : s_BN_P384_G_Flash,
                                             .pP   = data_from_ram ? s_BN_P384_P : s_BN_P384_P_Flash,
                                             .pN   = data_from_ram ? s_BN_P384_N : s_BN_P384_N_Flash,
                                             .misc = mcuxClEcc_DomainParam_misc_Pack(nByteLength, pByteLength)};
        case WEIER521_BIT_LENGTH:
            return (mcuxClEcc_DomainParam_t){.pA   = data_from_ram ? s_BN_P521_A : s_BN_P521_A_Flash,
                                             .pB   = data_from_ram ? s_BN_P521_B : s_BN_P521_B_Flash,
                                             .pG   = data_from_ram ? s_BN_P521_G : s_BN_P521_G_Flash,
                                             .pP   = data_from_ram ? s_BN_P521_P : s_BN_P521_P_Flash,
                                             .pN   = data_from_ram ? s_BN_P521_N : s_BN_P521_N_Flash,
                                             .misc = mcuxClEcc_DomainParam_misc_Pack(nByteLength, pByteLength)};
    }

    mcuxClEcc_DomainParam_t default_return;
    default_return.pA = NULL;
    return default_return;
}

static mcuxClEcc_Sign_Param_t get_param_sign(uint32_t bit_length,
                                             mcuxClEcc_DomainParam_t domain_params,
                                             bool data_from_ram,
                                             uint32_t m_length)
{
    if (data_from_ram)
    {
        switch (bit_length)
        {
            case WEIER256_BIT_LENGTH:
                return (mcuxClEcc_Sign_Param_t){
                    .curveParam  = domain_params,
                    .pHash       = m_length == 32U ? s_MessageDigest32Byte : s_MessageDigest64Byte,
                    .pPrivateKey = s_PrivateKeyInputWeier256,
                    .pSignature  = s_SignatureBufferWeier,
                    .optLen      = m_length == 32U ? mcuxClEcc_Sign_Param_optLen_Pack(32U) :
                                                     mcuxClEcc_Sign_Param_optLen_Pack(64U)};

            case WEIER384_BIT_LENGTH:
                return (mcuxClEcc_Sign_Param_t){
                    .curveParam  = domain_params,
                    .pHash       = m_length == 32U ? s_MessageDigest32Byte : s_MessageDigest64Byte,
                    .pPrivateKey = s_PrivateKeyInputWeier384,
                    .pSignature  = s_SignatureBufferWeier,
                    .optLen      = m_length == 32U ? mcuxClEcc_Sign_Param_optLen_Pack(32U) :
                                                     mcuxClEcc_Sign_Param_optLen_Pack(64U)};
            case WEIER521_BIT_LENGTH:
                return (mcuxClEcc_Sign_Param_t){
                    .curveParam  = domain_params,
                    .pHash       = m_length == 32U ? s_MessageDigest32Byte : s_MessageDigest64Byte,
                    .pPrivateKey = s_PrivateKeyInputWeier521,
                    .pSignature  = s_SignatureBufferWeier,
                    .optLen      = m_length == 32U ? mcuxClEcc_Sign_Param_optLen_Pack(32U) :
                                                     mcuxClEcc_Sign_Param_optLen_Pack(64U)};
        }
    }
    else
    {
        switch (bit_length)
        {
            case WEIER256_BIT_LENGTH:
                return (mcuxClEcc_Sign_Param_t){
                    .curveParam  = domain_params,
                    .pHash       = m_length == 32U ? s_MessageDigest32ByteFlash : s_MessageDigest64ByteFlash,
                    .pPrivateKey = s_PrivateKeyInputWeier256Flash,
                    .pSignature  = s_SignatureBufferWeier,
                    .optLen      = m_length == 32U ? mcuxClEcc_Sign_Param_optLen_Pack(32U) :
                                                     mcuxClEcc_Sign_Param_optLen_Pack(64U)};
            case WEIER384_BIT_LENGTH:
                return (mcuxClEcc_Sign_Param_t){
                    .curveParam  = domain_params,
                    .pHash       = m_length == 32U ? s_MessageDigest32ByteFlash : s_MessageDigest64ByteFlash,
                    .pPrivateKey = s_PrivateKeyInputWeier384Flash,
                    .pSignature  = s_SignatureBufferWeier,
                    .optLen      = m_length == 32U ? mcuxClEcc_Sign_Param_optLen_Pack(32U) :
                                                     mcuxClEcc_Sign_Param_optLen_Pack(64U)};
            case WEIER521_BIT_LENGTH:
                return (mcuxClEcc_Sign_Param_t){
                    .curveParam  = domain_params,
                    .pHash       = m_length == 32U ? s_MessageDigest32ByteFlash : s_MessageDigest64ByteFlash,
                    .pPrivateKey = s_PrivateKeyInputWeier521Flash,
                    .pSignature  = s_SignatureBufferWeier,
                    .optLen      = m_length == 32U ? mcuxClEcc_Sign_Param_optLen_Pack(32U) :
                                                     mcuxClEcc_Sign_Param_optLen_Pack(64U)};
        }
    }
    mcuxClEcc_Sign_Param_t default_return;
    default_return.pSignature = NULL;
    return default_return;
}

bool exec_weier_ecc_generate_signature(char *data_from, uint32_t m_length, uint32_t bit_length)
{
    const uint32_t pByteLength = (bit_length + 7U) / 8U;
    const uint32_t nByteLength = (bit_length + 7U) / 8U;
    /**************************************************************************/
    /* Preparation                                                            */
    /**************************************************************************/
    bool data_from_ram = !strcmp(data_from, "RAM");

    mcuxClSession_Descriptor_t sessionDesc;
    mcuxClSession_Handle_t pSession = &sessionDesc;

    /* Allocate and initialize session */
    MCUXCLEXAMPLE_ALLOCATE_AND_INITIALIZE_SESSION(pSession, MCUXCLECC_SIGN_WACPU_SIZE,
                                                  MCUXCLECC_SIGN_WAPKC_SIZE(pByteLength, nByteLength));

    /* Initialize the RNG context, with maximum size */
    uint32_t rng_ctx[MCUXCLRANDOMMODES_CTR_DRBG_AES256_CONTEXT_SIZE_IN_WORDS] = {0U};

    mcuxClRandom_Mode_t randomMode = NULL;

    uint32_t value = (uint32_t)MCUX_PKC_MIN((nByteLength * 8U) / 2U, 256U);
    if (value <= 128U) /* 128-bit security strength */
    {
        randomMode = mcuxClRandomModes_Mode_ELS_Drbg;
    }
    else /* 256-bit security strength */
    {
        randomMode = mcuxClRandomModes_Mode_CtrDrbg_AES256_DRG3;
    }

    MCUX_CSSL_FP_FUNCTION_CALL_PROTECTED(randomInit_result, randomInit_token,
                                         mcuxClRandom_init(pSession, (mcuxClRandom_Context_t)rng_ctx, randomMode));
    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandom_init) != randomInit_token) ||
        (MCUXCLRANDOM_STATUS_OK != randomInit_result))
    {
        PRINTF("[Error] DRBG init failed\r\n");
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    mcuxClEcc_DomainParam_t domain_param = get_domain_param_by_mode(bit_length, data_from_ram);

    /**************************************************************************/
    /* Generate signature                                                     */
    /**************************************************************************/
    mcuxClEcc_Sign_Param_t parameters = get_param_sign(bit_length, domain_param, data_from_ram, m_length);

    mcuxClEcc_ECDSA_SignatureProtocolDescriptor_t descriptor;
    descriptor.generateOption = MCUXCLECC_ECDSA_SIGNATURE_GENERATE_RANDOMIZED;
    parameters.pMode          = &descriptor;

    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(sign_result, sign_token, mcuxClEcc_Sign(pSession, &parameters));
    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEcc_Sign) != sign_token))
    {
        PRINTF("[Error] Weier signature token mismatch\r\n");
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }
    if (MCUXCLECC_STATUS_SIGN_INVALID_PARAMS == sign_result)
    {
        PRINTF("[Error] Bad input data\r\n");
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }
    if (MCUXCLECC_STATUS_SIGN_RNG_ERROR == sign_result)
    {
        PRINTF("[Error] RNG Failed\r\n");
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }
    if (MCUXCLECC_STATUS_SIGN_OK != sign_result)
    {
        PRINTF("[Error] ECC-Weier sign failed\r\n");
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    /**************************************************************************/
    /* Clean session                                                          */
    /**************************************************************************/
    if (!mcuxClExample_Session_Clean(pSession))
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    return MCUXCLEXAMPLE_STATUS_OK;
}

bool exec_weier_ecc_verify_signature(char *data_from, uint32_t m_length, uint32_t bit_length)
{
    const uint32_t pByteLength = (bit_length + 7U) / 8U;
    const uint32_t nByteLength = (bit_length + 7U) / 8U;

    /**************************************************************************/
    /* Preparation                                                            */
    /**************************************************************************/
    bool data_from_ram = !strcmp(data_from, "RAM");

    mcuxClSession_Descriptor_t sessionDesc;
    mcuxClSession_Handle_t pSession = &sessionDesc;
    MCUXCLEXAMPLE_ALLOCATE_AND_INITIALIZE_SESSION(pSession, MCUXCLECC_VERIFY_WACPU_SIZE,
                                                  MCUXCLECC_VERIFY_WAPKC_SIZE(pByteLength, nByteLength));

    mcuxClEcc_DomainParam_t domain_params = get_domain_param_by_mode(bit_length, data_from_ram);

    uint8_t pScalarPrecG[66U]                              = {0U};
    uint32_t scalarBitIndex                                = 4U * nByteLength;
    pScalarPrecG[nByteLength - 1U - (scalarBitIndex / 8U)] = (uint8_t)1U << (scalarBitIndex & 7U);

    uint8_t pResult[132U]                       = {0U};
    mcuxClEcc_PointMult_Param_t pointMultParams = {.curveParam = domain_params,
                                                   .pScalar    = pScalarPrecG,
                                                   .pPoint     = domain_params.pG,
                                                   .pResult    = pResult,
                                                   .optLen     = 0U};
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(retEccPointMult, tokenEccPointMult,
                                     mcuxClEcc_PointMult(pSession, &pointMultParams));

    if (MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEcc_PointMult) != tokenEccPointMult)
    {
        PRINTF("[Error] ECC Point multiplication token mismatch\r\n");
    }
    if (MCUXCLECC_STATUS_POINTMULT_INVALID_PARAMS == retEccPointMult)
    {
        PRINTF("[Error] ECC Point multiplication invalid params\r\n");
    }
    else if (MCUXCLECC_STATUS_POINTMULT_OK != retEccPointMult)
    {
        PRINTF("[Error] ECC Point multiplication failed\r\n");
    }

    MCUX_CSSL_FP_FUNCTION_CALL_END();
    uint8_t pOutputR[66U];
    mcuxClEcc_Verify_Param_t paramVerify;
    if (data_from_ram)
    {
        paramVerify.curveParam = domain_params;
        paramVerify.pPrecG     = pResult;
        paramVerify.pHash      = m_length == 32U ? s_MessageDigest32Byte : s_MessageDigest64Byte;
        paramVerify.pSignature = s_SignatureBufferWeier;
        switch (bit_length)
        {
            case WEIER256_BIT_LENGTH:
                paramVerify.pPublicKey = s_PublicKeyInputWeier256;
                break;
            case WEIER384_BIT_LENGTH:
                paramVerify.pPublicKey = s_PublicKeyInputWeier384;
                break;
            case WEIER521_BIT_LENGTH:
                paramVerify.pPublicKey = s_PublicKeyInputWeier521;
                break;
        }
        paramVerify.pOutputR = pOutputR;
        paramVerify.optLen =
            m_length == 32U ? mcuxClEcc_Sign_Param_optLen_Pack(32U) : mcuxClEcc_Sign_Param_optLen_Pack(64U);
    }
    else
    {
        paramVerify.curveParam = domain_params;
        paramVerify.pPrecG     = pResult;
        paramVerify.pHash      = m_length == 32U ? s_MessageDigest32ByteFlash : s_MessageDigest64ByteFlash;
        paramVerify.pSignature = s_SignatureBufferWeier;
        switch (bit_length)
        {
            case WEIER256_BIT_LENGTH:
                paramVerify.pPublicKey = s_PublicKeyInputWeier256Flash;
                break;
            case WEIER384_BIT_LENGTH:
                paramVerify.pPublicKey = s_PublicKeyInputWeier384Flash;
                break;
            case WEIER521_BIT_LENGTH:
                paramVerify.pPublicKey = s_PublicKeyInputWeier521Flash;
                break;
        }
        paramVerify.pOutputR = pOutputR;
        paramVerify.optLen =
            m_length == 32U ? mcuxClEcc_Sign_Param_optLen_Pack(32U) : mcuxClEcc_Sign_Param_optLen_Pack(64U);
    }

    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(retEccVerify, tokenEccVerify, mcuxClEcc_Verify(pSession, &paramVerify));
    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEcc_Verify) != tokenEccVerify) ||
        (MCUXCLECC_STATUS_VERIFY_OK != retEccVerify))
    {
        PRINTF("[Error] ECC-Weier verify failed\r\n");
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    /**************************************************************************/
    /* Clean session                                                          */
    /**************************************************************************/
    if (!mcuxClExample_Session_Clean(pSession))
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    return MCUXCLEXAMPLE_STATUS_OK;
}

void test_weier_signature(char *code_from, char *data_from, uint32_t m_length, uint32_t bit_length)
{
    if (!strcmp(code_from, "RAM") && !strcmp(data_from, "FLASH"))
        return;
    signature_algorithm_result a_result;
    strcpy(a_result.code, code_from);
    strcpy(a_result.data, data_from);
    strcpy(a_result.execution, m_length == 32U ? "SHA-256" : "SHA-512");
    a_result.signPerS   = TIME_PUBLIC(exec_weier_ecc_generate_signature(data_from, m_length, bit_length));
    a_result.verifyPerS = TIME_PUBLIC(exec_weier_ecc_verify_signature(data_from, m_length, bit_length));

    PRINT_SIGNATURE_RESULT(a_result);
}

void test_ecc_ed25519_signature(char *code_from, char *data_from, uint32_t m_length)
{
    if (!strcmp(code_from, "RAM") && !strcmp(data_from, "FLASH"))
        return;
    signature_algorithm_result a_result;
    strcpy(a_result.code, code_from);
    strcpy(a_result.data, data_from);
    strcpy(a_result.execution, m_length == MESSAGE_SMALL ? "SMALL MESSAGE" : "LARGE MESSAGE");
    exec_EdDSA_generate_signature_Ed25519(data_from, m_length, &a_result);
    exec_EdDSA_verify_signature_Ed25519(data_from, m_length, &a_result);

    PRINT_SIGNATURE_RESULT(a_result);
}

void test_rsa_signature(char *code_from, char *data_from, uint32_t m_length)
{
    if (!strcmp(code_from, "RAM") && !strcmp(data_from, "FLASH"))
        return;
    signature_algorithm_result a_result;
    strcpy(a_result.code, code_from);
    strcpy(a_result.data, data_from);
    strcpy(a_result.execution, m_length == 32U ? "SHA-256" : "SHA-512");
    exec_rsa_sign_pss_sha(data_from, m_length, &a_result);
    exec_rsa_verify_pss_sha(data_from, m_length, &a_result);

    PRINT_SIGNATURE_RESULT(a_result);
}

void run_tests_asymmetric(void)
{
    char code_from[6U];
    strcpy(code_from, BOARD_IS_XIP() ? "FLASH" : "RAM");

    PRINTF("ECC-ECDSA-WEIER-P256\r\n");
    test_weier_signature(code_from, "RAM", 64U, WEIER256_BIT_LENGTH);
    test_weier_signature(code_from, "FLASH", 64U, WEIER256_BIT_LENGTH);
    test_weier_signature(code_from, "RAM", 32U, WEIER256_BIT_LENGTH);
    test_weier_signature(code_from, "FLASH", 32U, WEIER256_BIT_LENGTH);
    PRINTF("\r\n");

    PRINTF("ECC-ECDSA-WEIER-P384\r\n");
    test_weier_signature(code_from, "RAM", 64U, WEIER384_BIT_LENGTH);
    test_weier_signature(code_from, "FLASH", 64U, WEIER384_BIT_LENGTH);
    test_weier_signature(code_from, "RAM", 32U, WEIER384_BIT_LENGTH);
    test_weier_signature(code_from, "FLASH", 32U, WEIER384_BIT_LENGTH);
    PRINTF("\r\n");

    PRINTF("ECC-ECDSA-WEIER-P521\r\n");
    test_weier_signature(code_from, "RAM", 64U, WEIER521_BIT_LENGTH);
    test_weier_signature(code_from, "FLASH", 64U, WEIER521_BIT_LENGTH);
    test_weier_signature(code_from, "RAM", 32U, WEIER521_BIT_LENGTH);
    test_weier_signature(code_from, "FLASH", 32U, WEIER521_BIT_LENGTH);
    PRINTF("\r\n");

    PRINTF("ECC-EDDSA-ED25519:\r\n");
    test_ecc_ed25519_signature(code_from, "RAM", MESSAGE_LARGE);
    test_ecc_ed25519_signature(code_from, "FLASH", MESSAGE_LARGE);
    test_ecc_ed25519_signature(code_from, "RAM", MESSAGE_SMALL);
    test_ecc_ed25519_signature(code_from, "FLASH", MESSAGE_SMALL);
    PRINTF("\r\n");

    PRINTF("RSA-PSS-SHA:\r\n");
    test_rsa_signature(code_from, "RAM", 64U);
    test_rsa_signature(code_from, "FLASH", 64U);
    test_rsa_signature(code_from, "RAM", 32U);
    test_rsa_signature(code_from, "FLASH", 32U);
    PRINTF("\r\n");
}
