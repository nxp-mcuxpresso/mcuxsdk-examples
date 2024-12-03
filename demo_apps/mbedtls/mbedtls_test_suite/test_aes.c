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

#include "mbedtls/aes.h"

#include "app.h"
#include "board.h"
#include "erpc_error_handler.h"
#include "erpc_server_setup.h"
#include "erpc_transport_setup.h"
#include "fsl_debug_console.h"
#include "mbedtls_test_suite.h"
#include "mbedtls_test_suite_server.h"

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

int32_t board_aes_encrypt_ecb(const data_t *key_str,
                              const data_t *src_str,
                              const data_t *hex_dst_string,
                              int32_t setkey_result)
{
    unsigned char output[100];
    mbedtls_aes_context ctx;

    memset(output, 0x00, 100);

    mbedtls_aes_init(&ctx);

    TEST_ASSERT(mbedtls_aes_setkey_enc(&ctx, key_str->x, key_str->len * 8) == setkey_result);
    if (setkey_result == 0)
    {
        TEST_ASSERT(mbedtls_aes_crypt_ecb(&ctx, MBEDTLS_AES_ENCRYPT, src_str->x, output) == 0);

        TEST_ASSERT(hexcmp(output, hex_dst_string->x, 16, hex_dst_string->len) == 0);
    }
    return EXIT_SUCCESS;
}

int32_t board_aes_decrypt_ecb(const data_t *key_str,
                              const data_t *src_str,
                              const data_t *hex_dst_string,
                              int32_t setkey_result)
{
    unsigned char output[100];
    mbedtls_aes_context ctx;

    memset(output, 0x00, 100);
    mbedtls_aes_init(&ctx);

    TEST_ASSERT(mbedtls_aes_setkey_dec(&ctx, key_str->x, key_str->len * 8) == setkey_result);
    if (setkey_result == 0)
    {
        TEST_ASSERT(mbedtls_aes_crypt_ecb(&ctx, MBEDTLS_AES_DECRYPT, src_str->x, output) == 0);

        TEST_ASSERT(hexcmp(output, hex_dst_string->x, 16, hex_dst_string->len) == 0);
    }

    return EXIT_SUCCESS;
}

int32_t board_aes_encrypt_cbc(const data_t *key_str,
                              const data_t *iv_str,
                              const data_t *src_str,
                              const data_t *hex_dst_string,
                              int32_t cbc_result)
{
    unsigned char output[100];
    mbedtls_aes_context ctx;

    memset(output, 0x00, 100);
    mbedtls_aes_init(&ctx);

    mbedtls_aes_setkey_enc(&ctx, key_str->x, key_str->len * 8);
    TEST_ASSERT(mbedtls_aes_crypt_cbc(&ctx, MBEDTLS_AES_ENCRYPT, src_str->len, iv_str->x, src_str->x, output) ==
                cbc_result);
    if (cbc_result == 0)
    {
        TEST_ASSERT(hexcmp(output, hex_dst_string->x, src_str->len, hex_dst_string->len) == 0);
    }

    return EXIT_SUCCESS;
}

int32_t board_aes_decrypt_cbc(const data_t *key_str,
                              const data_t *iv_str,
                              const data_t *src_str,
                              const data_t *hex_dst_string,
                              int32_t cbc_result)
{
    unsigned char output[100];
    mbedtls_aes_context ctx;

    memset(output, 0x00, 100);
    mbedtls_aes_init(&ctx);

    mbedtls_aes_setkey_dec(&ctx, key_str->x, key_str->len * 8);
    TEST_ASSERT(mbedtls_aes_crypt_cbc(&ctx, MBEDTLS_AES_DECRYPT, src_str->len, iv_str->x, src_str->x, output) ==
                cbc_result);
    if (cbc_result == 0)
    {
        TEST_ASSERT(hexcmp(output, hex_dst_string->x, src_str->len, hex_dst_string->len) == 0);
    }

    return EXIT_SUCCESS;
}

int32_t board_aes_encrypt_cfb128(const data_t *key_str,
                                 const data_t *iv_str,
                                 const data_t *src_str,
                                 const data_t *hex_dst_string)
{
    unsigned char output[100];
    mbedtls_aes_context ctx;
    size_t iv_offset = 0;

    memset(output, 0x00, 100);
    mbedtls_aes_init(&ctx);

    mbedtls_aes_setkey_enc(&ctx, key_str->x, key_str->len * 8);
    TEST_ASSERT(mbedtls_aes_crypt_cfb128(&ctx, MBEDTLS_AES_ENCRYPT, 16, &iv_offset, iv_str->x, src_str->x, output) ==
                0);

    TEST_ASSERT(hexcmp(output, hex_dst_string->x, 16, hex_dst_string->len) == 0);

    return EXIT_SUCCESS;
}

int32_t board_aes_decrypt_cfb128(const data_t *key_str,
                                 const data_t *iv_str,
                                 const data_t *src_str,
                                 const data_t *hex_dst_string)
{
    unsigned char output[100];
    mbedtls_aes_context ctx;
    size_t iv_offset = 0;

    memset(output, 0x00, 100);
    mbedtls_aes_init(&ctx);

    mbedtls_aes_setkey_enc(&ctx, key_str->x, key_str->len * 8);
    TEST_ASSERT(mbedtls_aes_crypt_cfb128(&ctx, MBEDTLS_AES_DECRYPT, 16, &iv_offset, iv_str->x, src_str->x, output) ==
                0);

    TEST_ASSERT(hexcmp(output, hex_dst_string->x, 16, hex_dst_string->len) == 0);

    return EXIT_SUCCESS;
}

int32_t board_aes_encrypt_cfb8(const data_t *key_str,
                               const data_t *iv_str,
                               const data_t *src_str,
                               const data_t *hex_dst_string)
{
    unsigned char output[100];
    mbedtls_aes_context ctx;

    memset(output, 0x00, 100);
    mbedtls_aes_init(&ctx);

    mbedtls_aes_setkey_enc(&ctx, key_str->x, key_str->len * 8);
    TEST_ASSERT(mbedtls_aes_crypt_cfb8(&ctx, MBEDTLS_AES_ENCRYPT, src_str->len, iv_str->x, src_str->x, output) == 0);

    TEST_ASSERT(hexcmp(output, hex_dst_string->x, src_str->len, hex_dst_string->len) == 0);

    return EXIT_SUCCESS;
}

int32_t board_aes_decrypt_cfb8(const data_t *key_str,
                               const data_t *iv_str,
                               const data_t *src_str,
                               const data_t *hex_dst_string)
{
    unsigned char output[100];
    mbedtls_aes_context ctx;

    memset(output, 0x00, 100);
    mbedtls_aes_init(&ctx);

    mbedtls_aes_setkey_enc(&ctx, key_str->x, key_str->len * 8);
    TEST_ASSERT(mbedtls_aes_crypt_cfb8(&ctx, MBEDTLS_AES_DECRYPT, src_str->len, iv_str->x, src_str->x, output) == 0);

    TEST_ASSERT(hexcmp(output, hex_dst_string->x, src_str->len, hex_dst_string->len) == 0);

    return EXIT_SUCCESS;
}

int32_t board_aes_encrypt_ofb(int32_t fragment_size,
                              const char *hex_key_string,
                              const char *hex_iv_string,
                              const char *hex_src_string,
                              const char *hex_dst_string)
{
    unsigned char key_str[32];
    unsigned char iv_str[16];
    unsigned char src_str[64];
    unsigned char dst_str[64];
    unsigned char output[32];
    mbedtls_aes_context ctx;
    size_t iv_offset = 0;
    int in_buffer_len;
    unsigned char *src_str_next;
    int key_len;

    memset(key_str, 0x00, sizeof(key_str));
    memset(iv_str, 0x00, sizeof(iv_str));
    memset(src_str, 0x00, sizeof(src_str));
    memset(dst_str, 0x00, sizeof(dst_str));
    memset(output, 0x00, sizeof(output));
    mbedtls_aes_init(&ctx);

    TEST_ASSERT(strlen(hex_key_string) <= (32 * 2));
    TEST_ASSERT(strlen(hex_iv_string) <= (16 * 2));
    TEST_ASSERT(strlen(hex_src_string) <= (64 * 2));
    TEST_ASSERT(strlen(hex_dst_string) <= (64 * 2));

    key_len = unhexify(key_str, hex_key_string);
    unhexify(iv_str, hex_iv_string);
    in_buffer_len = unhexify(src_str, hex_src_string);

    TEST_ASSERT(mbedtls_aes_setkey_enc(&ctx, key_str, key_len * 8) == 0);
    src_str_next = src_str;

    while (in_buffer_len > 0)
    {
        TEST_ASSERT(mbedtls_aes_crypt_ofb(&ctx, fragment_size, &iv_offset, iv_str, src_str_next, output) == 0);

        hexify(dst_str, output, fragment_size);
        TEST_ASSERT(strncmp((char *)dst_str, hex_dst_string, (2 * fragment_size)) == 0);

        in_buffer_len -= fragment_size;
        hex_dst_string += (fragment_size * 2);
        src_str_next += fragment_size;

        if (in_buffer_len < fragment_size)
            fragment_size = in_buffer_len;
    }

    return EXIT_SUCCESS;
}

int32_t board_mbedtls_aes_self_test(int32_t verbose)
{
    TEST_ASSERT(mbedtls_aes_self_test(verbose) == 0);

    return EXIT_SUCCESS;
}
