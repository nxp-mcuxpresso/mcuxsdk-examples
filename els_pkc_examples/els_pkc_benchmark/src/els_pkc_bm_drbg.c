/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "els_pkc_bm_drbg.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define CTR_DRBG(session, block_amount, data_from_ram)                                                                 \
    do                                                                                                                 \
    {                                                                                                                  \
        if (data_from_ram)                                                                                             \
        {                                                                                                              \
            MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(                                                                          \
                rg_status, generate_token,                                                                             \
                mcuxClRandom_generate(                                                                                 \
                    session, block_amount == SINGLE_BLOCK ? s_DrbgBufferSingleBlock : s_DrbgBuffer,                    \
                    block_amount == SINGLE_BLOCK ? sizeof(s_DrbgBufferSingleBlock) : sizeof(s_DrbgBuffer)));           \
            if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandom_generate) != generate_token) ||                             \
                (MCUXCLRANDOM_STATUS_OK != rg_status))                                                                 \
            {                                                                                                          \
                PRINTF("[Error] CTR random generation failed\r\n");                                                    \
                return MCUXCLEXAMPLE_STATUS_ERROR;                                                                     \
            }                                                                                                          \
            MCUX_CSSL_FP_FUNCTION_CALL_END();                                                                          \
        }                                                                                                              \
        else                                                                                                           \
        {                                                                                                              \
            MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(                                                                          \
                rg_status, generate_token,                                                                             \
                mcuxClRandom_generate(                                                                                 \
                    session, block_amount == SINGLE_BLOCK ? s_DrbgBufferSingleBlockFlash : s_DrbgBufferFlash,          \
                    block_amount == SINGLE_BLOCK ? sizeof(s_DrbgBufferSingleBlockFlash) : sizeof(s_DrbgBufferFlash))); \
            if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandom_generate) != generate_token) ||                             \
                (MCUXCLRANDOM_STATUS_OK != rg_status))                                                                 \
            {                                                                                                          \
                PRINTF("[Error] CTR random generation failed\r\n");                                                    \
                return MCUXCLEXAMPLE_STATUS_ERROR;                                                                     \
            }                                                                                                          \
            MCUX_CSSL_FP_FUNCTION_CALL_END();                                                                          \
        }                                                                                                              \
    } while (0U);

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
static uint8_t s_DrbgBufferSingleBlock[16U];
static uint8_t s_DrbgBuffer[1024U * 16U];

static uint8_t s_DrbgBufferSingleBlockFlash[16U];
static uint8_t s_DrbgBufferFlash[1024U * 16U];

/*******************************************************************************
 * Code
 ******************************************************************************/
bool exec_ctr_drbg(char *data_from,
                   uint32_t security_strength,
                   uint32_t block_amount,
                   algorithm_result *a_result,
                   const bool cache_enable)
{
    bool data_from_ram = !strcmp(data_from, "RAM");

    mcuxClSession_Descriptor_t sessionDesc;
    mcuxClSession_Handle_t session = &sessionDesc;
    MCUXCLEXAMPLE_ALLOCATE_AND_INITIALIZE_SESSION(session, MCUXCLRANDOMMODES_MAX_CPU_WA_BUFFER_SIZE, 0U);

    /**************************************************************************/
    /* DRBG selftest.                                                         */
    /**************************************************************************/
    mcuxClRandom_Mode_t randomMode =
        security_strength == 128U ? mcuxClRandomModes_Mode_ELS_Drbg : mcuxClRandomModes_Mode_CtrDrbg_AES256_DRG3;

    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(rs_status, selftest_token, mcuxClRandom_selftest(session, randomMode));

    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandom_selftest) != selftest_token) ||
        (MCUXCLRANDOM_STATUS_OK != rs_status))
    {
        PRINTF("[Error] Random self-test failed\r\n");
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    uint32_t context[MCUXCLRANDOMMODES_CTR_DRBG_AES256_CONTEXT_SIZE_IN_WORDS] = {0U};

    /**************************************************************************/
    /* DRBG initialization                                                    */
    /**************************************************************************/
    /* Initialize an AES CTR_DRBG */
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(ri_status, init_token,
                                     mcuxClRandom_init(session, (mcuxClRandom_Context_t)context, randomMode));

    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandom_init) != init_token) || (MCUXCLRANDOM_STATUS_OK != ri_status))
    {
        PRINTF("[Error] Random init failed\r\n");
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    /**************************************************************************/
    /* Generate several random byte strings and reseed the DRBG in between.   */
    /**************************************************************************/
    const uint32_t iteration_amount = cache_enable ? 1024U : 1U;
    a_result->cyclesPerBlock =
        COMPUTE_CYCLES(CTR_DRBG(session, block_amount, data_from_ram), block_amount * 16U, iteration_amount);
    a_result->cyclesPerByte = a_result->cyclesPerBlock / 16U;
    a_result->kbPerS        = KB_S(CTR_DRBG(session, block_amount, data_from_ram), block_amount * 16U);
    /**************************************************************************/
    /* Cleanup                                                                */
    /**************************************************************************/

    /* Random uninit. */
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(ru_status, uninit_token, mcuxClRandom_uninit(session));

    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandom_uninit) != uninit_token) || (MCUXCLRANDOM_STATUS_OK != ru_status))
    {
        PRINTF("[Error] Random uninit failed\r\n");
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

void test_drbg(
    char *code_from, char *data_from, uint32_t security_strength, uint32_t block_amount, const bool cache_enable)
{
    if (!strcmp(code_from, "RAM") && !strcmp(data_from, "FLASH"))
        return;
    algorithm_result a_result;
    strcpy(a_result.execution, block_amount == SINGLE_BLOCK ? "SINGLE BLOCK" : "MULTIPLE BLOCKS");
    strcpy(a_result.code, code_from);
    strcpy(a_result.data, data_from);
    exec_ctr_drbg(data_from, security_strength, block_amount, &a_result, cache_enable);
    strcpy(a_result.cached, cache_enable ? "YES" : "NO");

    PRINT_RESULT(a_result);
}

void run_tests_drbg(void)
{
    char code_from[6U];
    strcpy(code_from, BOARD_IS_XIP() ? "FLASH" : "RAM");

    PRINTF("AES-CTR-128-DRBG:\r\n");
    test_drbg(code_from, "FLASH", 128U, MULTIPLE_BLOCKS, false);
    test_drbg(code_from, "FLASH", 128U, MULTIPLE_BLOCKS, true);
    test_drbg(code_from, "RAM", 128U, MULTIPLE_BLOCKS, false);
    test_drbg(code_from, "RAM", 128U, MULTIPLE_BLOCKS, true);
    test_drbg(code_from, "FLASH", 128U, SINGLE_BLOCK, true);
    test_drbg(code_from, "RAM", 128U, SINGLE_BLOCK, true);
    PRINTF("\r\n");

    PRINTF("AES-CTR-256-DRBG:\r\n");
    test_drbg(code_from, "FLASH", 256U, MULTIPLE_BLOCKS, false);
    test_drbg(code_from, "FLASH", 256U, MULTIPLE_BLOCKS, true);
    test_drbg(code_from, "RAM", 256U, MULTIPLE_BLOCKS, false);
    test_drbg(code_from, "RAM", 256U, MULTIPLE_BLOCKS, true);
    test_drbg(code_from, "FLASH", 256U, SINGLE_BLOCK, true);
    test_drbg(code_from, "RAM", 256U, SINGLE_BLOCK, true);
    PRINTF("\r\n");
}
