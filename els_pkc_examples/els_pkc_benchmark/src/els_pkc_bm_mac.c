/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "els_pkc_bm_mac.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define SHA_256 256U
#define SHA_512 512U

#define COMPUTE_CMAC(session, key, block_amount, data_from_ram)                                                    \
    do                                                                                                             \
    {                                                                                                              \
        /* Output buffer for the computed MAC. */                                                                  \
        static uint8_t result_buffer[MCUXCLELS_CMAC_OUT_SIZE];                                                     \
        if (data_from_ram)                                                                                         \
        {                                                                                                          \
            /* Call the mcuxClMac_compute function to compute a CMAC in one shot. */                               \
            uint32_t result_size = 0U;                                                                             \
            MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(                                                                      \
                result, token,                                                                                     \
                mcuxClMac_compute(                                                                                 \
                    /* mcuxClSession_Handle_t session:  */ session, /* const mcuxClKey_Handle_t key:    */ key,    \
                    /* const mcuxClMac_Mode_t mode:     */ mcuxClMac_Mode_CMAC,                                    \
                    /* mcuxCl_InputBuffer_t pIn:        */ block_amount == SINGLE_BLOCK ? s_CmacInputSingleBlock : \
                                                                                          s_CmacInput,             \
                    /* uint32_t inLength:               */ block_amount == SINGLE_BLOCK ?                          \
                        sizeof(s_CmacInputSingleBlock) :                                                           \
                        sizeof(s_CmacInput),                                                                       \
                    /* mcuxCl_Buffer_t pMac:            */ result_buffer,                                          \
                    /* uint32_t * const pMacLength:     */ &result_size));                                         \
            if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClMac_compute) != token) || (MCUXCLMAC_STATUS_OK != result))     \
            {                                                                                                      \
                PRINTF("[Error] CMAC failed\r\n");                                                                 \
                return MCUXCLEXAMPLE_STATUS_ERROR;                                                                 \
            }                                                                                                      \
            MCUX_CSSL_FP_FUNCTION_CALL_END();                                                                      \
        }                                                                                                          \
        else                                                                                                       \
        {                                                                                                          \
            /* Call the mcuxClMac_compute function to compute a CMAC in one shot. */                               \
            uint32_t result_size = 0U;                                                                             \
            MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(                                                                      \
                result, token,                                                                                     \
                mcuxClMac_compute(/* mcuxClSession_Handle_t session:  */ session,                                  \
                                  /* const mcuxClKey_Handle_t key:    */ key,                                      \
                                  /* const mcuxClMac_Mode_t mode:     */ mcuxClMac_Mode_CMAC,                      \
                                  /* mcuxCl_InputBuffer_t pIn:        */ block_amount == SINGLE_BLOCK ?            \
                                      s_CmacInputSingleBlockFlash :                                                \
                                      s_CmacInputFlash,                                                            \
                                  /* uint32_t inLength:               */ block_amount == SINGLE_BLOCK ?            \
                                      sizeof(s_CmacInputSingleBlockFlash) :                                        \
                                      sizeof(s_CmacInputFlash),                                                    \
                                  /* mcuxCl_Buffer_t pMac:            */ result_buffer,                            \
                                  /* uint32_t * const pMacLength:     */ &result_size));                           \
            if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClMac_compute) != token) || (MCUXCLMAC_STATUS_OK != result))     \
            {                                                                                                      \
                PRINTF("[Error] CMAC failed\r\n");                                                                 \
                return MCUXCLEXAMPLE_STATUS_ERROR;                                                                 \
            }                                                                                                      \
            MCUX_CSSL_FP_FUNCTION_CALL_END();                                                                      \
        }                                                                                                          \
    } while (0)

#define COMPUTE_HMAC(session, key, block_amount, data_from_ram)                                                 \
    do                                                                                                          \
    {                                                                                                           \
        /* Output buffer for the computed MAC. */                                                               \
        static uint8_t result_buffer[MCUXCLHMAC_MAX_OUTPUT_SIZE];                                               \
        if (data_from_ram)                                                                                      \
        {                                                                                                       \
            /* Call the mcuxClMac_compute function to compute a HMAC in one shot. */                            \
            uint32_t result_size = 0U;                                                                          \
            MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(                                                                   \
                macCompute_result, macCompute_token,                                                            \
                mcuxClMac_compute(/* mcuxClSession_Handle_t session:  */ session,                               \
                                  /* const mcuxClKey_Handle_t key:    */ key,                                   \
                                  /* const mcuxClMac_Mode_t mode:     */ mode,                                  \
                                  /* mcuxCl_InputBuffer_t pIn:        */ block_amount == SINGLE_BLOCK ?         \
                                      (uint8_t *)s_HmacInputSingleBlock :                                       \
                                      (uint8_t *)s_HmacInput, /* No extra space for padding is required */      \
                                  /* uint32_t inLength:               */ block_amount == SINGLE_BLOCK ?         \
                                      sizeof(s_HmacInputSingleBlock) :                                          \
                                      sizeof(s_HmacInput),                                                      \
                                  /* mcuxCl_Buffer_t pMac:            */ result_buffer,                         \
                                  /* uint32_t * const pMacLength:     */ &result_size));                        \
            if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClMac_compute) != macCompute_token) ||                        \
                (MCUXCLMAC_STATUS_OK != macCompute_result))                                                     \
            {                                                                                                   \
                PRINTF("[Error] HMAC failed\r\n");                                                              \
                return MCUXCLEXAMPLE_STATUS_ERROR;                                                              \
            }                                                                                                   \
            MCUX_CSSL_FP_FUNCTION_CALL_END();                                                                   \
        }                                                                                                       \
        else                                                                                                    \
        {                                                                                                       \
            /* Call the mcuxClMac_compute function to compute a HMAC in one shot. */                            \
            uint32_t result_size = 0U;                                                                          \
            MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(                                                                   \
                macCompute_result, macCompute_token,                                                            \
                mcuxClMac_compute(/* mcuxClSession_Handle_t session:  */ session,                               \
                                  /* const mcuxClKey_Handle_t key:    */ key,                                   \
                                  /* const mcuxClMac_Mode_t mode:     */ mode,                                  \
                                  /* mcuxCl_InputBuffer_t pIn:        */ block_amount == SINGLE_BLOCK ?         \
                                      (uint8_t *)s_HmacInputSingleBlockFlash :                                  \
                                      (uint8_t *)s_HmacInputFlash, /* No extra space for padding is required */ \
                                  /* uint32_t inLength:               */ block_amount == SINGLE_BLOCK ?         \
                                      sizeof(s_HmacInputSingleBlockFlash) :                                     \
                                      sizeof(s_HmacInputFlash),                                                 \
                                  /* mcuxCl_Buffer_t pMac:            */ result_buffer,                         \
                                  /* uint32_t * const pMacLength:     */ &result_size));                        \
            if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClMac_compute) != macCompute_token) ||                        \
                (MCUXCLMAC_STATUS_OK != macCompute_result))                                                     \
            {                                                                                                   \
                PRINTF("[Error] HMAC failed\r\n");                                                              \
                return MCUXCLEXAMPLE_STATUS_ERROR;                                                              \
            }                                                                                                   \
            MCUX_CSSL_FP_FUNCTION_CALL_END();                                                                   \
        }                                                                                                       \
    } while (0)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
bool exec_cmac(uint32_t block_amount,
               char *data_from,
               algorithm_result *a_result,
               const uint16_t key_size,
               const bool cache_enable)
{
    /* Example AES-128 key */
    static uint8_t s_Key128[MCUXCLAES_AES128_KEY_SIZE] = {0x7CU, 0x0BU, 0x7DU, 0xB9U, 0x81U, 0x1FU, 0x10U, 0xD0U,
                                                          0x0EU, 0x47U, 0x6CU, 0x7AU, 0x0DU, 0x92U, 0xF6U, 0xE0U};
    /* Example AES-256 key */
    static uint8_t s_Key256[MCUXCLAES_AES256_KEY_SIZE] = {
        0x7CU, 0x0BU, 0x7DU, 0xB9U, 0x81U, 0x1FU, 0x10U, 0xD0U, 0x0EU, 0x47U, 0x6CU, 0x7AU, 0x0DU, 0x92U, 0xF6U, 0xE0U,
        0x7CU, 0x0BU, 0x7DU, 0xB9U, 0x81U, 0x1FU, 0x10U, 0xD0U, 0x0EU, 0x47U, 0x6CU, 0x7AU, 0x0DU, 0x92U, 0xF6U, 0xE0U};
    /* Example input to the CMAC function */
    static uint8_t s_CmacInputSingleBlock[MCUXCLAES_BLOCK_SIZE] = {
        0x1EU, 0xE0U, 0xECU, 0x46U, 0x6DU, 0x46U, 0xFDU, 0x84U, 0x9BU, 0x40U, 0xC0U, 0x66U, 0xB4U, 0xFBU, 0xBDU, 0x22U};
    /* Example multi-block input to the CMAC function */
    static uint8_t s_CmacInput[1024U * MCUXCLAES_BLOCK_SIZE];

    /* Example AES-128 key stored in flash */
    static const uint8_t s_Key128Flash[MCUXCLAES_AES128_KEY_SIZE] = {
        0x7CU, 0x0BU, 0x7DU, 0xB9U, 0x81U, 0x1FU, 0x10U, 0xD0U, 0x0EU, 0x47U, 0x6CU, 0x7AU, 0x0DU, 0x92U, 0xF6U, 0xE0U};
    /* Example AES-256 key stored in flash */
    static const uint8_t s_Key256Flash[MCUXCLAES_AES256_KEY_SIZE] = {
        0x7CU, 0x0BU, 0x7DU, 0xB9U, 0x81U, 0x1FU, 0x10U, 0xD0U, 0x0EU, 0x47U, 0x6CU, 0x7AU, 0x0DU, 0x92U, 0xF6U, 0xE0U,
        0x7CU, 0x0BU, 0x7DU, 0xB9U, 0x81U, 0x1FU, 0x10U, 0xD0U, 0x0EU, 0x47U, 0x6CU, 0x7AU, 0x0DU, 0x92U, 0xF6U, 0xE0U};
    /* Example single block input to the CMAC function stored in flash */
    static const uint8_t s_CmacInputSingleBlockFlash[MCUXCLAES_BLOCK_SIZE] = {
        0x1EU, 0xE0U, 0xECU, 0x46U, 0x6DU, 0x46U, 0xFDU, 0x84U, 0x9BU, 0x40U, 0xC0U, 0x66U, 0xB4U, 0xFBU, 0xBDU, 0x22U};
    /* Example multi-block input to the CMAC function stored in flash */
    static const uint8_t s_CmacInputFlash[1024U * MCUXCLAES_BLOCK_SIZE] = {0x00U};

    /**************************************************************************/
    /* Preparation                                                            */
    /**************************************************************************/
    bool data_from_ram = !strcmp(data_from, "RAM");

    /* Key buffer for the key in memory. */
    uint32_t key_buffer[32U];

    mcuxClSession_Descriptor_t sessionDesc;
    mcuxClSession_Handle_t session = &sessionDesc;

    /* Allocate and initialize session */
    MCUXCLEXAMPLE_ALLOCATE_AND_INITIALIZE_SESSION(
        session, MCUXCLMAC_MAX_CPU_WA_BUFFER_SIZE + MCUXCLRANDOMMODES_NCINIT_WACPU_SIZE, 0U);

    /* Initialize the PRNG */
    MCUXCLEXAMPLE_INITIALIZE_PRNG(session);

    /**************************************************************************/
    /* Key setup                                                              */
    /**************************************************************************/
    /* Create and initialize mcuxClKey_Descriptor_t structure. */
    uint32_t keyDesc[MCUXCLKEY_DESCRIPTOR_SIZE_IN_WORDS];
    mcuxClKey_Handle_t key = (mcuxClKey_Handle_t)&keyDesc;

    mcuxClEls_KeyProp_t cmac_key_properties;
    cmac_key_properties.word.value = 0U;
    cmac_key_properties.bits.ucmac = MCUXCLELS_KEYPROPERTY_CMAC_TRUE;
    cmac_key_properties.bits.kactv = MCUXCLELS_KEYPROPERTY_ACTIVE_TRUE;

    switch (key_size)
    {
        case MCUXCLAES_AES128_KEY_SIZE:
        {
            cmac_key_properties.bits.ksize = MCUXCLELS_KEYPROPERTY_KEY_SIZE_128;
            if (!mcuxClExample_Key_Init_And_Load(
                    session, key, mcuxClKey_Type_Aes128,
                    data_from_ram ? (mcuxCl_Buffer_t)s_Key128 : (mcuxCl_Buffer_t)s_Key128Flash,
                    data_from_ram ? sizeof(s_Key128) : sizeof(s_Key128Flash), &cmac_key_properties, key_buffer,
                    MCUXCLEXAMPLE_CONST_EXTERNAL_KEY))
            {
                PRINTF("[Error] Key initialization failed\r\n");
                return MCUXCLEXAMPLE_STATUS_ERROR;
            }
            break;
        }
        case MCUXCLAES_AES256_KEY_SIZE:
        {
            cmac_key_properties.bits.ksize = MCUXCLELS_KEYPROPERTY_KEY_SIZE_256;
            if (!mcuxClExample_Key_Init_And_Load(
                    session, key, mcuxClKey_Type_Aes256,
                    data_from_ram ? (mcuxCl_Buffer_t)s_Key256 : (mcuxCl_Buffer_t)s_Key256Flash,
                    data_from_ram ? sizeof(s_Key256) : sizeof(s_Key256Flash), &cmac_key_properties, key_buffer,
                    MCUXCLEXAMPLE_CONST_EXTERNAL_KEY))
            {
                PRINTF("[Error] Key initialization failed\r\n");
                return MCUXCLEXAMPLE_STATUS_ERROR;
            }
            break;
        }
    }

    /**************************************************************************/
    /* MAC computation                                                        */
    /**************************************************************************/
    const uint32_t iteration_amount = cache_enable ? 1024U : 1U;
    a_result->cyclesPerBlock =
        COMPUTE_CYCLES(COMPUTE_CMAC(session, key, block_amount, data_from_ram), block_amount, iteration_amount);
    a_result->cyclesPerByte = a_result->cyclesPerBlock / 16U;
    a_result->kbPerS        = KB_S(COMPUTE_CMAC(session, key, block_amount, data_from_ram), block_amount * 16U);

    /**************************************************************************/
    /* Cleanup                                                                */
    /**************************************************************************/
    /* Flush the key. */
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClKey_flush(session, key));

    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClKey_flush) != token) || (MCUXCLKEY_STATUS_OK != result))
    {
        PRINTF("[Error] Key flush failed\r\n");
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    /** Destroy Session and cleanup Session **/
    if (!mcuxClExample_Session_Clean(session))
    {
        PRINTF("[Error] Session cleaning failed\r\n");
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    return MCUXCLEXAMPLE_STATUS_OK;
}

bool exec_hmac(uint32_t block_amount,
               char *data_from,
               algorithm_result *a_result,
               const uint16_t sha_type,
               const bool cache_enable)
{
    /* Example (unpadded) key */
    static uint8_t s_Key[] = {0x00U, 0x11U, 0x22U, 0x33U, 0x44U, 0x55U, 0x66U, 0x77U, 0x88U, 0x99U, 0xAAU,
                              0xBBU, 0xCCU, 0xDDU, 0xEEU, 0xFFU, 0x00U, 0x11U, 0x22U, 0x33U, 0x44U, 0x55U,
                              0x66U, 0x77U, 0x88U, 0x99U, 0xAAU, 0xBBU, 0xCCU, 0xDDU, 0xEEU, 0xFFU};
    /* Example multi-block input to the HMAC function */
    static uint8_t s_HmacInput[1024U * 128U];
    /* Example single block input to the HMAC function */
    static uint8_t s_HmacInputSingleBlock[128U] = {0x00U, 0x9FU, 0x5EU, 0x39U, 0x94U, 0x30U, 0x03U, 0x82U,
                                                   0x50U, 0x72U, 0x1BU, 0xE1U, 0x79U, 0x65U, 0x35U, 0xFFU};

    /* Example (unpadded) key stored in flash */
    static const uint8_t s_KeyFlash[] = {0x00U, 0x11U, 0x22U, 0x33U, 0x44U, 0x55U, 0x66U, 0x77U, 0x88U, 0x99U, 0xAAU,
                                         0xBBU, 0xCCU, 0xDDU, 0xEEU, 0xFFU, 0x00U, 0x11U, 0x22U, 0x33U, 0x44U, 0x55U,
                                         0x66U, 0x77U, 0x88U, 0x99U, 0xAAU, 0xBBU, 0xCCU, 0xDDU, 0xEEU, 0xFFU};
    /* Example single block input to the HMAC function stored in flash */
    static const uint8_t s_HmacInputSingleBlockFlash[128U] = {0x00U, 0x9FU, 0x5EU, 0x39U, 0x94U, 0x30U, 0x03U, 0x82U,
                                                              0x50U, 0x72U, 0x1BU, 0xE1U, 0x79U, 0x65U, 0x35U, 0xFFU};
    /* Example multi-block input to the HMAC function stored in flash */
    static const uint8_t s_HmacInputFlash[1024U * 128U] = {0x00U};

    /**************************************************************************/
    /* Preparation                                                            */
    /**************************************************************************/
    bool data_from_ram = !strcmp(data_from, "RAM");

    /* Key buffer for the key in memory. */
    uint32_t key_buffer[sizeof(s_Key) / sizeof(uint32_t)];

    /* Allocate and initialize session / workarea */
    mcuxClSession_Descriptor_t sessionDesc;
    mcuxClSession_Handle_t session = &sessionDesc;

    /* Allocate and initialize session / workarea */
    MCUXCLEXAMPLE_ALLOCATE_AND_INITIALIZE_SESSION(
        session, MCUXCLHMAC_MAX_CPU_WA_BUFFER_SIZE + MCUXCLRANDOMMODES_NCINIT_WACPU_SIZE, 0U);

    /* Initialize the PRNG */
    MCUXCLEXAMPLE_INITIALIZE_PRNG(session);

    /**************************************************************************/
    /* Key setup                                                              */
    /**************************************************************************/

    /* Create and initialize mcuxClKey_Descriptor_t structure. */
    uint32_t keyDesc[MCUXCLKEY_DESCRIPTOR_SIZE_IN_WORDS];
    mcuxClKey_Handle_t key = (mcuxClKey_Handle_t)&keyDesc;

    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(
        keyInit_result, keyInit_token,
        mcuxClKey_init(
            /* mcuxClSession_Handle_t pSession:                */ session,
            /* mcuxClKey_Handle_t key:                         */ key,
            /* const mcuxClKey_Type* type:                     */ mcuxClKey_Type_Hmac_variableLength,
            /* mcuxCl_Buffer_t pKeyData:                       */
            data_from_ram ? (uint8_t *)s_Key : (uint8_t *)s_KeyFlash,
            /* uint32_t keyDataLength:                        */ data_from_ram ? sizeof(s_Key) : sizeof(s_KeyFlash)));

    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClKey_init) != keyInit_token) || (MCUXCLKEY_STATUS_OK != keyInit_result))
    {
        PRINTF("[Error] Key initialization failed\r\n");
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    /* Load key to memory. */
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(keyLoad_result, keyLoad_token, mcuxClKey_loadMemory(session, key, key_buffer));

    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClKey_loadMemory) != keyLoad_token) ||
        (MCUXCLKEY_STATUS_OK != keyLoad_result))
    {
        PRINTF("[Error] Loading key to memory failed\r\n");
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    /**************************************************************************/
    /* Generate an HMAC mode containing the hash algorithm                    */
    /**************************************************************************/
    uint8_t hmacModeDescBuffer[MCUXCLHMAC_HMAC_MODE_DESCRIPTOR_SIZE];
    mcuxClMac_CustomMode_t mode = (mcuxClMac_CustomMode_t)hmacModeDescBuffer;

    if (sha_type == SHA_256)
    {
        MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(hashCreateMode_result, hashCreateMode_token,
                                         mcuxClHmac_createHmacMode(
                                             /* mcuxClMac_CustomMode_t mode:       */ mode,
                                             /* mcuxClHash_Algo_t hashAlgorithm:   */ mcuxClHash_Algorithm_Sha256));

        if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClHmac_createHmacMode) != hashCreateMode_token) ||
            (MCUXCLMAC_STATUS_OK != hashCreateMode_result))
        {
            PRINTF("[Error] HMAC mode generation failed\r\n");
            return MCUXCLEXAMPLE_STATUS_ERROR;
        }
        MCUX_CSSL_FP_FUNCTION_CALL_END();
    }
    else if (sha_type == SHA_512)
    {
        MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(hashCreateMode_result, hashCreateMode_token,
                                         mcuxClHmac_createHmacMode(
                                             /* mcuxClMac_CustomMode_t mode:       */ mode,
                                             /* mcuxClHash_Algo_t hashAlgorithm:   */ mcuxClHash_Algorithm_Sha512));
        if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClHmac_createHmacMode) != hashCreateMode_token) ||
            (MCUXCLMAC_STATUS_OK != hashCreateMode_result))
        {
            PRINTF("[Error] HMAC mode generation failed\r\n");
            return MCUXCLEXAMPLE_STATUS_ERROR;
        }
        MCUX_CSSL_FP_FUNCTION_CALL_END();
    }

    /**************************************************************************/
    /* HMAC computation                                                       */
    /**************************************************************************/
    const uint32_t iteration_amount = cache_enable ? 1024U : 1U;
    a_result->cyclesPerBlock =
        COMPUTE_CYCLES(COMPUTE_HMAC(session, key, block_amount, data_from_ram), block_amount, iteration_amount);
    a_result->cyclesPerByte = a_result->cyclesPerBlock / 128U;
    a_result->kbPerS        = KB_S(COMPUTE_HMAC(session, key, block_amount, data_from_ram), block_amount * 128U);

    /**************************************************************************/
    /* Cleanup                                                                */
    /**************************************************************************/
    /* Flush the key. */
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(keyFlush_result, keyFlush_token, mcuxClKey_flush(session, key));
    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClKey_flush) != keyFlush_token) || (MCUXCLKEY_STATUS_OK != keyFlush_result))
    {
        PRINTF("[Error] Key flush failed\r\n");
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    /* Clean-up and destroy the session. */
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(sessionCleanup_result, sessionCleanup_token,
                                     mcuxClSession_cleanup(
                                         /* mcuxClSession_Handle_t           pSession: */ session));
    if (MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClSession_cleanup) != sessionCleanup_token ||
        MCUXCLSESSION_STATUS_OK != sessionCleanup_result)
    {
        PRINTF("[Error] Session cleaning failed\r\n");
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(sessionDestroy_result, sessionDestroy_token,
                                     mcuxClSession_destroy(
                                         /* mcuxClSession_Handle_t           pSession: */ session));
    if (MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClSession_destroy) != sessionDestroy_token ||
        MCUXCLSESSION_STATUS_OK != sessionDestroy_result)
    {
        PRINTF("[Error] Session destroy failed\r\n");
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    return MCUXCLEXAMPLE_STATUS_OK;
}

void test_cmac(
    char *code_from, char *data_from, uint32_t block_amount, const uint16_t key_size, const bool cache_enable)
{
    if (!strcmp(code_from, "RAM") && !strcmp(data_from, "FLASH"))
        return;
    algorithm_result a_result;
    strcpy(a_result.execution, block_amount == SINGLE_BLOCK ? "SINGLE BLOCK" : "MULTIPLE BLOCKS");
    strcpy(a_result.code, code_from);
    strcpy(a_result.data, data_from);
    exec_cmac(block_amount, data_from, &a_result, key_size, cache_enable);
    strcpy(a_result.cached, cache_enable ? "YES" : "NO");

    PRINT_RESULT(a_result);
}

void test_hmac(
    char *code_from, char *data_from, uint32_t block_amount, const uint16_t sha_type, const bool cache_enable)
{
    if (!strcmp(code_from, "RAM") && !strcmp(data_from, "FLASH"))
        return;
    algorithm_result a_result;
    strcpy(a_result.execution, block_amount == SINGLE_BLOCK ? "SINGLE BLOCK" : "MULTIPLE BLOCKS");
    strcpy(a_result.code, code_from);
    strcpy(a_result.data, data_from);
    exec_hmac(block_amount, data_from, &a_result, sha_type, cache_enable);
    strcpy(a_result.cached, cache_enable ? "YES" : "NO");

    PRINT_RESULT(a_result);
}

void run_tests_mac(void)
{
    char code_from[6U];
    strcpy(code_from, BOARD_IS_XIP() ? "FLASH" : "RAM");

    PRINTF("AES-CMAC-128:\r\n");
    test_cmac(code_from, "FLASH", MULTIPLE_BLOCKS, MCUXCLAES_AES128_KEY_SIZE, false);
    test_cmac(code_from, "FLASH", MULTIPLE_BLOCKS, MCUXCLAES_AES128_KEY_SIZE, true);
    test_cmac(code_from, "RAM", MULTIPLE_BLOCKS, MCUXCLAES_AES128_KEY_SIZE, false);
    test_cmac(code_from, "RAM", MULTIPLE_BLOCKS, MCUXCLAES_AES128_KEY_SIZE, true);
    test_cmac(code_from, "FLASH", SINGLE_BLOCK, MCUXCLAES_AES128_KEY_SIZE, true);
    test_cmac(code_from, "RAM", SINGLE_BLOCK, MCUXCLAES_AES128_KEY_SIZE, true);
    PRINTF("\r\n");

    PRINTF("AES-CMAC-256:\r\n");
    test_cmac(code_from, "FLASH", MULTIPLE_BLOCKS, MCUXCLAES_AES256_KEY_SIZE, false);
    test_cmac(code_from, "FLASH", MULTIPLE_BLOCKS, MCUXCLAES_AES256_KEY_SIZE, true);
    test_cmac(code_from, "RAM", MULTIPLE_BLOCKS, MCUXCLAES_AES256_KEY_SIZE, false);
    test_cmac(code_from, "RAM", MULTIPLE_BLOCKS, MCUXCLAES_AES256_KEY_SIZE, true);
    test_cmac(code_from, "FLASH", SINGLE_BLOCK, MCUXCLAES_AES256_KEY_SIZE, true);
    test_cmac(code_from, "RAM", SINGLE_BLOCK, MCUXCLAES_AES256_KEY_SIZE, true);
    PRINTF("\r\n");

    PRINTF("HMAC-SHA-256:\r\n");
    test_hmac(code_from, "FLASH", MULTIPLE_BLOCKS, SHA_256, false);
    test_hmac(code_from, "FLASH", MULTIPLE_BLOCKS, SHA_256, true);
    test_hmac(code_from, "RAM", MULTIPLE_BLOCKS, SHA_256, false);
    test_hmac(code_from, "RAM", MULTIPLE_BLOCKS, SHA_256, true);
    test_hmac(code_from, "FLASH", SINGLE_BLOCK, SHA_256, true);
    test_hmac(code_from, "RAM", SINGLE_BLOCK, SHA_256, true);
    PRINTF("\r\n");

    PRINTF("HMAC-SHA-512:\r\n");
    test_hmac(code_from, "FLASH", MULTIPLE_BLOCKS, SHA_512, false);
    test_hmac(code_from, "FLASH", MULTIPLE_BLOCKS, SHA_512, true);
    test_hmac(code_from, "RAM", MULTIPLE_BLOCKS, SHA_512, false);
    test_hmac(code_from, "RAM", MULTIPLE_BLOCKS, SHA_512, true);
    test_hmac(code_from, "FLASH", SINGLE_BLOCK, SHA_512, true);
    test_hmac(code_from, "RAM", SINGLE_BLOCK, SHA_512, true);
    PRINTF("\r\n");
}
