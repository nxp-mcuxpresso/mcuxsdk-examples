/*
 *  Test-suite demonstration program
 *
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of mbed TLS (https://tls.mbed.org)
 */
/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "mbedtls/des.h"

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "erpc_server_setup.h"
#include "erpc_transport_setup.h"
#include "erpc_error_handler.h"
#include "mbedtls_test_suite_server.h"
#include "mbedtls_test_suite.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

int32_t board_des_check_weak(const data_t *key, int32_t ret)
{
    TEST_ASSERT(mbedtls_des_key_check_weak(key->x) == ret);

    return EXIT_SUCCESS;
}

int32_t board_des_encrypt_ecb(const data_t *key_str, const data_t *src_str, const data_t *hex_dst_string)
{
    unsigned char output[100];
    mbedtls_des_context ctx;

    memset(output, 0x00, 100);
    mbedtls_des_init(&ctx);

    mbedtls_des_setkey_enc(&ctx, key_str->x);
    TEST_ASSERT(mbedtls_des_crypt_ecb(&ctx, src_str->x, output) == 0);

    TEST_ASSERT(hexcmp(output, hex_dst_string->x, 8, hex_dst_string->len) == 0);

    mbedtls_des_free(&ctx);

    return EXIT_SUCCESS;
}

int32_t board_des_decrypt_ecb(const data_t *key_str, const data_t *src_str, const data_t *hex_dst_string)
{
    unsigned char output[100];
    mbedtls_des_context ctx;

    memset(output, 0x00, 100);
    mbedtls_des_init(&ctx);

    mbedtls_des_setkey_dec(&ctx, key_str->x);
    TEST_ASSERT(mbedtls_des_crypt_ecb(&ctx, src_str->x, output) == 0);

    TEST_ASSERT(hexcmp(output, hex_dst_string->x, 8, hex_dst_string->len) == 0);

    mbedtls_des_free(&ctx);

    return EXIT_SUCCESS;
}

int32_t board_des_encrypt_cbc(const data_t *key_str,
                              const data_t *iv_str,
                              const data_t *src_str,
                              const data_t *hex_dst_string,
                              int32_t cbc_result)
{
    unsigned char output[100];
    mbedtls_des_context ctx;

    memset(output, 0x00, 100);
    mbedtls_des_init(&ctx);

    mbedtls_des_setkey_enc(&ctx, key_str->x);
    TEST_ASSERT(mbedtls_des_crypt_cbc(&ctx, MBEDTLS_DES_ENCRYPT, src_str->len, iv_str->x, src_str->x, output) ==
                cbc_result);
    if (cbc_result == 0)
    {
        TEST_ASSERT(hexcmp(output, hex_dst_string->x, src_str->len, hex_dst_string->len) == 0);
    }

    mbedtls_des_free(&ctx);

    return EXIT_SUCCESS;
}

int32_t board_des_decrypt_cbc(const data_t *key_str,
                              const data_t *iv_str,
                              const data_t *src_str,
                              const data_t *hex_dst_string,
                              int32_t cbc_result)
{
    unsigned char output[100];
    mbedtls_des_context ctx;

    memset(output, 0x00, 100);
    mbedtls_des_init(&ctx);

    mbedtls_des_setkey_dec(&ctx, key_str->x);
    TEST_ASSERT(mbedtls_des_crypt_cbc(&ctx, MBEDTLS_DES_DECRYPT, src_str->len, iv_str->x, src_str->x, output) ==
                cbc_result);
    if (cbc_result == 0)
    {
        TEST_ASSERT(hexcmp(output, hex_dst_string->x, src_str->len, hex_dst_string->len) == 0);
    }

    mbedtls_des_free(&ctx);

    return EXIT_SUCCESS;
}

int32_t board_des3_encrypt_ecb(int32_t key_count,
                               const data_t *key_str,
                               const data_t *src_str,
                               const data_t *hex_dst_string)
{
    unsigned char output[100];
    mbedtls_des3_context ctx;

    memset(output, 0x00, 100);
    mbedtls_des3_init(&ctx);

    if (key_count == 2)
        mbedtls_des3_set2key_enc(&ctx, key_str->x);
    else if (key_count == 3)
        mbedtls_des3_set3key_enc(&ctx, key_str->x);
    else
        TEST_ASSERT(0);

    TEST_ASSERT(mbedtls_des3_crypt_ecb(&ctx, src_str->x, output) == 0);

    TEST_ASSERT(hexcmp(output, hex_dst_string->x, 8, hex_dst_string->len) == 0);

    mbedtls_des3_free(&ctx);

    return EXIT_SUCCESS;
}

int32_t board_des3_decrypt_ecb(int32_t key_count,
                               const data_t *key_str,
                               const data_t *src_str,
                               const data_t *hex_dst_string)
{
    unsigned char output[100];
    mbedtls_des3_context ctx;

    memset(output, 0x00, 100);
    mbedtls_des3_init(&ctx);

    if (key_count == 2)
        mbedtls_des3_set2key_dec(&ctx, key_str->x);
    else if (key_count == 3)
        mbedtls_des3_set3key_dec(&ctx, key_str->x);
    else
        TEST_ASSERT(0);

    TEST_ASSERT(mbedtls_des3_crypt_ecb(&ctx, src_str->x, output) == 0);

    TEST_ASSERT(hexcmp(output, hex_dst_string->x, 8, hex_dst_string->len) == 0);

    mbedtls_des3_free(&ctx);

    return EXIT_SUCCESS;
}

int32_t board_des3_encrypt_cbc(int32_t key_count,
                               const data_t *key_str,
                               const data_t *iv_str,
                               const data_t *src_str,
                               const data_t *hex_dst_string,
                               int32_t cbc_result)
{
    unsigned char output[100];
    mbedtls_des3_context ctx;

    memset(output, 0x00, 100);
    mbedtls_des3_init(&ctx);

    if (key_count == 2)
        mbedtls_des3_set2key_enc(&ctx, key_str->x);
    else if (key_count == 3)
        mbedtls_des3_set3key_enc(&ctx, key_str->x);
    else
        TEST_ASSERT(0);

    TEST_ASSERT(mbedtls_des3_crypt_cbc(&ctx, MBEDTLS_DES_ENCRYPT, src_str->len, iv_str->x, src_str->x, output) ==
                cbc_result);

    if (cbc_result == 0)
    {
        TEST_ASSERT(hexcmp(output, hex_dst_string->x, src_str->len, hex_dst_string->len) == 0);
    }

    mbedtls_des3_free(&ctx);

    return EXIT_SUCCESS;
}

int32_t board_des3_decrypt_cbc(int32_t key_count,
                               const data_t *key_str,
                               const data_t *iv_str,
                               const data_t *src_str,
                               const data_t *hex_dst_string,
                               int32_t cbc_result)
{
    unsigned char output[100];
    mbedtls_des3_context ctx;

    memset(output, 0x00, 100);
    mbedtls_des3_init(&ctx);

    if (key_count == 2)
        mbedtls_des3_set2key_dec(&ctx, key_str->x);
    else if (key_count == 3)
        mbedtls_des3_set3key_dec(&ctx, key_str->x);
    else
        TEST_ASSERT(0);

    TEST_ASSERT(mbedtls_des3_crypt_cbc(&ctx, MBEDTLS_DES_DECRYPT, src_str->len, iv_str->x, src_str->x, output) ==
                cbc_result);

    if (cbc_result == 0)
    {
        TEST_ASSERT(hexcmp(output, hex_dst_string->x, src_str->len, hex_dst_string->len) == 0);
    }

    mbedtls_des3_free(&ctx);

    return EXIT_SUCCESS;
}

int32_t board_des_key_parity_run()
{
    int i, j, cnt;
    unsigned char key[MBEDTLS_DES_KEY_SIZE];
    unsigned int parity;

    memset(key, 0, MBEDTLS_DES_KEY_SIZE);
    cnt = 0;

    // Iterate through all possible byte values
    //
    for (i = 0; i < 32; i++)
    {
        for (j = 0; j < 8; j++)
            key[j] = cnt++;

        // Set the key parity according to the table
        //
        mbedtls_des_key_set_parity(key);

        // Check the parity with a function
        //
        for (j = 0; j < 8; j++)
        {
            parity = key[j] ^ (key[j] >> 4);
            parity = parity ^ (parity >> 1) ^ (parity >> 2) ^ (parity >> 3);
            parity &= 1;

            if (parity != 1)
                TEST_ASSERT(0);
        }

        // Check the parity with the table
        //
        TEST_ASSERT(mbedtls_des_key_check_key_parity(key) == 0);
    }

    return EXIT_SUCCESS;
}

int32_t board_mbedtls_des_self_test(int32_t verbose)
{
    TEST_ASSERT(mbedtls_des_self_test(verbose) == 0);

    return EXIT_SUCCESS;
}
