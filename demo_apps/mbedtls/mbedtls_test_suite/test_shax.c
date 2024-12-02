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

#include "mbedtls/sha1.h"
#include "mbedtls/sha256.h"
#include "mbedtls/sha512.h"

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

/* BEGIN_CASE depends_on:MBEDTLS_SHA1_C */
int32_t board_mbedtls_sha1(const data_t *src_str, const data_t *hex_hash_string)
{
    unsigned char output[41];

    memset(output, 0x00, 41);

    TEST_ASSERT(mbedtls_sha1_ret(src_str->x, src_str->len, output) == 0);

    TEST_ASSERT(hexcmp(output, hex_hash_string->x, 20, hex_hash_string->len) == 0);

    return EXIT_SUCCESS;
}
/* END_CASE */

/* BEGIN_CASE depends_on:MBEDTLS_SHA256_C */
int32_t board_sha224(const data_t *src_str, const data_t *hex_hash_string)
{
    unsigned char output[57];

    memset(output, 0x00, 57);

    TEST_ASSERT(mbedtls_sha256_ret(src_str->x, src_str->len, output, 1) == 0);

    TEST_ASSERT(hexcmp(output, hex_hash_string->x, 28, hex_hash_string->len) == 0);
    return EXIT_SUCCESS;
}
/* END_CASE */

/* BEGIN_CASE depends_on:MBEDTLS_SHA256_C */
int32_t board_mbedtls_sha256(const data_t *src_str, const data_t *hex_hash_string)
{
    unsigned char output[65];

    memset(output, 0x00, 65);

    TEST_ASSERT(mbedtls_sha256_ret(src_str->x, src_str->len, output, 0) == 0);

    TEST_ASSERT(hexcmp(output, hex_hash_string->x, 32, hex_hash_string->len) == 0);

    return EXIT_SUCCESS;
}
/* END_CASE */

/* BEGIN_CASE depends_on:MBEDTLS_SHA512_C */
int32_t board_sha384(const data_t *src_str, const data_t *hex_hash_string)
{
    unsigned char output[97];

    memset(output, 0x00, 97);

    TEST_ASSERT(mbedtls_sha512_ret(src_str->x, src_str->len, output, 1) == 0);

    TEST_ASSERT(hexcmp(output, hex_hash_string->x, 48, hex_hash_string->len) == 0);

    return EXIT_SUCCESS;
}
/* END_CASE */

/* BEGIN_CASE depends_on:MBEDTLS_SHA512_C */
int32_t board_mbedtls_sha512(const data_t *src_str, const data_t *hex_hash_string)
{
    unsigned char output[129];

    memset(output, 0x00, 129);

    TEST_ASSERT(mbedtls_sha512_ret(src_str->x, src_str->len, output, 0) == 0);

    TEST_ASSERT(hexcmp(output, hex_hash_string->x, 64, hex_hash_string->len) == 0);

    return EXIT_SUCCESS;
}
/* END_CASE */

int32_t board_sha1_selftest(int32_t verbose)
{
    TEST_ASSERT(mbedtls_sha1_self_test(verbose) == 0);

    return EXIT_SUCCESS;
}

int32_t board_sha256_selftest(int32_t verbose)
{
    TEST_ASSERT(mbedtls_sha256_self_test(verbose) == 0);

    return EXIT_SUCCESS;
}

int32_t board_sha512_selftest(int32_t verbose)
{
    TEST_ASSERT(mbedtls_sha512_self_test(verbose) == 0);

    return EXIT_SUCCESS;
}
