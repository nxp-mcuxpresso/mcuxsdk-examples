/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "els_pkc_bm_kdf.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define KEY_SIZE_256 256U
#define KEY_SIZE_128 128U

#define CKDF_SP800108(key_properties, delete_target_index)                                                            \
    do                                                                                                                \
    {                                                                                                                 \
        if (delete_target_index)                                                                                      \
        {                                                                                                             \
            DELETE_KEY(0x0EU);                                                                                        \
        }                                                                                                             \
        uint8_t der_data[12U] = {0x00U, 0x65U, 0x32U, 0x67U, 0x63U, 0x6FU, 0x6EU, 0x5FU, 0x73U, 0x65U, 0x00U, 0x00U}; \
        MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(resultCkdf, tokenAsync,                                                      \
                                         mcuxClEls_Ckdf_Sp800108_Async(0x06U, 0x0EU, key_properties, der_data));      \
        if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_Ckdf_Sp800108_Async) != tokenAsync) ||                            \
            (MCUXCLELS_STATUS_OK_WAIT != resultCkdf))                                                                 \
        {                                                                                                             \
            PRINTF("[Error] ckdf-sp800108 failed\r\n");                                                               \
            return MCUXCLEXAMPLE_STATUS_ERROR;                                                                        \
        }                                                                                                             \
        MCUX_CSSL_FP_FUNCTION_CALL_END();                                                                             \
        MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(resultWait, tokenWait,                                                       \
                                         mcuxClEls_WaitForOperation(MCUXCLELS_ERROR_FLAGS_CLEAR));                    \
        if (MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_WaitForOperation) != tokenWait)                                    \
        {                                                                                                             \
            PRINTF("[Error] ckdf-sp800108 wait token mismatch\r\n");                                                  \
            return MCUXCLEXAMPLE_STATUS_ERROR;                                                                        \
        }                                                                                                             \
        if (MCUXCLELS_STATUS_OK != resultWait)                                                                        \
        {                                                                                                             \
            PRINTF("[Error] ckdf-sp800108 wait failed\r\n");                                                          \
            return MCUXCLEXAMPLE_STATUS_ERROR;                                                                        \
        }                                                                                                             \
        MCUX_CSSL_FP_FUNCTION_CALL_END();                                                                             \
    } while (0);

#define HKDF_SP80056C()                                                                                            \
    do                                                                                                             \
    {                                                                                                              \
        const uint8_t label_buf[32U] = {                                                                           \
            0x00U, 0x01U, 0x02U, 0x03U, 0x04U, 0x05U, 0x06U, 0x07U, 0x08U, 0x09U, 0x0AU,                           \
            0x0BU, 0x0CU, 0x0DU, 0x0EU, 0x0FU, 0x10U, 0x11U, 0x12U, 0x13U, 0x14U, 0x15U,                           \
            0x16U, 0x17U, 0x18U, 0x19U, 0x1AU, 0x1BU, 0x1CU, 0x1DU, 0x1EU, 0x1FU,                                  \
        };                                                                                                         \
        uint8_t out[32U];                                                                                          \
        MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(resultCkdf, tokenAsync,                                                   \
                                         mcuxClEls_Hkdf_Sp80056c_Async(0x0EU, out, label_buf, sizeof(label_buf))); \
        if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_Hkdf_Sp80056c_Async) != tokenAsync) ||                         \
            (MCUXCLELS_STATUS_OK_WAIT != resultCkdf))                                                              \
        {                                                                                                          \
            PRINTF("[Error] hkdf-sp80056c failed\r\n");                                                            \
            return MCUXCLEXAMPLE_STATUS_ERROR;                                                                     \
        }                                                                                                          \
        MCUX_CSSL_FP_FUNCTION_CALL_END();                                                                          \
        MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(resultWait, tokenWait,                                                    \
                                         mcuxClEls_WaitForOperation(MCUXCLELS_ERROR_FLAGS_CLEAR));                 \
        if (MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_WaitForOperation) != tokenWait)                                 \
        {                                                                                                          \
            PRINTF("[Error] hkdf-sp80056c wait token mismatch\r\n");                                               \
            return MCUXCLEXAMPLE_STATUS_ERROR;                                                                     \
        }                                                                                                          \
        if (MCUXCLELS_STATUS_OK != resultWait)                                                                     \
        {                                                                                                          \
            PRINTF("[Error] hkdf-sp80056c wait failed\r\n");                                                       \
            return MCUXCLEXAMPLE_STATUS_ERROR;                                                                     \
        }                                                                                                          \
        MCUX_CSSL_FP_FUNCTION_CALL_END();                                                                          \
    } while (0);

#define HKDF_RFC5869(options, key_properties, delete_target_index)                                                  \
    do                                                                                                              \
    {                                                                                                               \
        if (delete_target_index)                                                                                    \
        {                                                                                                           \
            DELETE_KEY(0x04U);                                                                                      \
        }                                                                                                           \
        uint8_t der_data[MCUXCLELS_HKDF_RFC5869_DERIVATIONDATA_SIZE] = {0x00U, 0x65U, 0x32U, 0x67U, 0x63U, 0x6FU,   \
                                                                        0x6EU, 0x5FU, 0x73U, 0x65U, 0x00U, 0x00U};  \
        MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(                                                                           \
            resultCkdf, tokenAsync, mcuxClEls_Hkdf_Rfc5869_Async(options, 0x0EU, 0x04U, key_properties, der_data)); \
        if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_Hkdf_Rfc5869_Async) != tokenAsync) ||                           \
            (MCUXCLELS_STATUS_OK_WAIT != resultCkdf))                                                               \
        {                                                                                                           \
            PRINTF("[Error] hkdf-rfc5869 failed\r\n");                                                              \
            return MCUXCLEXAMPLE_STATUS_ERROR;                                                                      \
        }                                                                                                           \
        MCUX_CSSL_FP_FUNCTION_CALL_END();                                                                           \
        MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(resultWait, tokenWait,                                                     \
                                         mcuxClEls_WaitForOperation(MCUXCLELS_ERROR_FLAGS_CLEAR));                  \
        if (MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_WaitForOperation) != tokenWait)                                  \
        {                                                                                                           \
            PRINTF("[Error] hkdf-rfc5869 wait token mismatch\r\n");                                                 \
            return MCUXCLEXAMPLE_STATUS_ERROR;                                                                      \
        }                                                                                                           \
        if (MCUXCLELS_STATUS_OK != resultWait)                                                                      \
        {                                                                                                           \
            PRINTF("[Error] hkdf-rfc5869 wait failed\r\n");                                                         \
            return MCUXCLEXAMPLE_STATUS_ERROR;                                                                      \
        }                                                                                                           \
        MCUX_CSSL_FP_FUNCTION_CALL_END();                                                                           \
    } while (0);

#define DELETE_KEY(key_index)                                                                                       \
    do                                                                                                              \
    { /* Delete existing key for storing it for derivation */                                                       \
        MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClEls_KeyDelete_Async(key_index));                      \
        if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_KeyDelete_Async) != token) ||                                   \
            (MCUXCLELS_STATUS_OK_WAIT != result))                                                                   \
        {                                                                                                           \
            PRINTF("[Error] Key delete failed\r\n");                                                                \
        }                                                                                                           \
        MCUX_CSSL_FP_FUNCTION_CALL_END();                                                                           \
        MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClEls_WaitForOperation(MCUXCLELS_ERROR_FLAGS_CLEAR));   \
        if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_WaitForOperation) != token) || (MCUXCLELS_STATUS_OK != result)) \
        {                                                                                                           \
            PRINTF("[Error] Key delete wait failed\r\n");                                                           \
        }                                                                                                           \
        MCUX_CSSL_FP_FUNCTION_CALL_END();                                                                           \
    } while (0);

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
bool exec_hkdf_rfc5869(algorithm_result *a_result)
{
    DELETE_KEY(0xEU);
    mcuxClEls_KeyProp_t key_properties_ckdf;
    key_properties_ckdf.word.value = 0x84000021U;
    key_properties_ckdf.bits.uhkdf = 1U;
    CKDF_SP800108(key_properties_ckdf, false);
    DELETE_KEY(0x04U);

    mcuxClEls_HkdfOption_t opt;
    opt.word.value = MCUXCLELS_HKDF_ALGO_RFC5869;

    mcuxClEls_KeyProp_t key_properties;
    key_properties.word.value = 0x84000021U;

    double sum = 0.0F;
    for (uint16_t i = 0U; i < 256U; ++i)
    {
        sum += COMPUTE_CYCLES(HKDF_RFC5869(opt, key_properties, false), 1U, 1U);
        DELETE_KEY(0x04U);
    }
    a_result->cyclesPerBlock = sum / 256U; /* Cycles/Key */
    a_result->cyclesPerByte  = a_result->cyclesPerBlock / 32U;
    a_result->kbPerS         = KB_S(HKDF_RFC5869(opt, key_properties, true), 32U) - KB_S(DELETE_KEY(0x04U), 32U);

    return MCUXCLEXAMPLE_STATUS_OK;
}

bool exec_hkdf_sp80056c(algorithm_result *a_result)
{
    /* Set the uhkdf to true for the derivation key used in hkdf */
    DELETE_KEY(0xEU);
    mcuxClEls_KeyProp_t key_properties;
    key_properties.word.value = 0x84000021U;
    key_properties.bits.uhkdf = 1U;
    CKDF_SP800108(key_properties, false);

    a_result->cyclesPerBlock = COMPUTE_CYCLES(HKDF_SP80056C(), 1U, 1024U); /* Cycles/Key */
    a_result->cyclesPerByte  = a_result->cyclesPerBlock / 32U;
    a_result->kbPerS         = KB_S(HKDF_SP80056C(), 32U);

    return MCUXCLEXAMPLE_STATUS_OK;
}

bool exec_ckdf_Sp800108(algorithm_result *a_result, uint16_t key_size)
{
    /**************************************************************************/
    /* Preparation                                                            */
    /**************************************************************************/
    // Allocate and initialize session
    mcuxClSession_Descriptor_t session;
    MCUXCLEXAMPLE_ALLOCATE_AND_INITIALIZE_SESSION(&session, MCUXCLRANDOMMODES_MAX_CPU_WA_BUFFER_SIZE, 0U);
    MCUXCLEXAMPLE_INITIALIZE_PRNG(&session);

    /**************************************************************************/
    /* Key setup                                                              */
    /**************************************************************************/
    mcuxClEls_KeyProp_t key_properties;
    key_properties.word.value = MCUXCLELS_KEYPROPERTY_VALUE_CKDF | MCUXCLELS_KEYPROPERTY_VALUE_GENERAL_PURPOSE_SLOT |
                                MCUXCLELS_KEYPROPERTY_VALUE_ACTIVE |
                                (key_size == KEY_SIZE_256 ? MCUXCLELS_KEYPROPERTY_VALUE_KEY_SIZE_256 :
                                                            MCUXCLELS_KEYPROPERTY_VALUE_KEY_SIZE_128) |
                                MCUXCLELS_KEYPROPERTY_VALUE_NOTSECURE;

    DELETE_KEY(0x0EU);
    double sum = 0.0F;
    for (uint16_t i = 0U; i < 256U; ++i)
    {
        sum += COMPUTE_CYCLES(CKDF_SP800108(key_properties, false), 1U, 1U);
        DELETE_KEY(0x0EU);
    }
    a_result->cyclesPerBlock = sum / 256U; /* Cycles/Key */
    a_result->cyclesPerByte  = a_result->cyclesPerBlock / (key_size / 8U);
    a_result->kbPerS =
        KB_S(CKDF_SP800108(key_properties, true), (key_size / 8U)) - KB_S(DELETE_KEY(0x0EU), (key_size / 8U));
    /*************************************************************************/
    /* Cleanup                                                                */
    /**************************************************************************/
    /** Destroy Session and cleanup Session **/
    if (!mcuxClExample_Session_Clean(&session))
    {
        PRINTF("[Error] Session cleaning failed\r\n");
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    return MCUXCLEXAMPLE_STATUS_OK;
}

void test_ckdf_Sp800108(char *code_from, uint16_t key_size)
{
    algorithm_result a_result;
    strcpy(a_result.execution, key_size == KEY_SIZE_256 ? "256 BIT KEY" : "128 BIT KEY");
    strcpy(a_result.code, code_from);
    strcpy(a_result.data, "RAM");
    exec_ckdf_Sp800108(&a_result, key_size);

    PRINT_KDF_RESULT(a_result);
}

void test_hkdf_sp80056c(char *code_from)
{
    algorithm_result a_result;
    strcpy(a_result.execution, "256 BIT KEY");
    strcpy(a_result.code, code_from);
    strcpy(a_result.data, "RAM");
    exec_hkdf_sp80056c(&a_result);

    PRINT_KDF_RESULT(a_result);
}

void test_hkdf_rfc5869(char *code_from)
{
    algorithm_result a_result;
    strcpy(a_result.execution, "256 BIT KEY");
    strcpy(a_result.data, "RAM");
    strcpy(a_result.code, code_from);
    exec_hkdf_rfc5869(&a_result);

    PRINT_KDF_RESULT(a_result);
}

void run_tests_kdf()
{
    char code_from[6U];
    strcpy(code_from, BOARD_IS_XIP() ? "FLASH" : "RAM");

    PRINTF("CKDF-SP800108\r\n");
    test_ckdf_Sp800108(code_from, KEY_SIZE_256);
    test_ckdf_Sp800108(code_from, KEY_SIZE_128);
    PRINTF("\r\n");

    PRINTF("HKDF-SP80056C\r\n");
    test_hkdf_sp80056c(code_from);
    PRINTF("\r\n");

    PRINTF("HKDF-RFC5869\r\n");
    test_hkdf_rfc5869(code_from);
    PRINTF("\r\n");
}
