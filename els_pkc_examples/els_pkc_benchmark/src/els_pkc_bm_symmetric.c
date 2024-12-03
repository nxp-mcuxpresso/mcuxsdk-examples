/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "els_pkc_bm_symmetric.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define AES_128_KEY 16U
#define AES_192_KEY 24U
#define AES_256_KEY 32U

#define AES_ENCRYPT(mode, data_from_ram, key, session, block_amount)                                                  \
    do                                                                                                                \
    {                                                                                                                 \
        uint32_t msg_enc_size = 0U;                                                                                   \
        if (data_from_ram)                                                                                            \
        {                                                                                                             \
            MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(                                                                         \
                result_enc, token_enc,                                                                                \
                mcuxClCipher_crypt(                                                                                   \
                    /* mcuxClSession_Handle_t session: */ session, /* mcuxClKey_Handle_t key:         */ key,         \
                    /* mcuxClCipher_Mode_t mode:       */ mode,                                                       \
                    /* mcuxCl_InputBuffer_t pIv:       */ mode == mcuxClCipher_Mode_AES_ECB_Enc_NoPadding ?           \
                        NULL :                                                                                        \
                        s_Aes128Iv,                                                                                   \
                    /* uint32_t ivLength:              */ mode == mcuxClCipher_Mode_AES_ECB_Enc_NoPadding ?           \
                        0U :                                                                                          \
                        sizeof(s_Aes128Iv),                                                                           \
                    /* mcuxCl_InputBuffer_t pIn:       */ block_amount == SINGLE_BLOCK ? s_MsgPlainSingleBlock :      \
                                                                                         s_MsgPlain,                  \
                    /* uint32_t inLength:              */ block_amount == SINGLE_BLOCK ?                              \
                        sizeof(s_MsgPlainSingleBlock) :                                                               \
                        sizeof(s_MsgPlain),                                                                           \
                    /* mcuxCl_Buffer_t pOut:           */ block_amount == SINGLE_BLOCK ? s_MsgEncSingleBlock :        \
                                                                                         s_MsgEnc,                    \
                    /* uint32_t * const pOutLength:    */ &msg_enc_size));                                            \
                                                                                                                      \
            if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClCipher_crypt) != token_enc) ||                                    \
                (MCUXCLCIPHER_STATUS_OK != result_enc))                                                               \
            {                                                                                                         \
                PRINTF("[Error] Encryption failed\r\n");                                                              \
                return MCUXCLEXAMPLE_ERROR;                                                                           \
            }                                                                                                         \
            MCUX_CSSL_FP_FUNCTION_CALL_END();                                                                         \
        }                                                                                                             \
        else                                                                                                          \
        { /* Data from flash memory */                                                                                \
            MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(                                                                         \
                result_enc, token_enc,                                                                                \
                mcuxClCipher_crypt(                                                                                   \
                    /* mcuxClSession_Handle_t session: */ session, /* mcuxClKey_Handle_t key:         */ key,         \
                    /* mcuxClCipher_Mode_t mode:       */ mode,                                                       \
                    /* mcuxCl_InputBuffer_t pIv:       */ mode == mcuxClCipher_Mode_AES_ECB_Enc_NoPadding ?           \
                        NULL :                                                                                        \
                        s_Aes128IvFlash,                                                                              \
                    /* uint32_t ivLength:              */ mode == mcuxClCipher_Mode_AES_ECB_Enc_NoPadding ?           \
                        0U :                                                                                          \
                        sizeof(s_Aes128IvFlash),                                                                      \
                    /* mcuxCl_InputBuffer_t pIn:       */ block_amount == SINGLE_BLOCK ? s_MsgPlainSingleBlockFlash : \
                                                                                         s_MsgPlainFlash,             \
                    /* uint32_t inLength:              */ block_amount == SINGLE_BLOCK ?                              \
                        sizeof(s_MsgPlainSingleBlockFlash) :                                                          \
                        sizeof(s_MsgPlainFlash),                                                                      \
                    /* mcuxCl_Buffer_t pOut:           */ block_amount == SINGLE_BLOCK ? s_MsgEncSingleBlock :        \
                                                                                         s_MsgEnc,                    \
                    /* uint32_t * const pOutLength:    */ &msg_enc_size));                                            \
                                                                                                                      \
            if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClCipher_crypt) != token_enc) ||                                    \
                (MCUXCLCIPHER_STATUS_OK != result_enc))                                                               \
            {                                                                                                         \
                PRINTF("[Error] Encryption failed\r\n");                                                              \
                return MCUXCLEXAMPLE_ERROR;                                                                           \
            }                                                                                                         \
            MCUX_CSSL_FP_FUNCTION_CALL_END();                                                                         \
        }                                                                                                             \
    } while (0);

#define AEAD_ENCRYPT(mode, data_from_ram, key, session, block_amount)                                                  \
    do                                                                                                                 \
    {                                                                                                                  \
        uint32_t msg_enc_size = 0U;                                                                                    \
        uint8_t msg_tag[6U];                                                                                           \
        if (data_from_ram)                                                                                             \
        {                                                                                                              \
            MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(                                                                          \
                result_enc, token_enc,                                                                                 \
                mcuxClAead_crypt(                                                                                      \
                    /* mcuxClSession_Handle_t session, */ session, /* mcuxClKey_Handle_t key,         */ key,          \
                    /* mcuxClAead_Mode_t mode,         */ mode, /* mcuxCl_InputBuffer_t pNonce,    */ s_Nonce64,       \
                    /* uint32_t nonceSize,             */ sizeof(s_Nonce64),                                           \
                    /* mcuxCl_InputBuffer_t pIn,       */ block_amount == SINGLE_BLOCK ? s_MsgPlainSingleBlock :       \
                                                                                         s_MsgPlain,                   \
                    /* uint32_t inSize,                */ block_amount == SINGLE_BLOCK ?                               \
                        sizeof(s_MsgPlainSingleBlock) :                                                                \
                        sizeof(s_MsgPlain),                                                                            \
                    /* mcuxCl_InputBuffer_t pAdata,    */ s_MsgAdata,                                                  \
                    /* uint32_t adataSize,             */ sizeof(s_MsgAdata),                                          \
                    /* mcuxCl_Buffer_t pOut,           */ block_amount == SINGLE_BLOCK ? s_MsgEncSingleBlock :         \
                                                                                         s_MsgEnc,                     \
                    /* uint32_t * const pOutSize       */ &msg_enc_size,                                               \
                    /* mcuxCl_Buffer_t pTag,           */ msg_tag,                                                     \
                    /* uint32_t tagSize                */ sizeof(msg_tag)));                                           \
            if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClAead_crypt) != token_enc) || (MCUXCLAEAD_STATUS_OK != result_enc)) \
            {                                                                                                          \
                PRINTF("[Error] Encryption failed\r\n");                                                               \
                return MCUXCLEXAMPLE_STATUS_ERROR;                                                                     \
            }                                                                                                          \
            MCUX_CSSL_FP_FUNCTION_CALL_END();                                                                          \
        }                                                                                                              \
        else /* Data from flash memory */                                                                              \
        {                                                                                                              \
            MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(                                                                          \
                result_enc, token_enc,                                                                                 \
                mcuxClAead_crypt(                                                                                      \
                    /* mcuxClSession_Handle_t session, */ session, /* mcuxClKey_Handle_t key,         */ key,          \
                    /* mcuxClAead_Mode_t mode,         */ mode, /* mcuxCl_InputBuffer_t pNonce,    */ s_Nonce64Flash,  \
                    /* uint32_t nonceSize,             */ sizeof(s_Nonce64Flash),                                      \
                    /* mcuxCl_InputBuffer_t pIn,       */ block_amount == SINGLE_BLOCK ? s_MsgPlainSingleBlockFlash :  \
                                                                                         s_MsgPlainFlash,              \
                    /* uint32_t inSize,                */ block_amount == SINGLE_BLOCK ?                               \
                        sizeof(s_MsgPlainSingleBlockFlash) :                                                           \
                        sizeof(s_MsgPlainFlash),                                                                       \
                    /* mcuxCl_InputBuffer_t pAdata,    */ s_MsgAdataFlash,                                             \
                    /* uint32_t adataSize,             */ sizeof(s_MsgAdataFlash),                                     \
                    /* mcuxCl_Buffer_t pOut,           */ block_amount == SINGLE_BLOCK ? s_MsgEncSingleBlock :         \
                                                                                         s_MsgEnc,                     \
                    /* uint32_t * const pOutSize       */ &msg_enc_size,                                               \
                    /* mcuxCl_Buffer_t pTag,           */ msg_tag,                                                     \
                    /* uint32_t tagSize                */ sizeof(msg_tag)));                                           \
            if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClAead_crypt) != token_enc) || (MCUXCLAEAD_STATUS_OK != result_enc)) \
            {                                                                                                          \
                PRINTF("[Error] Encryption failed\r\n");                                                               \
                return MCUXCLEXAMPLE_STATUS_ERROR;                                                                     \
            }                                                                                                          \
            MCUX_CSSL_FP_FUNCTION_CALL_END();                                                                          \
        }                                                                                                              \
    } while (0);

#define AES_CCM_192(data_from_ram, key, session, block_amount)                                                        \
    do                                                                                                                \
    {                                                                                                                 \
        if (data_from_ram)                                                                                            \
        {                                                                                                             \
            uint32_t msg_enc_size = 0U;                                                                               \
            uint8_t result_buffer[16U];                                                                               \
            MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(                                                                         \
                result_enc, token_enc,                                                                                \
                mcuxClCipher_crypt(                                                                                   \
                    /* mcuxClSession_Handle_t session: */ session, /* mcuxClKey_Handle_t key:         */ key,         \
                    /* mcuxClCipher_Mode_t mode:       */ mcuxClCipher_Mode_AES_CBC_Enc_NoPadding,                    \
                    /* mcuxCl_InputBuffer_t pIv:       */ s_Aes128Iv,                                                 \
                    /* uint32_t ivLength:              */ sizeof(s_Aes128Iv),                                         \
                    /* mcuxCl_InputBuffer_t pIn:       */ block_amount == SINGLE_BLOCK ? s_MsgPlainSingleBlock :      \
                                                                                         s_MsgPlain,                  \
                    /* uint32_t inLength:              */ block_amount == SINGLE_BLOCK ?                              \
                        sizeof(s_MsgPlainSingleBlock) :                                                               \
                        sizeof(s_MsgPlain),                                                                           \
                    /* mcuxCl_Buffer_t pOut:           */ block_amount == SINGLE_BLOCK ? s_MsgEncSingleBlock :        \
                                                                                         s_MsgEnc,                    \
                    /* uint32_t * const pOutLength:    */ &msg_enc_size));                                            \
            if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClCipher_crypt) != token_enc) ||                                    \
                (MCUXCLCIPHER_STATUS_OK != result_enc))                                                               \
            {                                                                                                         \
                PRINTF("[Error] Encryption failed\r\n");                                                              \
                return MCUXCLEXAMPLE_ERROR;                                                                           \
            }                                                                                                         \
            MCUX_CSSL_FP_FUNCTION_CALL_END();                                                                         \
            MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(                                                                         \
                result_enc, token_enc,                                                                                \
                mcuxClCipher_crypt(                                                                                   \
                    /* mcuxClSession_Handle_t session: */ session, /* mcuxClKey_Handle_t key:         */ key,         \
                    /* mcuxClCipher_Mode_t mode:       */ mcuxClCipher_Mode_AES_CTR,                                  \
                    /* mcuxCl_InputBuffer_t pIv:       */ s_Aes128Iv,                                                 \
                    /* uint32_t ivLength:              */ sizeof(s_Aes128Iv),                                         \
                    /* mcuxCl_InputBuffer_t pIn:       */ block_amount == SINGLE_BLOCK ? s_MsgPlainSingleBlock :      \
                                                                                         s_MsgPlain,                  \
                    /* uint32_t inLength:              */ block_amount == SINGLE_BLOCK ?                              \
                        sizeof(s_MsgPlainSingleBlock) :                                                               \
                        sizeof(s_MsgPlain),                                                                           \
                    /* mcuxCl_Buffer_t pOut:           */ block_amount == SINGLE_BLOCK ? s_MsgEncSingleBlock :        \
                                                                                         s_MsgEnc,                    \
                    /* uint32_t * const pOutLength:    */ &msg_enc_size));                                            \
            if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClCipher_crypt) != token_enc) ||                                    \
                (MCUXCLCIPHER_STATUS_OK != result_enc))                                                               \
            {                                                                                                         \
                PRINTF("[Error] Encryption failed\r\n");                                                              \
                return MCUXCLEXAMPLE_ERROR;                                                                           \
            }                                                                                                         \
            MCUX_CSSL_FP_FUNCTION_CALL_END();                                                                         \
        }                                                                                                             \
        else /* Data from flash memory */                                                                             \
        {                                                                                                             \
            uint32_t msg_enc_size = 0U;                                                                               \
            uint8_t result_buffer[16U];                                                                               \
            MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(                                                                         \
                result_enc, token_enc,                                                                                \
                mcuxClCipher_crypt(                                                                                   \
                    /* mcuxClSession_Handle_t session: */ session, /* mcuxClKey_Handle_t key:         */ key,         \
                    /* mcuxClCipher_Mode_t mode:       */ mcuxClCipher_Mode_AES_CBC_Enc_NoPadding,                    \
                    /* mcuxCl_InputBuffer_t pIv:       */ s_Aes128Iv,                                                 \
                    /* uint32_t ivLength:              */ sizeof(s_Aes128IvFlash),                                    \
                    /* mcuxCl_InputBuffer_t pIn:       */ block_amount == SINGLE_BLOCK ? s_MsgPlainSingleBlockFlash : \
                                                                                         s_MsgPlainFlash,             \
                    /* uint32_t inLength:              */ block_amount == SINGLE_BLOCK ?                              \
                        sizeof(s_MsgPlainSingleBlockFlash) :                                                          \
                        sizeof(s_MsgPlainFlash),                                                                      \
                    /* mcuxCl_Buffer_t pOut:           */ block_amount == SINGLE_BLOCK ? s_MsgEncSingleBlock :        \
                                                                                         s_MsgEnc,                    \
                    /* uint32_t * const pOutLength:    */ &msg_enc_size));                                            \
            if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClCipher_crypt) != token_enc) ||                                    \
                (MCUXCLCIPHER_STATUS_OK != result_enc))                                                               \
            {                                                                                                         \
                PRINTF("[Error] Encryption failed\r\n");                                                              \
                return MCUXCLEXAMPLE_ERROR;                                                                           \
            }                                                                                                         \
            MCUX_CSSL_FP_FUNCTION_CALL_END();                                                                         \
            MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(                                                                         \
                result_enc, token_enc,                                                                                \
                mcuxClCipher_crypt(                                                                                   \
                    /* mcuxClSession_Handle_t session: */ session, /* mcuxClKey_Handle_t key:         */ key,         \
                    /* mcuxClCipher_Mode_t mode:       */ mcuxClCipher_Mode_AES_CTR,                                  \
                    /* mcuxCl_InputBuffer_t pIv:       */ s_Aes128IvFlash,                                            \
                    /* uint32_t ivLength:              */ sizeof(s_Aes128IvFlash),                                    \
                    /* mcuxCl_InputBuffer_t pIn:       */ block_amount == SINGLE_BLOCK ? s_MsgPlainSingleBlockFlash : \
                                                                                         s_MsgPlainFlash,             \
                    /* uint32_t inLength:              */ block_amount == SINGLE_BLOCK ?                              \
                        sizeof(s_MsgPlainSingleBlockFlash) :                                                          \
                        sizeof(s_MsgPlainFlash),                                                                      \
                    /* mcuxCl_Buffer_t pOut:           */ block_amount == SINGLE_BLOCK ? s_MsgEncSingleBlock :        \
                                                                                         s_MsgEnc,                    \
                    /* uint32_t * const pOutLength:    */ &msg_enc_size));                                            \
            if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClCipher_crypt) != token_enc) ||                                    \
                (MCUXCLCIPHER_STATUS_OK != result_enc))                                                               \
            {                                                                                                         \
                PRINTF("[Error] Encryption failed\r\n");                                                              \
                return MCUXCLEXAMPLE_ERROR;                                                                           \
            }                                                                                                         \
            MCUX_CSSL_FP_FUNCTION_CALL_END();                                                                         \
        }                                                                                                             \
    } while (0);

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* Variables stored in RAM */

/** 128 bit key for the AES encryption */
static uint8_t s_Aes128Key[16U] = {0x2BU, 0x7EU, 0x15U, 0x16U, 0x28U, 0xAEU, 0xD2U, 0xA6U,
                                   0xABU, 0xF7U, 0x15U, 0x88U, 0x09U, 0xCFU, 0x4FU, 0x3CU};

/** 192 bit key for the AES encryption */
static uint8_t s_Aes192Key[24U] = {0x2BU, 0x7EU, 0x15U, 0x16U, 0x28U, 0xAEU, 0xD2U, 0xA6U, 0xABU, 0xF7U, 0x15U, 0x88U,
                                   0x09U, 0xCFU, 0x4FU, 0x3CU, 0xABU, 0xF7U, 0x15U, 0x88U, 0x09U, 0xCFU, 0x4FU, 0x3CU};

/** 256 bit key for the AES encryption */
static uint8_t s_Aes256Key[32U] = {0x2BU, 0x7EU, 0x15U, 0x16U, 0x28U, 0xAEU, 0xD2U, 0xA6U, 0xABU, 0xF7U, 0x15U,
                                   0x88U, 0x09U, 0xCFU, 0x4FU, 0x3CU, 0x2BU, 0x7EU, 0x15U, 0x16U, 0x28U, 0xAEU,
                                   0xD2U, 0xA6U, 0xABU, 0xF7U, 0x15U, 0x88U, 0x09U, 0xCFU, 0x4FU, 0x3CU};

/** IV of the AES encryption */
static uint8_t s_Aes128Iv[MCUXCLAES_BLOCK_SIZE] = {0xF8U, 0xD2U, 0x68U, 0x76U, 0x81U, 0x6FU, 0x0FU, 0xBAU,
                                                   0x86U, 0x2BU, 0xD8U, 0xA3U, 0x2DU, 0x04U, 0x67U, 0xC3U};

/** Plaintext input for the AES encryption consisting of 1024 blocks */
static uint8_t s_MsgPlain[1024U * MCUXCLAES_BLOCK_SIZE] = {0x00U};

/** Single-block plaintext input for the AES encryption */
static uint8_t s_MsgPlainSingleBlock[MCUXCLAES_BLOCK_SIZE] = {0x2BU, 0x7EU, 0x15U, 0x16U, 0x28U, 0xAEU, 0xD2U, 0xA6U,
                                                              0xABU, 0xF7U, 0x15U, 0x88U, 0x09U, 0xCFU, 0x4FU, 0x3CU};

/** Multi block buffer for the encrypted message */
static uint8_t s_MsgEnc[1024U * MCUXCLAES_BLOCK_SIZE] = {0x00U};

/** Single block buffer for the encrypted message */
static uint8_t s_MsgEncSingleBlock[MCUXCLAES_BLOCK_SIZE] = {0x00U};

/** Associated data for AEAD */
static uint8_t s_MsgAdata[MCUXCLAES_BLOCK_SIZE] = {0x00U, 0x01U, 0x02U, 0x03U, 0x04U, 0x05U, 0x06U, 0x07U,
                                                   0x08U, 0x09U, 0x0AU, 0x0BU, 0x0CU, 0x0DU, 0x0EU, 0x0FU};

/** Nonce for AEAD */
static uint8_t s_Nonce64[8U] = {0x10U, 0x21U, 0x32U, 0x43U, 0x54U, 0x65U, 0x76U, 0x87U};

/* Variables stored in flash */

/** 128 bit key for the AES encryption stored in flash */
static const uint8_t s_Aes128KeyFlash[16U] = {0x2BU, 0x7EU, 0x15U, 0x16U, 0x28U, 0xAEU, 0xD2U, 0xA6U,
                                              0xABU, 0xF7U, 0x15U, 0x88U, 0x09U, 0xCFU, 0x4FU, 0x3CU};

/** 192 bit key for the AES encryption stroed in flash */
static const uint8_t s_Aes192KeyFlash[24U] = {0x2BU, 0x7EU, 0x15U, 0x16U, 0x28U, 0xAEU, 0xD2U, 0xA6U,
                                              0xABU, 0xF7U, 0x15U, 0x88U, 0x09U, 0xCFU, 0x4FU, 0x3CU,
                                              0xABU, 0xF7U, 0x15U, 0x88U, 0x09U, 0xCFU, 0x4FU, 0x3CU};

/** 256 bit key for the AES encryption stored in flash */
static const uint8_t s_Aes256KeyFlash[32U] = {
    0x2BU, 0x7EU, 0x15U, 0x16U, 0x28U, 0xAEU, 0xD2U, 0xA6U, 0xABU, 0xF7U, 0x15U, 0x88U, 0x09U, 0xCFU, 0x4FU, 0x3CU,
    0x2BU, 0x7EU, 0x15U, 0x16U, 0x28U, 0xAEU, 0xD2U, 0xA6U, 0xABU, 0xF7U, 0x15U, 0x88U, 0x09U, 0xCFU, 0x4FU, 0x3CU};

/** IV of the AES encryption stored in flash */
static const uint8_t s_Aes128IvFlash[MCUXCLAES_BLOCK_SIZE] = {0xF8U, 0xD2U, 0x68U, 0x76U, 0x81U, 0x6FU, 0x0FU, 0xBAU,
                                                              0x86U, 0x2BU, 0xD8U, 0xA3U, 0x2DU, 0x04U, 0x67U, 0xC3U};

/*!
 * Plaintext input for the AES encryption consisting of 1024 blocks
 * stored in flash
 */
static const uint8_t s_MsgPlainFlash[1024U * MCUXCLAES_BLOCK_SIZE] = {0x00U};

/** Single-block plaintext input for the AES encryption stored in flash */
static const uint8_t s_MsgPlainSingleBlockFlash[MCUXCLAES_BLOCK_SIZE] = {
    0x2BU, 0x7EU, 0x15U, 0x16U, 0x28U, 0xAEU, 0xD2U, 0xA6U, 0xABU, 0xF7U, 0x15U, 0x88U, 0x09U, 0xCFU, 0x4FU, 0x3CU};

/** Associated data for AEAD stored in flash */
static const uint8_t s_MsgAdataFlash[MCUXCLAES_BLOCK_SIZE] = {0x00U, 0x01U, 0x02U, 0x03U, 0x04U, 0x05U, 0x06U, 0x07U,
                                                              0x08U, 0x09U, 0x0AU, 0x0BU, 0x0CU, 0x0DU, 0x0EU, 0x0FU};

/** Nonce for AEAD stored in flash */
static const uint8_t s_Nonce64Flash[8U] = {0x10U, 0x21U, 0x32U, 0x43U, 0x54U, 0x65U, 0x76U, 0x87U};

/*******************************************************************************
 * Code
 ******************************************************************************/
bool exec_cl_cipher_mode(mcuxClCipher_Mode_t mode,
                         uint32_t block_amount,
                         char *data_from,
                         algorithm_result *a_result,
                         const uint16_t key_size,
                         const bool cache_enable)
{
    /**************************************************************************/
    /* Preparation                                                            */
    bool data_from_ram = !strcmp(data_from, "RAM");

    /* Initialize session */
    mcuxClSession_Descriptor_t sessionDesc;
    mcuxClSession_Handle_t session = &sessionDesc;
    MCUXCLEXAMPLE_ALLOCATE_AND_INITIALIZE_SESSION(session, MCUXCLCIPHER_AES_CRYPT_CPU_WA_BUFFER_SIZE, 0U);

    /* Initialize key */
    uint32_t keyDesc[MCUXCLKEY_DESCRIPTOR_SIZE_IN_WORDS];
    mcuxClKey_Handle_t key = (mcuxClKey_Handle_t)&keyDesc;

    /* Set key properties */
    mcuxClEls_KeyProp_t key_properties;

    key_properties.word.value = 0U;
    key_properties.bits.kactv = MCUXCLELS_KEYPROPERTY_ACTIVE_TRUE;

    /* Load key */
    uint32_t dstData[8U];
    switch (key_size)
    {
        case AES_128_KEY:
        {
            key_properties.bits.ksize = MCUXCLELS_KEYPROPERTY_KEY_SIZE_128;
            if (!mcuxClExample_Key_Init_And_Load(
                    session, key, mcuxClKey_Type_Aes128,
                    data_from_ram ? (mcuxCl_Buffer_t)s_Aes128Key : (mcuxCl_Buffer_t)s_Aes128KeyFlash, key_size,
                    &key_properties, dstData, MCUXCLEXAMPLE_CONST_EXTERNAL_KEY))
            {
                PRINTF("[Error] Key initialization failed\r\n");
                return MCUXCLEXAMPLE_ERROR;
            }
            break;
        }
        case AES_192_KEY:
        {
            if (!mcuxClExample_Key_Init_And_Load(
                    session, key, mcuxClKey_Type_Aes192,
                    data_from_ram ? (mcuxCl_Buffer_t)s_Aes192Key : (mcuxCl_Buffer_t)s_Aes192KeyFlash, key_size,
                    &key_properties, dstData, MCUXCLEXAMPLE_CONST_EXTERNAL_KEY))
            {
                PRINTF("[Error] Key initialization failed\r\n");
                return MCUXCLEXAMPLE_ERROR;
            }
            break;
        }
        case AES_256_KEY:
        {
            key_properties.bits.ksize = MCUXCLELS_KEYPROPERTY_KEY_SIZE_256;
            if (!mcuxClExample_Key_Init_And_Load(
                    session, key, mcuxClKey_Type_Aes256,
                    data_from_ram ? (mcuxCl_Buffer_t)s_Aes256Key : (mcuxCl_Buffer_t)s_Aes256KeyFlash, key_size,
                    &key_properties, dstData, MCUXCLEXAMPLE_CONST_EXTERNAL_KEY))
            {
                PRINTF("[Error] Key initialization failed\r\n");
                return MCUXCLEXAMPLE_ERROR;
            }
            break;
        }
    }

    /**************************************************************************/
    /* Encryption                                                             */
    /**************************************************************************/
    const uint32_t iteration_amount = cache_enable ? 1024U : 1U;
    a_result->cyclesPerBlock =
        COMPUTE_CYCLES(AES_ENCRYPT(mode, data_from_ram, key, session, block_amount), block_amount, iteration_amount);
    a_result->cyclesPerByte = a_result->cyclesPerBlock / 16U;
    a_result->kbPerS        = KB_S(AES_ENCRYPT(mode, data_from_ram, key, session, block_amount), block_amount * 16U);

    /**************************************************************************/
    /* Cleanup                                                                */
    /**************************************************************************/
    /* Flush the key */
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClKey_flush(session, key));

    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClKey_flush) != token) || (MCUXCLKEY_STATUS_OK != result))
    {
        PRINTF("[Error] Key flush failed\r\n");
        return MCUXCLEXAMPLE_ERROR;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    /* Destroy Session and cleanup Session */
    if (!mcuxClExample_Session_Clean(session))
    {
        PRINTF("[Error] Session cleaning failed\r\n");
        return MCUXCLEXAMPLE_ERROR;
    }

    return MCUXCLEXAMPLE_OK;
}

bool exec_cl_aead_mode(mcuxClAead_Mode_t mode,
                       uint32_t block_amount,
                       char *data_from,
                       algorithm_result *a_result,
                       const uint16_t key_size,
                       const bool cache_enable)
{
    /**************************************************************************/
    /* Preparation                                                            */
    /**************************************************************************/
    bool data_from_ram = !strcmp(data_from, "RAM");

    /* Initialize session */
    mcuxClSession_Descriptor_t sessionDesc;
    mcuxClSession_Handle_t session = &sessionDesc;

    /* Allocate and initialize session */
    MCUXCLEXAMPLE_ALLOCATE_AND_INITIALIZE_SESSION(
        session, MCUXCLAEAD_CRYPT_CPU_WA_BUFFER_SIZE + MCUXCLRANDOMMODES_NCINIT_WACPU_SIZE, 0U);

    /* Initialize the PRNG */
    MCUXCLEXAMPLE_INITIALIZE_PRNG(session);

    /* Initialize key */
    uint32_t keyDesc[MCUXCLKEY_DESCRIPTOR_SIZE_IN_WORDS];
    mcuxClKey_Handle_t key = (mcuxClKey_Handle_t)&keyDesc;

    /* Set key properties */
    mcuxClEls_KeyProp_t key_properties;

    key_properties.word.value = 0U;
    key_properties.bits.kactv = MCUXCLELS_KEYPROPERTY_ACTIVE_TRUE;

    uint32_t dstData[32U];

    /* Initializes a key handle, Set key properties and Load key */
    switch (key_size)
    {
        case AES_128_KEY:
        {
            key_properties.bits.ksize = MCUXCLELS_KEYPROPERTY_KEY_SIZE_128;
            if (!mcuxClExample_Key_Init_And_Load(
                    session, key, mcuxClKey_Type_Aes128,
                    data_from_ram ? (mcuxCl_Buffer_t)s_Aes128Key : (mcuxCl_Buffer_t)s_Aes128KeyFlash, key_size,
                    &key_properties, dstData, MCUXCLEXAMPLE_CONST_EXTERNAL_KEY))
            {
                PRINTF("[Error] Key initialization failed\r\n");
                return MCUXCLEXAMPLE_ERROR;
            }
            break;
        }
        case AES_192_KEY:
        {
            if (!mcuxClExample_Key_Init_And_Load(
                    session, key, mcuxClKey_Type_Aes192,
                    data_from_ram ? (mcuxCl_Buffer_t)s_Aes192Key : (mcuxCl_Buffer_t)s_Aes192KeyFlash, key_size,
                    &key_properties, dstData, MCUXCLEXAMPLE_CONST_EXTERNAL_KEY))
            {
                PRINTF("[Error] Key initialization failed\r\n");
                return MCUXCLEXAMPLE_ERROR;
            }
            break;
        }
        case AES_256_KEY:
        {
            key_properties.bits.ksize = MCUXCLELS_KEYPROPERTY_KEY_SIZE_256;
            if (!mcuxClExample_Key_Init_And_Load(
                    session, key, mcuxClKey_Type_Aes256,
                    data_from_ram ? (mcuxCl_Buffer_t)s_Aes256Key : (mcuxCl_Buffer_t)s_Aes256KeyFlash, key_size,
                    &key_properties, dstData, MCUXCLEXAMPLE_CONST_EXTERNAL_KEY))
            {
                PRINTF("[Error] Key initialization failed\r\n");
                return MCUXCLEXAMPLE_ERROR;
            }
            break;
        }
    }

    /**************************************************************************/
    /* Encryption                                                             */
    /**************************************************************************/
    const uint32_t iteration_amount = cache_enable ? 1024U : 1U;
    if (key_size == AES_192_KEY)
    {
        a_result->cyclesPerBlock =
            COMPUTE_CYCLES(AES_CCM_192(data_from_ram, key, session, block_amount), block_amount, iteration_amount);
        a_result->cyclesPerByte = a_result->cyclesPerBlock / 16U;
        a_result->kbPerS        = KB_S(AES_CCM_192(data_from_ram, key, session, block_amount), block_amount * 16U);
    }
    else
    {
        a_result->cyclesPerBlock = COMPUTE_CYCLES(AEAD_ENCRYPT(mode, data_from_ram, key, session, block_amount),
                                                  block_amount, iteration_amount);
        a_result->cyclesPerByte  = a_result->cyclesPerBlock / 16U;
        a_result->kbPerS = KB_S(AEAD_ENCRYPT(mode, data_from_ram, key, session, block_amount), block_amount * 16U);
    }

    /**************************************************************************/
    /* Cleanup                                                                */
    /**************************************************************************/
    /* Flush the key */
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClKey_flush(session, key));

    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClKey_flush) != token) || (MCUXCLKEY_STATUS_OK != result))
    {
        PRINTF("[Error] Key flush failed\r\n");
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    /* Destroy Session and cleanup Session */
    if (!mcuxClExample_Session_Clean(session))
    {
        PRINTF("[Error] Session cleaning failed\r\n");
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    return MCUXCLEXAMPLE_STATUS_OK;
}

void test_aes_cbc(
    char *code_from, char *data_from, uint32_t block_amount, const uint16_t key_size, const bool cache_enable)
{
    if (!strcmp(code_from, "RAM") && !strcmp(data_from, "FLASH"))
        return;
    algorithm_result a_result;
    strcpy(a_result.execution, block_amount == SINGLE_BLOCK ? "SINGLE BLOCK" : "MULTIPLE BLOCKS");
    strcpy(a_result.code, code_from);
    strcpy(a_result.data, data_from);
    exec_cl_cipher_mode(mcuxClCipher_Mode_AES_CBC_Enc_NoPadding, block_amount, data_from, &a_result, key_size,
                        cache_enable);
    strcpy(a_result.cached, cache_enable ? "YES" : "NO");

    PRINT_RESULT(a_result);
}

void test_aes_ecb(
    char *code_from, char *data_from, uint32_t block_amount, const uint16_t key_size, const bool cache_enable)
{
    if (!strcmp(code_from, "RAM") && !strcmp(data_from, "FLASH"))
        return;
    algorithm_result a_result;
    strcpy(a_result.execution, block_amount == SINGLE_BLOCK ? "SINGLE BLOCK" : "MULTIPLE BLOCKS");
    strcpy(a_result.code, code_from);
    strcpy(a_result.data, data_from);
    exec_cl_cipher_mode(mcuxClCipher_Mode_AES_ECB_Enc_NoPadding, block_amount, data_from, &a_result, key_size,
                        cache_enable);
    strcpy(a_result.cached, cache_enable ? "YES" : "NO");

    PRINT_RESULT(a_result);
}

void test_aes_ctr(
    char *code_from, char *data_from, uint32_t block_amount, const uint16_t key_size, const bool cache_enable)
{
    if (!strcmp(code_from, "RAM") && !strcmp(data_from, "FLASH"))
        return;
    algorithm_result a_result;
    strcpy(a_result.execution, block_amount == SINGLE_BLOCK ? "SINGLE BLOCK" : "MULTIPLE BLOCKS");
    strcpy(a_result.code, code_from);
    strcpy(a_result.data, data_from);
    exec_cl_cipher_mode(mcuxClCipher_Mode_AES_CTR, block_amount, data_from, &a_result, key_size, cache_enable);
    strcpy(a_result.cached, cache_enable ? "YES" : "NO");

    PRINT_RESULT(a_result);
}

void test_aes_ccm(
    char *code_from, char *data_from, uint32_t block_amount, const uint16_t key_size, const bool cache_enable)
{
    if (!strcmp(code_from, "RAM") && !strcmp(data_from, "FLASH"))
        return;
    algorithm_result a_result;
    strcpy(a_result.execution, block_amount == SINGLE_BLOCK ? "SINGLE BLOCK" : "MULTIPLE BLOCKS");
    strcpy(a_result.code, code_from);
    strcpy(a_result.data, data_from);
    exec_cl_aead_mode(mcuxClAead_Mode_AES_CCM_ENC, block_amount, data_from, &a_result, key_size, cache_enable);
    strcpy(a_result.cached, cache_enable ? "YES" : "NO");

    PRINT_RESULT(a_result);
}

void test_aes_gcm(
    char *code_from, char *data_from, uint32_t block_amount, const uint16_t key_size, const bool cache_enable)
{
    if (!strcmp(code_from, "RAM") && !strcmp(data_from, "FLASH"))
        return;
    algorithm_result a_result;
    strcpy(a_result.execution, block_amount == SINGLE_BLOCK ? "SINGLE BLOCK" : "MULTIPLE BLOCKS");
    strcpy(a_result.code, code_from);
    strcpy(a_result.data, data_from);
    exec_cl_aead_mode(mcuxClAead_Mode_AES_GCM_ENC, block_amount, data_from, &a_result, key_size, cache_enable);
    strcpy(a_result.cached, cache_enable ? "YES" : "NO");

    PRINT_RESULT(a_result);
}

void run_tests_symmetric(void)
{
    char code_from[6U];
    strcpy(code_from, BOARD_IS_XIP() ? "FLASH" : "RAM");

    PRINTF("AES-CBC-128:\r\n");
    test_aes_cbc(code_from, "FLASH", MULTIPLE_BLOCKS, AES_128_KEY, false);
    test_aes_cbc(code_from, "FLASH", MULTIPLE_BLOCKS, AES_128_KEY, true);
    test_aes_cbc(code_from, "RAM", MULTIPLE_BLOCKS, AES_128_KEY, false);
    test_aes_cbc(code_from, "RAM", MULTIPLE_BLOCKS, AES_128_KEY, true);
    test_aes_cbc(code_from, "FLASH", SINGLE_BLOCK, AES_128_KEY, true);
    test_aes_cbc(code_from, "RAM", SINGLE_BLOCK, AES_128_KEY, true);
    PRINTF("\r\n");

    PRINTF("AES-CBC-192:\r\n");
    test_aes_cbc(code_from, "FLASH", MULTIPLE_BLOCKS, AES_192_KEY, false);
    test_aes_cbc(code_from, "FLASH", MULTIPLE_BLOCKS, AES_192_KEY, true);
    test_aes_cbc(code_from, "RAM", MULTIPLE_BLOCKS, AES_192_KEY, false);
    test_aes_cbc(code_from, "RAM", MULTIPLE_BLOCKS, AES_192_KEY, true);
    test_aes_cbc(code_from, "FLASH", SINGLE_BLOCK, AES_192_KEY, true);
    test_aes_cbc(code_from, "RAM", SINGLE_BLOCK, AES_192_KEY, true);
    PRINTF("\r\n");

    PRINTF("AES-CBC-256:\r\n");
    test_aes_cbc(code_from, "FLASH", MULTIPLE_BLOCKS, AES_256_KEY, false);
    test_aes_cbc(code_from, "FLASH", MULTIPLE_BLOCKS, AES_256_KEY, true);
    test_aes_cbc(code_from, "RAM", MULTIPLE_BLOCKS, AES_256_KEY, false);
    test_aes_cbc(code_from, "RAM", MULTIPLE_BLOCKS, AES_256_KEY, true);
    test_aes_cbc(code_from, "FLASH", SINGLE_BLOCK, AES_256_KEY, true);
    test_aes_cbc(code_from, "RAM", SINGLE_BLOCK, AES_256_KEY, true);
    PRINTF("\r\n");

    PRINTF("AES-ECB-128:\r\n");
    test_aes_ecb(code_from, "FLASH", MULTIPLE_BLOCKS, AES_128_KEY, false);
    test_aes_ecb(code_from, "FLASH", MULTIPLE_BLOCKS, AES_128_KEY, true);
    test_aes_ecb(code_from, "RAM", MULTIPLE_BLOCKS, AES_128_KEY, false);
    test_aes_ecb(code_from, "RAM", MULTIPLE_BLOCKS, AES_128_KEY, true);
    test_aes_ecb(code_from, "FLASH", SINGLE_BLOCK, AES_128_KEY, true);
    test_aes_ecb(code_from, "RAM", SINGLE_BLOCK, AES_128_KEY, true);
    PRINTF("\r\n");

    PRINTF("AES-ECB-192:\r\n");
    test_aes_ecb(code_from, "FLASH", MULTIPLE_BLOCKS, AES_192_KEY, false);
    test_aes_ecb(code_from, "FLASH", MULTIPLE_BLOCKS, AES_192_KEY, true);
    test_aes_ecb(code_from, "RAM", MULTIPLE_BLOCKS, AES_192_KEY, false);
    test_aes_ecb(code_from, "RAM", MULTIPLE_BLOCKS, AES_192_KEY, true);
    test_aes_ecb(code_from, "FLASH", SINGLE_BLOCK, AES_192_KEY, true);
    test_aes_ecb(code_from, "RAM", SINGLE_BLOCK, AES_192_KEY, true);
    PRINTF("\r\n");

    PRINTF("AES-ECB-256:\r\n");
    test_aes_ecb(code_from, "FLASH", MULTIPLE_BLOCKS, AES_256_KEY, false);
    test_aes_ecb(code_from, "FLASH", MULTIPLE_BLOCKS, AES_256_KEY, true);
    test_aes_ecb(code_from, "RAM", MULTIPLE_BLOCKS, AES_256_KEY, false);
    test_aes_ecb(code_from, "RAM", MULTIPLE_BLOCKS, AES_256_KEY, true);
    test_aes_ecb(code_from, "FLASH", SINGLE_BLOCK, AES_256_KEY, true);
    test_aes_ecb(code_from, "RAM", SINGLE_BLOCK, AES_256_KEY, true);
    PRINTF("\r\n");

    PRINTF("AES-CTR-128:\r\n");
    test_aes_ctr(code_from, "FLASH", MULTIPLE_BLOCKS, AES_128_KEY, false);
    test_aes_ctr(code_from, "FLASH", MULTIPLE_BLOCKS, AES_128_KEY, true);
    test_aes_ctr(code_from, "RAM", MULTIPLE_BLOCKS, AES_128_KEY, false);
    test_aes_ctr(code_from, "RAM", MULTIPLE_BLOCKS, AES_128_KEY, true);
    test_aes_ctr(code_from, "FLASH", SINGLE_BLOCK, AES_128_KEY, true);
    test_aes_ctr(code_from, "RAM", SINGLE_BLOCK, AES_128_KEY, true);
    PRINTF("\r\n");

    PRINTF("AES-CTR-192:\r\n");
    test_aes_ctr(code_from, "FLASH", MULTIPLE_BLOCKS, AES_192_KEY, false);
    test_aes_ctr(code_from, "FLASH", MULTIPLE_BLOCKS, AES_192_KEY, true);
    test_aes_ctr(code_from, "RAM", MULTIPLE_BLOCKS, AES_192_KEY, false);
    test_aes_ctr(code_from, "RAM", MULTIPLE_BLOCKS, AES_192_KEY, true);
    test_aes_ctr(code_from, "FLASH", SINGLE_BLOCK, AES_192_KEY, true);
    test_aes_ctr(code_from, "RAM", SINGLE_BLOCK, AES_192_KEY, true);
    PRINTF("\r\n");

    PRINTF("AES-CTR-256:\r\n");
    test_aes_ctr(code_from, "FLASH", MULTIPLE_BLOCKS, AES_256_KEY, false);
    test_aes_ctr(code_from, "FLASH", MULTIPLE_BLOCKS, AES_256_KEY, true);
    test_aes_ctr(code_from, "RAM", MULTIPLE_BLOCKS, AES_256_KEY, false);
    test_aes_ctr(code_from, "RAM", MULTIPLE_BLOCKS, AES_256_KEY, true);
    test_aes_ctr(code_from, "FLASH", SINGLE_BLOCK, AES_256_KEY, true);
    test_aes_ctr(code_from, "RAM", SINGLE_BLOCK, AES_256_KEY, true);
    PRINTF("\r\n");

    PRINTF("AES-CCM-128(WITH CMAC):\r\n");
    test_aes_ccm(code_from, "FLASH", MULTIPLE_BLOCKS, AES_128_KEY, false);
    test_aes_ccm(code_from, "FLASH", MULTIPLE_BLOCKS, AES_128_KEY, true);
    test_aes_ccm(code_from, "RAM", MULTIPLE_BLOCKS, AES_128_KEY, false);
    test_aes_ccm(code_from, "RAM", MULTIPLE_BLOCKS, AES_128_KEY, true);
    test_aes_ccm(code_from, "FLASH", SINGLE_BLOCK, AES_128_KEY, true);
    test_aes_ccm(code_from, "RAM", SINGLE_BLOCK, AES_128_KEY, true);
    PRINTF("\r\n");

    PRINTF("AES-CCM-192(1.CBC 2.CTR):\r\n");
    test_aes_ccm(code_from, "FLASH", MULTIPLE_BLOCKS, AES_192_KEY, false);
    test_aes_ccm(code_from, "FLASH", MULTIPLE_BLOCKS, AES_192_KEY, true);
    test_aes_ccm(code_from, "RAM", MULTIPLE_BLOCKS, AES_192_KEY, false);
    test_aes_ccm(code_from, "RAM", MULTIPLE_BLOCKS, AES_192_KEY, true);
    test_aes_ccm(code_from, "FLASH", SINGLE_BLOCK, AES_192_KEY, true);
    test_aes_ccm(code_from, "RAM", SINGLE_BLOCK, AES_192_KEY, true);
    PRINTF("\r\n");

    PRINTF("AES-CCM-256(WITH CMAC):\r\n");
    test_aes_ccm(code_from, "FLASH", MULTIPLE_BLOCKS, AES_256_KEY, false);
    test_aes_ccm(code_from, "FLASH", MULTIPLE_BLOCKS, AES_256_KEY, true);
    test_aes_ccm(code_from, "RAM", MULTIPLE_BLOCKS, AES_256_KEY, false);
    test_aes_ccm(code_from, "RAM", MULTIPLE_BLOCKS, AES_256_KEY, true);
    test_aes_ccm(code_from, "FLASH", SINGLE_BLOCK, AES_256_KEY, true);
    test_aes_ccm(code_from, "RAM", SINGLE_BLOCK, AES_256_KEY, true);
    PRINTF("\r\n");

    PRINTF("AES-GCM-128:\r\n");
    test_aes_gcm(code_from, "FLASH", MULTIPLE_BLOCKS, AES_128_KEY, false);
    test_aes_gcm(code_from, "FLASH", MULTIPLE_BLOCKS, AES_128_KEY, true);
    test_aes_gcm(code_from, "RAM", MULTIPLE_BLOCKS, AES_128_KEY, false);
    test_aes_gcm(code_from, "RAM", MULTIPLE_BLOCKS, AES_128_KEY, true);
    test_aes_gcm(code_from, "FLASH", SINGLE_BLOCK, AES_128_KEY, true);
    test_aes_gcm(code_from, "RAM", SINGLE_BLOCK, AES_128_KEY, true);
    PRINTF("\r\n");

    PRINTF("AES-GCM-192:\r\n");
    test_aes_gcm(code_from, "FLASH", MULTIPLE_BLOCKS, AES_192_KEY, false);
    test_aes_gcm(code_from, "FLASH", MULTIPLE_BLOCKS, AES_192_KEY, true);
    test_aes_gcm(code_from, "RAM", MULTIPLE_BLOCKS, AES_192_KEY, false);
    test_aes_gcm(code_from, "RAM", MULTIPLE_BLOCKS, AES_192_KEY, true);
    test_aes_gcm(code_from, "FLASH", SINGLE_BLOCK, AES_192_KEY, true);
    test_aes_gcm(code_from, "RAM", SINGLE_BLOCK, AES_192_KEY, true);
    PRINTF("\r\n");

    PRINTF("AES-GCM-256:\r\n");
    test_aes_gcm(code_from, "FLASH", MULTIPLE_BLOCKS, AES_256_KEY, false);
    test_aes_gcm(code_from, "FLASH", MULTIPLE_BLOCKS, AES_256_KEY, true);
    test_aes_gcm(code_from, "RAM", MULTIPLE_BLOCKS, AES_256_KEY, false);
    test_aes_gcm(code_from, "RAM", MULTIPLE_BLOCKS, AES_256_KEY, true);
    test_aes_gcm(code_from, "FLASH", SINGLE_BLOCK, AES_256_KEY, true);
    test_aes_gcm(code_from, "RAM", SINGLE_BLOCK, AES_256_KEY, true);
    PRINTF("\r\n");
}
