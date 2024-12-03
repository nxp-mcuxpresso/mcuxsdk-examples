/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "els_pkc_bm_kwa.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* Key, which is getting wrapped */
static uint32_t s_KeyToWrap[8U];

/* Key encryption key */
static uint8_t s_Kek[32U];

/* Output after key RFC-3394 key wrap function */
static uint32_t s_OutputBuffer[12U];

/* Key, which is getting wrapped stored in flash */
static const uint8_t s_KeyToWrapFlash[32U];

/* Key encryption key stored in flash */
static const uint8_t s_KekFlash[32U];

/*******************************************************************************
 * Code
 ******************************************************************************/
bool exec_rfc3394_wrap(char *data_from)
{
    bool data_from_ram = !strcmp("RAM", data_from);

    mcuxClEls_KeyProp_t key_properties;
    key_properties.word.value = 0U;
    key_properties.bits.ksize = MCUXCLELS_KEYPROPERTY_KEY_SIZE_256;
    key_properties.bits.kactv = MCUXCLELS_KEYPROPERTY_ACTIVE_TRUE;

    bool wrap_result = mcuxClExample_rfc3394_wrap(
        /*const uint32_t * pInput         */ s_KeyToWrap,        /*  pointer to key to be wrapped */
        /*size_t inputLength,            */ sizeof(s_KeyToWrap), /*  length of key to be wrapped in bytes */
        /*const uint8_t * pKek_in        */ data_from_ram ? s_Kek : s_KekFlash, /*  pointer to key wrapping key */
        /*mcuxClEls_KeyIndex_t keyIdx    */ 0U,                                 /*  keyslot index of key wrapping key */
        /*uint8_t extkey                 */ 1U, /*  0-use key stored internally at keyIdx as wrapping key, 1-use
                                                   external pKek_in as wrapping key */
        /*size_t kekLength               */ data_from_ram ? sizeof(s_Kek) : sizeof(s_KekFlash), /*  length of key
                                                                                                   wrapping key in bytes
                                                                                                 */
        /*uint32_t * pOutput              */ s_OutputBuffer, /*  pointer to output buffer, size has to be inputLength +
                                                             16  bytes */
        /*mcuxClEls_KeyProp_t properties */ key_properties); /*  properties of the key to be wrapped */
    if (!wrap_result)
    {
        PRINTF("rfc3394 wrap failed\r\n");
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }
    return MCUXCLEXAMPLE_STATUS_OK;
}

bool exec_rfc3394_unwrap(char *data_from)
{
    bool data_from_ram = !strcmp("RAM", data_from);
    uint32_t outputBuffer[10U];

    bool unwrap_result = mcuxClExample_rfc3394_unwrap(
        s_OutputBuffer,                     //< pointer to rfc3394 blob to be wrapped
        sizeof(s_OutputBuffer),             //< length of key the rfc3394 blob in bytes
        data_from_ram ? s_Kek : s_KekFlash, //< pointer to key wrapping key
        0U,                                 //< keyslot index of key wrapping key
        1, //< 0-use key stored internally at keyIdx as wrapping key, 1-use external pKek_in as wrapping key
        data_from_ram ? sizeof(s_Kek) : sizeof(s_KekFlash), //< length of key wrapping key in bytes
        outputBuffer //< pointer to output buffer, size has to inputLength - 8 bytes, contents will be
                     // properties|zeros|key
    );
    if (!unwrap_result)
    {
        PRINTF("rfc3394 unwrap failed\r\n");
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }
    return MCUXCLEXAMPLE_STATUS_OK;
}

void test_rfc3394_wrap(char *code_from, char *data_from, bool cache_enable)
{
    if (!strcmp(code_from, "RAM") && !strcmp(data_from, "FLASH"))
        return;
    algorithm_result a_result;
    strcpy(a_result.execution, "WRAP");
    strcpy(a_result.code, code_from);
    strcpy(a_result.data, data_from);
    uint32_t iteration_amount = cache_enable ? 1024U : 1U;
    a_result.cyclesPerBlock   = COMPUTE_CYCLES(exec_rfc3394_wrap(data_from), 1, iteration_amount);
    a_result.cyclesPerByte    = a_result.cyclesPerBlock / 32U;
    a_result.kbPerS           = KB_S(exec_rfc3394_wrap(data_from), 32U);
    strcpy(a_result.cached, cache_enable ? "YES" : "NO");

    PRINT_RESULT(a_result);
}

void test_rfc3394_unwrap(char *code_from, char *data_from, bool cache_enable)
{
    if (!strcmp(code_from, "RAM") && !strcmp(data_from, "FLASH"))
        return;
    algorithm_result a_result;
    strcpy(a_result.execution, "UNWRAP");
    strcpy(a_result.code, code_from);
    strcpy(a_result.data, data_from);
    uint32_t iteration_amount = cache_enable ? 1024U : 1U;
    a_result.cyclesPerBlock   = COMPUTE_CYCLES(exec_rfc3394_unwrap(data_from), 1, iteration_amount);
    a_result.cyclesPerByte    = a_result.cyclesPerBlock / 32U;
    a_result.kbPerS           = KB_S(exec_rfc3394_unwrap(data_from), 32U);
    strcpy(a_result.cached, cache_enable ? "YES" : "NO");

    PRINT_RESULT(a_result);
}

void run_tests_kwa()
{
    char code_from[6U];
    strcpy(code_from, BOARD_IS_XIP() ? "FLASH" : "RAM");

    PRINTF("AES-256-RFC3394\r\n");
    test_rfc3394_wrap(code_from, "FLASH", false);
    test_rfc3394_wrap(code_from, "FLASH", true);
    test_rfc3394_wrap(code_from, "RAM", false);
    test_rfc3394_wrap(code_from, "RAM", true);

    test_rfc3394_unwrap(code_from, "FLASH", false);
    test_rfc3394_unwrap(code_from, "FLASH", true);
    test_rfc3394_unwrap(code_from, "RAM", false);
    test_rfc3394_unwrap(code_from, "RAM", true);
    PRINTF("\r\n");
}
