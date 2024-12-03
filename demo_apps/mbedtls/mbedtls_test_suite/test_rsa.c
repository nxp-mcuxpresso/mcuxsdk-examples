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

#include "mbedtls/rsa.h"
#include "mbedtls/rsa_internal.h"
#include "mbedtls/md2.h"
#include "mbedtls/md4.h"
#include "mbedtls/md5.h"
#include "mbedtls/sha1.h"
#include "mbedtls/sha256.h"
#include "mbedtls/sha512.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"

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
#define INPUT_N_TYPE 1
#define INPUT_E_TYPE 2
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
char *board_input_N;
char *board_input_E;
/*******************************************************************************
 * Code
 ******************************************************************************/

/* BEGIN_CASE */
int32_t board_mbedtls_rsa_pkcs1_sign(const data_t *message_str,
                                     int32_t padding_mode,
                                     int32_t digest,
                                     int32_t mod,
                                     int32_t radix_P,
                                     const char *input_P,
                                     int32_t radix_Q,
                                     const char *input_Q,
                                     int32_t radix_N,
                                     const char *input_N,
                                     int32_t radix_E,
                                     const char *input_E,
                                     const data_t *result_hex_str,
                                     int32_t board_result)
{
    unsigned char hash_result[1000];
    unsigned char output[1000];
    mbedtls_rsa_context ctx;
    mbedtls_mpi N, P, Q, E;
    rnd_pseudo_info rnd_info;

    mbedtls_mpi_init(&N);
    mbedtls_mpi_init(&P);
    mbedtls_mpi_init(&Q);
    mbedtls_mpi_init(&E);
    mbedtls_rsa_init(&ctx, padding_mode, 0);

    memset(hash_result, 0x00, 1000);
    memset(output, 0x00, 1000);
    memset(&rnd_info, 0, sizeof(rnd_pseudo_info));

    TEST_ASSERT(mbedtls_mpi_read_string(&P, radix_P, input_P) == 0);
    TEST_ASSERT(mbedtls_mpi_read_string(&Q, radix_Q, input_Q) == 0);
    TEST_ASSERT(mbedtls_mpi_read_string(&N, radix_N, input_N) == 0);
    TEST_ASSERT(mbedtls_mpi_read_string(&E, radix_E, input_E) == 0);

    TEST_ASSERT(mbedtls_rsa_import(&ctx, &N, &P, &Q, NULL, &E) == 0);
    TEST_ASSERT(mbedtls_rsa_get_len(&ctx) == (size_t)(mod / 8));
    TEST_ASSERT(mbedtls_rsa_complete(&ctx) == 0);
    TEST_ASSERT(mbedtls_rsa_check_privkey(&ctx) == 0);

    if (mbedtls_md_info_from_type(digest) != NULL)
        TEST_ASSERT(mbedtls_md(mbedtls_md_info_from_type(digest), message_str->x, message_str->len, hash_result) == 0);

    TEST_ASSERT(mbedtls_rsa_pkcs1_sign(&ctx, &rnd_pseudo_rand, &rnd_info, MBEDTLS_RSA_PRIVATE, digest, 0, hash_result,
                                       output) == board_result);
    if (board_result == 0)
    {
        TEST_ASSERT(hexcmp(output, result_hex_str->x, ctx.len, result_hex_str->len) == 0);
    }

    mbedtls_mpi_free(&N);
    mbedtls_mpi_free(&P);
    mbedtls_mpi_free(&Q);
    mbedtls_mpi_free(&E);
    mbedtls_rsa_free(&ctx);
    return EXIT_SUCCESS;
}
/* END_CASE */

/* BEGIN_CASE */
int32_t board_mbedtls_rsa_pkcs1_verify(const data_t *message_str,
                                       int32_t padding_mode,
                                       int32_t digest,
                                       int32_t mod,
                                       int32_t radix_N,
                                       const char *input_N,
                                       int32_t radix_E,
                                       const char *input_E,
                                       const data_t *result_str,
                                       int32_t board_result)
{
    unsigned char hash_result[1000];
    mbedtls_rsa_context ctx;

    mbedtls_mpi N, E;

    mbedtls_mpi_init(&N);
    mbedtls_mpi_init(&E);
    mbedtls_rsa_init(&ctx, padding_mode, 0);
    memset(hash_result, 0x00, 1000);

    TEST_ASSERT(mbedtls_mpi_read_string(&N, radix_N, input_N) == 0);
    TEST_ASSERT(mbedtls_mpi_read_string(&E, radix_E, input_E) == 0);
    TEST_ASSERT(mbedtls_rsa_import(&ctx, &N, NULL, NULL, NULL, &E) == 0);
    TEST_ASSERT(mbedtls_rsa_get_len(&ctx) == (size_t)(mod / 8));
    TEST_ASSERT(mbedtls_rsa_check_pubkey(&ctx) == 0);

    if (mbedtls_md_info_from_type(digest) != NULL)
        TEST_ASSERT(mbedtls_md(mbedtls_md_info_from_type(digest), message_str->x, message_str->len, hash_result) == 0);

    TEST_ASSERT(mbedtls_rsa_pkcs1_verify(&ctx, NULL, NULL, MBEDTLS_RSA_PUBLIC, digest, 0, hash_result, result_str->x) ==
                board_result);

    mbedtls_mpi_free(&N);
    mbedtls_mpi_free(&E);
    mbedtls_rsa_free(&ctx);
    return EXIT_SUCCESS;
}
/* END_CASE */

/* BEGIN_CASE */
int32_t board_rsa_pkcs1_sign_raw(const data_t *hash_result,
                                 int32_t padding_mode,
                                 int32_t mod,
                                 int32_t radix_P,
                                 const char *input_P,
                                 int32_t radix_Q,
                                 const char *input_Q,
                                 int32_t radix_N,
                                 const char *input_N,
                                 int32_t radix_E,
                                 const char *input_E,
                                 const data_t *result_hex_str)
{
    unsigned char output[1000];
    mbedtls_rsa_context ctx;
    mbedtls_mpi N, P, Q, E;
    rnd_pseudo_info rnd_info;

    mbedtls_rsa_init(&ctx, padding_mode, 0);
    mbedtls_mpi_init(&N);
    mbedtls_mpi_init(&P);
    mbedtls_mpi_init(&Q);
    mbedtls_mpi_init(&E);

    memset(output, 0x00, 1000);
    memset(&rnd_info, 0, sizeof(rnd_pseudo_info));

    TEST_ASSERT(mbedtls_mpi_read_string(&P, radix_P, input_P) == 0);
    TEST_ASSERT(mbedtls_mpi_read_string(&Q, radix_Q, input_Q) == 0);
    TEST_ASSERT(mbedtls_mpi_read_string(&N, radix_N, input_N) == 0);
    TEST_ASSERT(mbedtls_mpi_read_string(&E, radix_E, input_E) == 0);

    TEST_ASSERT(mbedtls_rsa_import(&ctx, &N, &P, &Q, NULL, &E) == 0);
    TEST_ASSERT(mbedtls_rsa_get_len(&ctx) == (size_t)(mod / 8));
    TEST_ASSERT(mbedtls_rsa_complete(&ctx) == 0);
    TEST_ASSERT(mbedtls_rsa_check_privkey(&ctx) == 0);

    TEST_ASSERT(mbedtls_rsa_pkcs1_sign(&ctx, &rnd_pseudo_rand, &rnd_info, MBEDTLS_RSA_PRIVATE, MBEDTLS_MD_NONE,
                                       hash_result->len, hash_result->x, output) == 0);

    TEST_ASSERT(hexcmp(output, result_hex_str->x, ctx.len, result_hex_str->len) == 0);

#if defined(MBEDTLS_PKCS1_V15)
    /* For PKCS#1 v1.5, there is an alternative way to generate signatures */
    if (padding_mode == MBEDTLS_RSA_PKCS_V15)
    {
        int res;
        memset(output, 0x00, 1000);

        res = mbedtls_rsa_rsaes_pkcs1_v15_encrypt(&ctx, &rnd_pseudo_rand, &rnd_info, MBEDTLS_RSA_PRIVATE,
                                                  hash_result->len, hash_result->x, output);

#if !defined(MBEDTLS_RSA_ALT)
        TEST_ASSERT(res == 0);
#else
        TEST_ASSERT((res == 0) || (res == MBEDTLS_ERR_RSA_UNSUPPORTED_OPERATION));
#endif

        if (res == 0)
        {
            TEST_ASSERT(hexcmp(output, result_hex_str->x, ctx.len, result_hex_str->len) == 0);
        }
    }
#endif /* MBEDTLS_PKCS1_V15 */

    mbedtls_mpi_free(&N);
    mbedtls_mpi_free(&P);
    mbedtls_mpi_free(&Q);
    mbedtls_mpi_free(&E);

    mbedtls_rsa_free(&ctx);

    return EXIT_SUCCESS;
}
/* END_CASE */

/* BEGIN_CASE */
int32_t board_rsa_pkcs1_verify_raw(const data_t *hash_result,
                                   int32_t padding_mode,
                                   int32_t mod,
                                   int32_t radix_N,
                                   const char *input_N,
                                   int32_t radix_E,
                                   const char *input_E,
                                   const data_t *result_str,
                                   int32_t correct)
{
    unsigned char output[1000];
    mbedtls_rsa_context ctx;

    mbedtls_mpi N, E;
    mbedtls_mpi_init(&N);
    mbedtls_mpi_init(&E);

    mbedtls_rsa_init(&ctx, padding_mode, 0);
    memset(output, 0x00, sizeof(output));

    TEST_ASSERT(mbedtls_mpi_read_string(&N, radix_N, input_N) == 0);
    TEST_ASSERT(mbedtls_mpi_read_string(&E, radix_E, input_E) == 0);

    TEST_ASSERT(mbedtls_rsa_import(&ctx, &N, NULL, NULL, NULL, &E) == 0);
    TEST_ASSERT(mbedtls_rsa_get_len(&ctx) == (size_t)(mod / 8));
    TEST_ASSERT(mbedtls_rsa_check_pubkey(&ctx) == 0);

    TEST_ASSERT(mbedtls_rsa_pkcs1_verify(&ctx, NULL, NULL, MBEDTLS_RSA_PUBLIC, MBEDTLS_MD_NONE, hash_result->len,
                                         hash_result->x, result_str->x) == correct);

#if defined(MBEDTLS_PKCS1_V15)
    /* For PKCS#1 v1.5, there is an alternative way to verify signatures */
    if (padding_mode == MBEDTLS_RSA_PKCS_V15)
    {
        int res;
        int ok;
        size_t olen;

        res = mbedtls_rsa_rsaes_pkcs1_v15_decrypt(&ctx, NULL, NULL, MBEDTLS_RSA_PUBLIC, &olen, result_str->x, output,
                                                  sizeof(output));

#if !defined(MBEDTLS_RSA_ALT)
        TEST_ASSERT(res == 0);
#else
        TEST_ASSERT((res == 0) || (res == MBEDTLS_ERR_RSA_UNSUPPORTED_OPERATION));
#endif

        if (res == 0)
        {
            ok = olen == hash_result->len && memcmp(output, hash_result->x, olen) == 0;
            if (correct == 0)
                TEST_ASSERT(ok == 1);
            else
                TEST_ASSERT(ok == 0);
        }
    }
#endif /* MBEDTLS_PKCS1_V15 */

    mbedtls_mpi_free(&N);
    mbedtls_mpi_free(&E);
    mbedtls_rsa_free(&ctx);

    return EXIT_SUCCESS;
}
/* END_CASE */

/* BEGIN_CASE */
int32_t board_mbedtls_rsa_pkcs1_encrypt(const data_t *message_str,
                                        int32_t padding_mode,
                                        int32_t mod,
                                        int32_t radix_N,
                                        const char *input_N,
                                        int32_t radix_E,
                                        const char *input_E,
                                        const data_t *result_hex_str,
                                        int32_t board_result)
{
    unsigned char output[1000];
    mbedtls_rsa_context ctx;
    rnd_pseudo_info rnd_info;

    mbedtls_mpi N, E;
    mbedtls_mpi_init(&N);
    mbedtls_mpi_init(&E);

    memset(&rnd_info, 0, sizeof(rnd_pseudo_info));

    mbedtls_rsa_init(&ctx, padding_mode, 0);
    memset(output, 0x00, 1000);

    TEST_ASSERT(mbedtls_mpi_read_string(&N, radix_N, input_N) == 0);
    TEST_ASSERT(mbedtls_mpi_read_string(&E, radix_E, input_E) == 0);

    TEST_ASSERT(mbedtls_rsa_import(&ctx, &N, NULL, NULL, NULL, &E) == 0);
    TEST_ASSERT(mbedtls_rsa_get_len(&ctx) == (size_t)(mod / 8));
    TEST_ASSERT(mbedtls_rsa_check_pubkey(&ctx) == 0);

    TEST_ASSERT(mbedtls_rsa_pkcs1_encrypt(&ctx, &rnd_pseudo_rand, &rnd_info, MBEDTLS_RSA_PUBLIC, message_str->len,
                                          message_str->x, output) == board_result);
    if (board_result == 0)
    {
        TEST_ASSERT(hexcmp(output, result_hex_str->x, ctx.len, result_hex_str->len) == 0);
    }

    mbedtls_mpi_free(&N);
    mbedtls_mpi_free(&E);
    mbedtls_rsa_free(&ctx);

    return EXIT_SUCCESS;
}
/* END_CASE */

/* BEGIN_CASE */
int32_t board_rsa_pkcs1_encrypt_bad_rng(const data_t *message_str,
                                        int32_t padding_mode,
                                        int32_t mod,
                                        int32_t radix_N,
                                        const char *input_N,
                                        int32_t radix_E,
                                        const char *input_E,
                                        const data_t *result_hex_str,
                                        int32_t board_result)
{
    unsigned char output[1000];
    mbedtls_rsa_context ctx;

    mbedtls_mpi N, E;

    mbedtls_mpi_init(&N);
    mbedtls_mpi_init(&E);
    mbedtls_rsa_init(&ctx, padding_mode, 0);
    memset(output, 0x00, 1000);

    TEST_ASSERT(mbedtls_mpi_read_string(&N, radix_N, input_N) == 0);
    TEST_ASSERT(mbedtls_mpi_read_string(&E, radix_E, input_E) == 0);

    TEST_ASSERT(mbedtls_rsa_import(&ctx, &N, NULL, NULL, NULL, &E) == 0);
    TEST_ASSERT(mbedtls_rsa_get_len(&ctx) == (size_t)(mod / 8));
    TEST_ASSERT(mbedtls_rsa_check_pubkey(&ctx) == 0);

    TEST_ASSERT(mbedtls_rsa_pkcs1_encrypt(&ctx, &rnd_zero_rand, NULL, MBEDTLS_RSA_PUBLIC, message_str->len,
                                          message_str->x, output) == board_result);
    if (board_result == 0)
    {
        TEST_ASSERT(hexcmp(output, result_hex_str->x, ctx.len, result_hex_str->len) == 0);
    }

    mbedtls_mpi_free(&N);
    mbedtls_mpi_free(&E);
    mbedtls_rsa_free(&ctx);

    return EXIT_SUCCESS;
}
/* END_CASE */

/* BEGIN_CASE */
int32_t board_mbedtls_rsa_pkcs1_decrypt(const data_t *message_str,
                                        int32_t padding_mode,
                                        int32_t mod,
                                        int32_t radix_P,
                                        const char *input_P,
                                        int32_t radix_Q,
                                        const char *input_Q,
                                        int32_t radix_N,
                                        const char *input_N,
                                        int32_t radix_E,
                                        const char *input_E,
                                        int32_t max_output,
                                        const data_t *result_hex_str,
                                        int32_t board_result)
{
    unsigned char output[1000];
    mbedtls_rsa_context ctx;
    size_t output_len;
    rnd_pseudo_info rnd_info;
    mbedtls_mpi N, P, Q, E;

    mbedtls_mpi_init(&N);
    mbedtls_mpi_init(&P);
    mbedtls_mpi_init(&Q);
    mbedtls_mpi_init(&E);

    mbedtls_rsa_init(&ctx, padding_mode, 0);

    memset(output, 0x00, 1000);
    memset(&rnd_info, 0, sizeof(rnd_pseudo_info));

    TEST_ASSERT(mbedtls_mpi_read_string(&P, radix_P, input_P) == 0);
    TEST_ASSERT(mbedtls_mpi_read_string(&Q, radix_Q, input_Q) == 0);
    TEST_ASSERT(mbedtls_mpi_read_string(&N, radix_N, input_N) == 0);
    TEST_ASSERT(mbedtls_mpi_read_string(&E, radix_E, input_E) == 0);

    TEST_ASSERT(mbedtls_rsa_import(&ctx, &N, &P, &Q, NULL, &E) == 0);
    TEST_ASSERT(mbedtls_rsa_get_len(&ctx) == (size_t)(mod / 8));
    TEST_ASSERT(mbedtls_rsa_complete(&ctx) == 0);
    TEST_ASSERT(mbedtls_rsa_check_privkey(&ctx) == 0);

    output_len = 0;

    TEST_ASSERT(mbedtls_rsa_pkcs1_decrypt(&ctx, rnd_pseudo_rand, &rnd_info, MBEDTLS_RSA_PRIVATE, &output_len,
                                          message_str->x, output, max_output) == board_result);
    if (board_result == 0)
    {
        TEST_ASSERT(hexcmp(output, result_hex_str->x, output_len, result_hex_str->len) == 0);
    }

    mbedtls_mpi_free(&N);
    mbedtls_mpi_free(&P);
    mbedtls_mpi_free(&Q);
    mbedtls_mpi_free(&E);
    mbedtls_rsa_free(&ctx);

    return EXIT_SUCCESS;
}
/* END_CASE */

/* BEGIN_CASE */
int32_t board_mbedtls_rsa_public(const data_t *message_str,
                                 int32_t mod,
                                 int32_t radix_N,
                                 const char *input_N,
                                 int32_t radix_E,
                                 const char *input_E,
                                 const data_t *result_hex_str,
                                 int32_t board_result)
{
    unsigned char output[1000];
    mbedtls_rsa_context ctx, ctx2; /* Also test mbedtls_rsa_copy() while at it */

    mbedtls_mpi N, E;

    mbedtls_mpi_init(&N);
    mbedtls_mpi_init(&E);
    mbedtls_rsa_init(&ctx, MBEDTLS_RSA_PKCS_V15, 0);
    mbedtls_rsa_init(&ctx2, MBEDTLS_RSA_PKCS_V15, 0);
    memset(output, 0x00, 1000);

    TEST_ASSERT(mbedtls_mpi_read_string(&N, radix_N, input_N) == 0);
    TEST_ASSERT(mbedtls_mpi_read_string(&E, radix_E, input_E) == 0);

    TEST_ASSERT(mbedtls_rsa_import(&ctx, &N, NULL, NULL, NULL, &E) == 0);
    TEST_ASSERT(mbedtls_rsa_get_len(&ctx) == (size_t)(mod / 8));
    TEST_ASSERT(mbedtls_rsa_check_pubkey(&ctx) == 0);

    TEST_ASSERT(mbedtls_rsa_public(&ctx, message_str->x, output) == board_result);
    if (board_result == 0)
    {
        TEST_ASSERT(hexcmp(output, result_hex_str->x, ctx.len, result_hex_str->len) == 0);
    }

    /* And now with the copy */
    TEST_ASSERT(mbedtls_rsa_copy(&ctx2, &ctx) == 0);
    /* clear the original to be sure */
    mbedtls_rsa_free(&ctx);

    TEST_ASSERT(mbedtls_rsa_check_pubkey(&ctx2) == 0);

    memset(output, 0x00, 1000);
    TEST_ASSERT(mbedtls_rsa_public(&ctx2, message_str->x, output) == board_result);
    if (board_result == 0)
    {
        TEST_ASSERT(hexcmp(output, result_hex_str->x, ctx.len, result_hex_str->len) == 0);
    }

    mbedtls_mpi_free(&N);
    mbedtls_mpi_free(&E);
    mbedtls_rsa_free(&ctx);
    mbedtls_rsa_free(&ctx2);

    return EXIT_SUCCESS;
}
/* END_CASE */

/* BEGIN_CASE */
int32_t board_mbedtls_rsa_private(const data_t *message_str,
                                  int32_t mod,
                                  int32_t radix_P,
                                  const char *input_P,
                                  int32_t radix_Q,
                                  const char *input_Q,
                                  int32_t radix_N,
                                  const char *input_N,
                                  int32_t radix_E,
                                  const char *input_E,
                                  const data_t *result_hex_str,
                                  int32_t board_result)
{
    unsigned char output[1000];
    mbedtls_rsa_context ctx, ctx2; /* Also test mbedtls_rsa_copy() while at it */
    mbedtls_mpi N, P, Q, E;
    rnd_pseudo_info rnd_info;
    int i;

    mbedtls_mpi_init(&N);
    mbedtls_mpi_init(&P);
    mbedtls_mpi_init(&Q);
    mbedtls_mpi_init(&E);
    mbedtls_rsa_init(&ctx, MBEDTLS_RSA_PKCS_V15, 0);
    mbedtls_rsa_init(&ctx2, MBEDTLS_RSA_PKCS_V15, 0);

    memset(&rnd_info, 0, sizeof(rnd_pseudo_info));

    TEST_ASSERT(mbedtls_mpi_read_string(&P, radix_P, input_P) == 0);
    TEST_ASSERT(mbedtls_mpi_read_string(&Q, radix_Q, input_Q) == 0);
    TEST_ASSERT(mbedtls_mpi_read_string(&N, radix_N, input_N) == 0);
    TEST_ASSERT(mbedtls_mpi_read_string(&E, radix_E, input_E) == 0);

    TEST_ASSERT(mbedtls_rsa_import(&ctx, &N, &P, &Q, NULL, &E) == 0);
    TEST_ASSERT(mbedtls_rsa_get_len(&ctx) == (size_t)(mod / 8));
    TEST_ASSERT(mbedtls_rsa_complete(&ctx) == 0);
    TEST_ASSERT(mbedtls_rsa_check_privkey(&ctx) == 0);

    /* repeat three times to test updating of blinding values */
    for (i = 0; i < 3; i++)
    {
        memset(output, 0x00, 1000);
        TEST_ASSERT(mbedtls_rsa_private(&ctx, rnd_pseudo_rand, &rnd_info, message_str->x, output) == board_result);
        if (board_result == 0)
        {
            TEST_ASSERT(hexcmp(output, result_hex_str->x, ctx.len, result_hex_str->len) == 0);
        }
    }

    /* And now one more time with the copy */
    TEST_ASSERT(mbedtls_rsa_copy(&ctx2, &ctx) == 0);
    /* clear the original to be sure */
    mbedtls_rsa_free(&ctx);

    TEST_ASSERT(mbedtls_rsa_check_privkey(&ctx2) == 0);

    memset(output, 0x00, 1000);
    TEST_ASSERT(mbedtls_rsa_private(&ctx2, rnd_pseudo_rand, &rnd_info, message_str->x, output) == board_result);
    if (board_result == 0)
    {
        TEST_ASSERT(hexcmp(output, result_hex_str->x, ctx2.len, result_hex_str->len) == 0);
    }

    mbedtls_mpi_free(&N);
    mbedtls_mpi_free(&P);
    mbedtls_mpi_free(&Q);
    mbedtls_mpi_free(&E);

    mbedtls_rsa_free(&ctx);
    mbedtls_rsa_free(&ctx2);

    return EXIT_SUCCESS;
}
/* END_CASE */

/* BEGIN_CASE */
int32_t board_rsa_check_privkey_null()
{
    mbedtls_rsa_context ctx;
    memset(&ctx, 0x00, sizeof(mbedtls_rsa_context));

    TEST_ASSERT(mbedtls_rsa_check_privkey(&ctx) == MBEDTLS_ERR_RSA_KEY_CHECK_FAILED);

    return EXIT_SUCCESS;
}
/* END_CASE */

int32_t board_mbedtls_rsa_check_pubkey(
    int32_t radix_N, const char *input_N, int32_t radix_E, const char *input_E, int32_t board_result)
{
    mbedtls_rsa_context ctx;
    mbedtls_mpi N, E;

    mbedtls_mpi_init(&N);
    mbedtls_mpi_init(&E);
    mbedtls_rsa_init(&ctx, MBEDTLS_RSA_PKCS_V15, 0);

    if (strlen(input_N))
    {
        TEST_ASSERT(mbedtls_mpi_read_string(&N, radix_N, input_N) == 0);
    }
    if (strlen(input_E))
    {
        TEST_ASSERT(mbedtls_mpi_read_string(&E, radix_E, input_E) == 0);
    }

    TEST_ASSERT(mbedtls_rsa_import(&ctx, &N, NULL, NULL, NULL, &E) == 0);
    TEST_ASSERT(mbedtls_rsa_check_pubkey(&ctx) == board_result);

    mbedtls_mpi_free(&N);
    mbedtls_mpi_free(&E);
    mbedtls_rsa_free(&ctx);

    return EXIT_SUCCESS;
}

/* BEGIN_CASE */
int32_t board_mbedtls_rsa_check_privkey(int32_t mod,
                                        int32_t radix_P,
                                        const char *input_P,
                                        int32_t radix_Q,
                                        const char *input_Q,
                                        int32_t radix_N,
                                        const char *input_N,
                                        int32_t radix_E,
                                        const char *input_E,
                                        int32_t radix_D,
                                        const char *input_D,
                                        int32_t radix_DP,
                                        const char *input_DP,
                                        int32_t radix_DQ,
                                        const char *input_DQ,
                                        int32_t radix_QP,
                                        const char *input_QP,
                                        int32_t board_result)
{
    mbedtls_rsa_context ctx;

    mbedtls_rsa_init(&ctx, MBEDTLS_RSA_PKCS_V15, 0);

    ctx.len = mod / 8;
    if (strlen(input_P))
    {
        TEST_ASSERT(mbedtls_mpi_read_string(&ctx.P, radix_P, input_P) == 0);
    }
    if (strlen(input_Q))
    {
        TEST_ASSERT(mbedtls_mpi_read_string(&ctx.Q, radix_Q, input_Q) == 0);
    }
    if (strlen(input_N))
    {
        TEST_ASSERT(mbedtls_mpi_read_string(&ctx.N, radix_N, input_N) == 0);
    }
    if (strlen(input_E))
    {
        TEST_ASSERT(mbedtls_mpi_read_string(&ctx.E, radix_E, input_E) == 0);
    }
    if (strlen(input_D))
    {
        TEST_ASSERT(mbedtls_mpi_read_string(&ctx.D, radix_D, input_D) == 0);
    }
#if !defined(MBEDTLS_RSA_NO_CRT)
    if (strlen(input_DP))
    {
        TEST_ASSERT(mbedtls_mpi_read_string(&ctx.DP, radix_DP, input_DP) == 0);
    }
    if (strlen(input_DQ))
    {
        TEST_ASSERT(mbedtls_mpi_read_string(&ctx.DQ, radix_DQ, input_DQ) == 0);
    }
    if (strlen(input_QP))
    {
        TEST_ASSERT(mbedtls_mpi_read_string(&ctx.QP, radix_QP, input_QP) == 0);
    }
#else
    ((void)radix_DP);
    ((void)input_DP);
    ((void)radix_DQ);
    ((void)input_DQ);
    ((void)radix_QP);
    ((void)input_QP);
#endif

    TEST_ASSERT(mbedtls_rsa_check_privkey(&ctx) == board_result);

    mbedtls_rsa_free(&ctx);
    return EXIT_SUCCESS;
}
/* END_CASE */

/* BEGIN_CASE */
int32_t board_rsa_check_pubpriv(int32_t mod,
                                int32_t radix_Npub,
                                const char *input_Npub,
                                int32_t radix_Epub,
                                const char *input_Epub,
                                int32_t radix_P,
                                const char *input_P,
                                int32_t radix_Q,
                                const char *input_Q,
                                int32_t radix_N,
                                const char *input_N,
                                int32_t radix_E,
                                const char *input_E,
                                int32_t radix_D,
                                const char *input_D,
                                int32_t radix_DP,
                                const char *input_DP,
                                int32_t radix_DQ,
                                const char *input_DQ,
                                int32_t radix_QP,
                                const char *input_QP,
                                int32_t board_result)
{
    mbedtls_rsa_context pub, prv;

    mbedtls_rsa_init(&pub, MBEDTLS_RSA_PKCS_V15, 0);
    mbedtls_rsa_init(&prv, MBEDTLS_RSA_PKCS_V15, 0);

    pub.len = mod / 8;
    prv.len = mod / 8;

    if (strlen(input_Npub))
    {
        TEST_ASSERT(mbedtls_mpi_read_string(&pub.N, radix_Npub, input_Npub) == 0);
    }
    if (strlen(input_Epub))
    {
        TEST_ASSERT(mbedtls_mpi_read_string(&pub.E, radix_Epub, input_Epub) == 0);
    }

    if (strlen(input_P))
    {
        TEST_ASSERT(mbedtls_mpi_read_string(&prv.P, radix_P, input_P) == 0);
    }
    if (strlen(input_Q))
    {
        TEST_ASSERT(mbedtls_mpi_read_string(&prv.Q, radix_Q, input_Q) == 0);
    }
    if (strlen(input_N))
    {
        TEST_ASSERT(mbedtls_mpi_read_string(&prv.N, radix_N, input_N) == 0);
    }
    if (strlen(input_E))
    {
        TEST_ASSERT(mbedtls_mpi_read_string(&prv.E, radix_E, input_E) == 0);
    }
    if (strlen(input_D))
    {
        TEST_ASSERT(mbedtls_mpi_read_string(&prv.D, radix_D, input_D) == 0);
    }
#if !defined(MBEDTLS_RSA_NO_CRT)
    if (strlen(input_DP))
    {
        TEST_ASSERT(mbedtls_mpi_read_string(&prv.DP, radix_DP, input_DP) == 0);
    }
    if (strlen(input_DQ))
    {
        TEST_ASSERT(mbedtls_mpi_read_string(&prv.DQ, radix_DQ, input_DQ) == 0);
    }
    if (strlen(input_QP))
    {
        TEST_ASSERT(mbedtls_mpi_read_string(&prv.QP, radix_QP, input_QP) == 0);
    }
#else
    ((void)radix_DP);
    ((void)input_DP);
    ((void)radix_DQ);
    ((void)input_DQ);
    ((void)radix_QP);
    ((void)input_QP);
#endif

    TEST_ASSERT(mbedtls_rsa_check_pub_priv(&pub, &prv) == board_result);

    mbedtls_rsa_free(&pub);
    mbedtls_rsa_free(&prv);
    return EXIT_SUCCESS;
}
/* END_CASE */

/* BEGIN_CASE depends_on:MBEDTLS_CTR_DRBG_C:MBEDTLS_ENTROPY_C */
int32_t board_mbedtls_rsa_gen_key(int32_t nrbits, int32_t exponent, int32_t board_result)
{
    mbedtls_rsa_context ctx;
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    const char *pers = "test_suite_rsa";

    mbedtls_ctr_drbg_init(&ctr_drbg);
    mbedtls_entropy_init(&entropy);
    mbedtls_rsa_init(&ctx, 0, 0);

    TEST_ASSERT(mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, (const unsigned char *)pers,
                                      strlen(pers)) == 0);

    TEST_ASSERT(mbedtls_rsa_gen_key(&ctx, mbedtls_ctr_drbg_random, &ctr_drbg, nrbits, exponent) == board_result);
    if (board_result == 0)
    {
        TEST_ASSERT(mbedtls_rsa_check_privkey(&ctx) == 0);
        TEST_ASSERT(mbedtls_mpi_cmp_mpi(&ctx.P, &ctx.Q) > 0);
    }

    mbedtls_rsa_free(&ctx);
    mbedtls_ctr_drbg_free(&ctr_drbg);
    mbedtls_entropy_free(&entropy);

    return EXIT_SUCCESS;
}
/* END_CASE */

/* BEGIN_CASE */
int32_t board_mbedtls_rsa_deduce_primes(int32_t radix_N,
                                        const char *input_N,
                                        int32_t radix_D,
                                        const char *input_D,
                                        int32_t radix_E,
                                        const char *input_E,
                                        int32_t radix_P,
                                        const char *output_P,
                                        int32_t radix_Q,
                                        const char *output_Q,
                                        int32_t corrupt,
                                        int32_t board_result)
{
    mbedtls_mpi N, P, Pp, Q, Qp, D, E;

    mbedtls_mpi_init(&N);
    mbedtls_mpi_init(&P);
    mbedtls_mpi_init(&Q);
    mbedtls_mpi_init(&Pp);
    mbedtls_mpi_init(&Qp);
    mbedtls_mpi_init(&D);
    mbedtls_mpi_init(&E);

    TEST_ASSERT(mbedtls_mpi_read_string(&N, radix_N, input_N) == 0);
    TEST_ASSERT(mbedtls_mpi_read_string(&D, radix_D, input_D) == 0);
    TEST_ASSERT(mbedtls_mpi_read_string(&E, radix_E, input_E) == 0);
    TEST_ASSERT(mbedtls_mpi_read_string(&Qp, radix_P, output_P) == 0);
    TEST_ASSERT(mbedtls_mpi_read_string(&Pp, radix_Q, output_Q) == 0);

    if (corrupt)
        TEST_ASSERT(mbedtls_mpi_add_int(&D, &D, 2) == 0);

    /* Try to deduce P, Q from N, D, E only. */
    TEST_ASSERT(mbedtls_rsa_deduce_primes(&N, &D, &E, &P, &Q) == board_result);

    if (!corrupt)
    {
        /* Check if (P,Q) = (Pp, Qp) or (P,Q) = (Qp, Pp) */
        TEST_ASSERT((mbedtls_mpi_cmp_mpi(&P, &Pp) == 0 && mbedtls_mpi_cmp_mpi(&Q, &Qp) == 0) ||
                    (mbedtls_mpi_cmp_mpi(&P, &Qp) == 0 && mbedtls_mpi_cmp_mpi(&Q, &Pp) == 0));
    }

    mbedtls_mpi_free(&N);
    mbedtls_mpi_free(&P);
    mbedtls_mpi_free(&Q);
    mbedtls_mpi_free(&Pp);
    mbedtls_mpi_free(&Qp);
    mbedtls_mpi_free(&D);
    mbedtls_mpi_free(&E);

    return EXIT_SUCCESS;
}
/* END_CASE */

/* BEGIN_CASE */
int32_t board_mbedtls_rsa_deduce_private_exponent(int32_t radix_P,
                                                  const char *input_P,
                                                  int32_t radix_Q,
                                                  const char *input_Q,
                                                  int32_t radix_E,
                                                  const char *input_E,
                                                  int32_t radix_D,
                                                  const char *output_D,
                                                  int32_t corrupt,
                                                  int32_t board_result)
{
    mbedtls_mpi P, Q, D, Dp, E, R, Rp;

    mbedtls_mpi_init(&P);
    mbedtls_mpi_init(&Q);
    mbedtls_mpi_init(&D);
    mbedtls_mpi_init(&Dp);
    mbedtls_mpi_init(&E);
    mbedtls_mpi_init(&R);
    mbedtls_mpi_init(&Rp);

    TEST_ASSERT(mbedtls_mpi_read_string(&P, radix_P, input_P) == 0);
    TEST_ASSERT(mbedtls_mpi_read_string(&Q, radix_Q, input_Q) == 0);
    TEST_ASSERT(mbedtls_mpi_read_string(&E, radix_E, input_E) == 0);
    TEST_ASSERT(mbedtls_mpi_read_string(&Dp, radix_D, output_D) == 0);

    if (corrupt)
    {
        /* Make E even */
        TEST_ASSERT(mbedtls_mpi_set_bit(&E, 0, 0) == 0);
    }

    /* Try to deduce D from N, P, Q, E. */
    TEST_ASSERT(mbedtls_rsa_deduce_private_exponent(&P, &Q, &E, &D) == board_result);

    if (!corrupt)
    {
        /*
         * Check that D and Dp agree modulo LCM(P-1, Q-1).
         */

        /* Replace P,Q by P-1, Q-1 */
        TEST_ASSERT(mbedtls_mpi_sub_int(&P, &P, 1) == 0);
        TEST_ASSERT(mbedtls_mpi_sub_int(&Q, &Q, 1) == 0);

        /* Check D == Dp modulo P-1 */
        TEST_ASSERT(mbedtls_mpi_mod_mpi(&R, &D, &P) == 0);
        TEST_ASSERT(mbedtls_mpi_mod_mpi(&Rp, &Dp, &P) == 0);
        TEST_ASSERT(mbedtls_mpi_cmp_mpi(&R, &Rp) == 0);

        /* Check D == Dp modulo Q-1 */
        TEST_ASSERT(mbedtls_mpi_mod_mpi(&R, &D, &Q) == 0);
        TEST_ASSERT(mbedtls_mpi_mod_mpi(&Rp, &Dp, &Q) == 0);
        TEST_ASSERT(mbedtls_mpi_cmp_mpi(&R, &Rp) == 0);
    }

exit:

    mbedtls_mpi_free(&P);
    mbedtls_mpi_free(&Q);
    mbedtls_mpi_free(&D);
    mbedtls_mpi_free(&Dp);
    mbedtls_mpi_free(&E);
    mbedtls_mpi_free(&R);
    mbedtls_mpi_free(&Rp);

    return EXIT_SUCCESS;
}
/* END_CASE */

/* BEGIN_CASE depends_on:MBEDTLS_CTR_DRBG_C:MBEDTLS_ENTROPY_C:ENTROPY_HAVE_STRONG */
int32_t board_mbedtls_rsa_import(int32_t radix_N,
                                 const char *input_N,
                                 int32_t radix_P,
                                 const char *input_P,
                                 int32_t radix_Q,
                                 const char *input_Q,
                                 int32_t radix_D,
                                 const char *input_D,
                                 int32_t radix_E,
                                 const char *input_E,
                                 int32_t successive,
                                 int32_t is_priv,
                                 int32_t res_check,
                                 int32_t res_complete)
{
    mbedtls_mpi N, P, Q, D, E;
    mbedtls_rsa_context ctx;

    /* Buffers used for encryption-decryption test */
    unsigned char *buf_orig = NULL;
    unsigned char *buf_enc  = NULL;
    unsigned char *buf_dec  = NULL;

    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    const char *pers = "test_suite_rsa";

    const int have_N = (strlen(input_N) > 0);
    const int have_P = (strlen(input_P) > 0);
    const int have_Q = (strlen(input_Q) > 0);
    const int have_D = (strlen(input_D) > 0);
    const int have_E = (strlen(input_E) > 0);

    mbedtls_ctr_drbg_init(&ctr_drbg);
    mbedtls_entropy_init(&entropy);
    mbedtls_rsa_init(&ctx, 0, 0);

    mbedtls_mpi_init(&N);
    mbedtls_mpi_init(&P);
    mbedtls_mpi_init(&Q);
    mbedtls_mpi_init(&D);
    mbedtls_mpi_init(&E);

    TEST_ASSERT(mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, (const unsigned char *)pers,
                                      strlen(pers)) == 0);

    if (have_N)
        TEST_ASSERT(mbedtls_mpi_read_string(&N, radix_N, input_N) == 0);

    if (have_P)
        TEST_ASSERT(mbedtls_mpi_read_string(&P, radix_P, input_P) == 0);

    if (have_Q)
        TEST_ASSERT(mbedtls_mpi_read_string(&Q, radix_Q, input_Q) == 0);

    if (have_D)
        TEST_ASSERT(mbedtls_mpi_read_string(&D, radix_D, input_D) == 0);

    if (have_E)
        TEST_ASSERT(mbedtls_mpi_read_string(&E, radix_E, input_E) == 0);

    if (!successive)
    {
        TEST_ASSERT(mbedtls_rsa_import(&ctx, have_N ? &N : NULL, have_P ? &P : NULL, have_Q ? &Q : NULL,
                                       have_D ? &D : NULL, have_E ? &E : NULL) == 0);
    }
    else
    {
        /* Import N, P, Q, D, E separately.
         * This should make no functional difference. */

        TEST_ASSERT(mbedtls_rsa_import(&ctx, have_N ? &N : NULL, NULL, NULL, NULL, NULL) == 0);

        TEST_ASSERT(mbedtls_rsa_import(&ctx, NULL, have_P ? &P : NULL, NULL, NULL, NULL) == 0);

        TEST_ASSERT(mbedtls_rsa_import(&ctx, NULL, NULL, have_Q ? &Q : NULL, NULL, NULL) == 0);

        TEST_ASSERT(mbedtls_rsa_import(&ctx, NULL, NULL, NULL, have_D ? &D : NULL, NULL) == 0);

        TEST_ASSERT(mbedtls_rsa_import(&ctx, NULL, NULL, NULL, NULL, have_E ? &E : NULL) == 0);
    }

    TEST_ASSERT(mbedtls_rsa_complete(&ctx) == res_complete);

    /* On expected success, perform some public and private
     * key operations to check if the key is working properly. */
    if (res_complete == 0)
    {
        if (is_priv)
            TEST_ASSERT(mbedtls_rsa_check_privkey(&ctx) == res_check);
        else
            TEST_ASSERT(mbedtls_rsa_check_pubkey(&ctx) == res_check);

        if (res_check != 0)
            goto exit;

        buf_orig = mbedtls_calloc(1, mbedtls_rsa_get_len(&ctx));
        buf_enc  = mbedtls_calloc(1, mbedtls_rsa_get_len(&ctx));
        buf_dec  = mbedtls_calloc(1, mbedtls_rsa_get_len(&ctx));
        if (buf_orig == NULL || buf_enc == NULL || buf_dec == NULL)
            goto exit;

        TEST_ASSERT(mbedtls_ctr_drbg_random(&ctr_drbg, buf_orig, mbedtls_rsa_get_len(&ctx)) == 0);

        /* Make sure the number we're generating is smaller than the modulus */
        buf_orig[0] = 0x00;

        TEST_ASSERT(mbedtls_rsa_public(&ctx, buf_orig, buf_enc) == 0);

        if (is_priv)
        {
            TEST_ASSERT(mbedtls_rsa_private(&ctx, mbedtls_ctr_drbg_random, &ctr_drbg, buf_enc, buf_dec) == 0);

            TEST_ASSERT(memcmp(buf_orig, buf_dec, mbedtls_rsa_get_len(&ctx)) == 0);
        }
    }

    mbedtls_free(buf_orig);
    mbedtls_free(buf_enc);
    mbedtls_free(buf_dec);

    mbedtls_rsa_free(&ctx);

    mbedtls_ctr_drbg_free(&ctr_drbg);
    mbedtls_entropy_free(&entropy);

    mbedtls_mpi_free(&N);
    mbedtls_mpi_free(&P);
    mbedtls_mpi_free(&Q);
    mbedtls_mpi_free(&D);
    mbedtls_mpi_free(&E);

    return EXIT_SUCCESS;

exit:
    return EXIT_FAILURE;
}
/* END_CASE */

/* BEGIN_CASE */
int32_t board_mbedtls_rsa_export(int32_t radix_N,
                                 const char *input_N,
                                 int32_t radix_P,
                                 const char *input_P,
                                 int32_t radix_Q,
                                 const char *input_Q,
                                 int32_t radix_D,
                                 const char *input_D,
                                 int32_t radix_E,
                                 const char *input_E,
                                 int32_t is_priv,
                                 int32_t successive)
{
    /* Original MPI's with which we set up the RSA context */
    mbedtls_mpi N, P, Q, D, E;

    /* Exported MPI's */
    mbedtls_mpi Ne, Pe, Qe, De, Ee;

    const int have_N = (strlen(input_N) > 0);
    const int have_P = (strlen(input_P) > 0);
    const int have_Q = (strlen(input_Q) > 0);
    const int have_D = (strlen(input_D) > 0);
    const int have_E = (strlen(input_E) > 0);

    mbedtls_rsa_context ctx;

    mbedtls_rsa_init(&ctx, 0, 0);

    mbedtls_mpi_init(&N);
    mbedtls_mpi_init(&P);
    mbedtls_mpi_init(&Q);
    mbedtls_mpi_init(&D);
    mbedtls_mpi_init(&E);

    mbedtls_mpi_init(&Ne);
    mbedtls_mpi_init(&Pe);
    mbedtls_mpi_init(&Qe);
    mbedtls_mpi_init(&De);
    mbedtls_mpi_init(&Ee);

    /* Setup RSA context */

    if (have_N)
        TEST_ASSERT(mbedtls_mpi_read_string(&N, radix_N, input_N) == 0);

    if (have_P)
        TEST_ASSERT(mbedtls_mpi_read_string(&P, radix_P, input_P) == 0);

    if (have_Q)
        TEST_ASSERT(mbedtls_mpi_read_string(&Q, radix_Q, input_Q) == 0);

    if (have_D)
        TEST_ASSERT(mbedtls_mpi_read_string(&D, radix_D, input_D) == 0);

    if (have_E)
        TEST_ASSERT(mbedtls_mpi_read_string(&E, radix_E, input_E) == 0);

    TEST_ASSERT(mbedtls_rsa_import(&ctx, strlen(input_N) ? &N : NULL, strlen(input_P) ? &P : NULL,
                                   strlen(input_Q) ? &Q : NULL, strlen(input_D) ? &D : NULL,
                                   strlen(input_E) ? &E : NULL) == 0);

    TEST_ASSERT(mbedtls_rsa_complete(&ctx) == 0);

    /*
     * Export parameters and compare to original ones.
     */

    /* N and E must always be present. */
    if (!successive)
    {
        TEST_ASSERT(mbedtls_rsa_export(&ctx, &Ne, NULL, NULL, NULL, &Ee) == 0);
    }
    else
    {
        TEST_ASSERT(mbedtls_rsa_export(&ctx, &Ne, NULL, NULL, NULL, NULL) == 0);
        TEST_ASSERT(mbedtls_rsa_export(&ctx, NULL, NULL, NULL, NULL, &Ee) == 0);
    }
    TEST_ASSERT(mbedtls_mpi_cmp_mpi(&N, &Ne) == 0);
    TEST_ASSERT(mbedtls_mpi_cmp_mpi(&E, &Ee) == 0);

    /* If we were providing enough information to setup a complete private context,
     * we expect to be able to export all core parameters. */

    if (is_priv)
    {
        if (!successive)
        {
            TEST_ASSERT(mbedtls_rsa_export(&ctx, NULL, &Pe, &Qe, &De, NULL) == 0);
        }
        else
        {
            TEST_ASSERT(mbedtls_rsa_export(&ctx, NULL, &Pe, NULL, NULL, NULL) == 0);
            TEST_ASSERT(mbedtls_rsa_export(&ctx, NULL, NULL, &Qe, NULL, NULL) == 0);
            TEST_ASSERT(mbedtls_rsa_export(&ctx, NULL, NULL, NULL, &De, NULL) == 0);
        }

        if (have_P)
            TEST_ASSERT(mbedtls_mpi_cmp_mpi(&P, &Pe) == 0);

        if (have_Q)
            TEST_ASSERT(mbedtls_mpi_cmp_mpi(&Q, &Qe) == 0);

        if (have_D)
            TEST_ASSERT(mbedtls_mpi_cmp_mpi(&D, &De) == 0);

        /* While at it, perform a sanity check */
        TEST_ASSERT(mbedtls_rsa_validate_params(&Ne, &Pe, &Qe, &De, &Ee, NULL, NULL) == 0);
    }

    mbedtls_rsa_free(&ctx);

    mbedtls_mpi_free(&N);
    mbedtls_mpi_free(&P);
    mbedtls_mpi_free(&Q);
    mbedtls_mpi_free(&D);
    mbedtls_mpi_free(&E);

    mbedtls_mpi_free(&Ne);
    mbedtls_mpi_free(&Pe);
    mbedtls_mpi_free(&Qe);
    mbedtls_mpi_free(&De);
    mbedtls_mpi_free(&Ee);

    return EXIT_SUCCESS;
}
/* END_CASE */

/* BEGIN_CASE depends_on:MBEDTLS_ENTROPY_C:ENTROPY_HAVE_STRONG */
int32_t board_mbedtls_rsa_validate_params(int32_t radix_N,
                                          const char *input_N,
                                          int32_t radix_P,
                                          const char *input_P,
                                          int32_t radix_Q,
                                          const char *input_Q,
                                          int32_t radix_D,
                                          const char *input_D,
                                          int32_t radix_E,
                                          const char *input_E,
                                          int32_t prng,
                                          int32_t board_result)
{
    /* Original MPI's with which we set up the RSA context */
    mbedtls_mpi N, P, Q, D, E;

    const int have_N = (strlen(input_N) > 0);
    const int have_P = (strlen(input_P) > 0);
    const int have_Q = (strlen(input_Q) > 0);
    const int have_D = (strlen(input_D) > 0);
    const int have_E = (strlen(input_E) > 0);

    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    const char *pers = "test_suite_rsa";

    mbedtls_mpi_init(&N);
    mbedtls_mpi_init(&P);
    mbedtls_mpi_init(&Q);
    mbedtls_mpi_init(&D);
    mbedtls_mpi_init(&E);

    mbedtls_ctr_drbg_init(&ctr_drbg);
    mbedtls_entropy_init(&entropy);
    TEST_ASSERT(mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, (const unsigned char *)pers,
                                      strlen(pers)) == 0);

    if (have_N)
        TEST_ASSERT(mbedtls_mpi_read_string(&N, radix_N, input_N) == 0);

    if (have_P)
        TEST_ASSERT(mbedtls_mpi_read_string(&P, radix_P, input_P) == 0);

    if (have_Q)
        TEST_ASSERT(mbedtls_mpi_read_string(&Q, radix_Q, input_Q) == 0);

    if (have_D)
        TEST_ASSERT(mbedtls_mpi_read_string(&D, radix_D, input_D) == 0);

    if (have_E)
        TEST_ASSERT(mbedtls_mpi_read_string(&E, radix_E, input_E) == 0);

    TEST_ASSERT(mbedtls_rsa_validate_params(
                    have_N ? &N : NULL, have_P ? &P : NULL, have_Q ? &Q : NULL, have_D ? &D : NULL, have_E ? &E : NULL,
                    prng ? mbedtls_ctr_drbg_random : NULL, prng ? &ctr_drbg : NULL) == board_result);

    mbedtls_ctr_drbg_free(&ctr_drbg);
    mbedtls_entropy_free(&entropy);

    mbedtls_mpi_free(&N);
    mbedtls_mpi_free(&P);
    mbedtls_mpi_free(&Q);
    mbedtls_mpi_free(&D);
    mbedtls_mpi_free(&E);

    return EXIT_SUCCESS;
}
/* END_CASE */

/* BEGIN_CASE depends_on:MBEDTLS_CTR_DRBG_C:MBEDTLS_ENTROPY_C */
int32_t board_mbedtls_rsa_export_raw(const data_t *input_N,
                                     const data_t *input_P,
                                     const data_t *input_Q,
                                     const data_t *input_D,
                                     const data_t *input_E,
                                     int32_t is_priv,
                                     int32_t successive)
{
    /* Exported buffers */
    unsigned char bufNe[1000];
    unsigned char bufPe[1000];
    unsigned char bufQe[1000];
    unsigned char bufDe[1000];
    unsigned char bufEe[1000];

    mbedtls_rsa_context ctx;

    mbedtls_rsa_init(&ctx, 0, 0);

    /* Setup RSA context */
    TEST_ASSERT(mbedtls_rsa_import_raw(&ctx, input_N->len ? input_N->x : NULL, input_N->len,
                                       input_P->len ? input_P->x : NULL, input_P->len, input_Q->len ? input_Q->x : NULL,
                                       input_Q->len, input_D->len ? input_D->x : NULL, input_D->len,
                                       input_E->len ? input_E->x : NULL, input_E->len) == 0);

    TEST_ASSERT(mbedtls_rsa_complete(&ctx) == 0);

    /*
     * Export parameters and compare to original ones.
     */

    /* N and E must always be present. */
    if (!successive)
    {
        TEST_ASSERT(mbedtls_rsa_export_raw(&ctx, bufNe, input_N->len, NULL, 0, NULL, 0, NULL, 0, bufEe, input_E->len) ==
                    0);
    }
    else
    {
        TEST_ASSERT(mbedtls_rsa_export_raw(&ctx, bufNe, input_N->len, NULL, 0, NULL, 0, NULL, 0, NULL, 0) == 0);
        TEST_ASSERT(mbedtls_rsa_export_raw(&ctx, NULL, 0, NULL, 0, NULL, 0, NULL, 0, bufEe, input_E->len) == 0);
    }
    TEST_ASSERT(memcmp(input_N->x, bufNe, input_N->len) == 0);
    TEST_ASSERT(memcmp(input_E->x, bufEe, input_E->len) == 0);

    /* If we were providing enough information to setup a complete private context,
     * we expect to be able to export all core parameters. */

    if (is_priv)
    {
        if (!successive)
        {
            TEST_ASSERT(mbedtls_rsa_export_raw(&ctx, NULL, 0, bufPe, input_P->len ? input_P->len : sizeof(bufPe), bufQe,
                                               input_Q->len ? input_Q->len : sizeof(bufQe), bufDe,
                                               input_D->len ? input_D->len : sizeof(bufDe), NULL, 0) == 0);
        }
        else
        {
            TEST_ASSERT(mbedtls_rsa_export_raw(&ctx, NULL, 0, bufPe, input_P->len ? input_P->len : sizeof(bufPe), NULL,
                                               0, NULL, 0, NULL, 0) == 0);

            TEST_ASSERT(mbedtls_rsa_export_raw(&ctx, NULL, 0, NULL, 0, bufQe,
                                               input_Q->len ? input_Q->len : sizeof(bufQe), NULL, 0, NULL, 0) == 0);

            TEST_ASSERT(mbedtls_rsa_export_raw(&ctx, NULL, 0, NULL, 0, NULL, 0, bufDe,
                                               input_D->len ? input_D->len : sizeof(bufDe), NULL, 0) == 0);
        }

        if (input_P->len)
            TEST_ASSERT(memcmp(input_P->x, bufPe, input_P->len) == 0);

        if (input_Q->len)
            TEST_ASSERT(memcmp(input_Q->x, bufQe, input_Q->len) == 0);

        if (input_D->len)
            TEST_ASSERT(memcmp(input_D->x, bufDe, input_D->len) == 0);
    }

    mbedtls_rsa_free(&ctx);

    return EXIT_SUCCESS;
}
/* END_CASE */

/* BEGIN_CASE depends_on:MBEDTLS_CTR_DRBG_C:MBEDTLS_ENTROPY_C:ENTROPY_HAVE_STRONG */
int32_t board_mbedtls_rsa_import_raw(const data_t *input_N,
                                     const data_t *input_P,
                                     const data_t *input_Q,
                                     const data_t *input_D,
                                     const data_t *input_E,
                                     int32_t successive,
                                     int32_t is_priv,
                                     int32_t res_check,
                                     int32_t res_complete)
{
    /* Buffers used for encryption-decryption test */
    unsigned char *buf_orig = NULL;
    unsigned char *buf_enc  = NULL;
    unsigned char *buf_dec  = NULL;

    mbedtls_rsa_context ctx;
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;

    const char *pers = "test_suite_rsa";

    mbedtls_ctr_drbg_init(&ctr_drbg);
    mbedtls_entropy_init(&entropy);
    mbedtls_rsa_init(&ctx, 0, 0);

    TEST_ASSERT(mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, (const unsigned char *)pers,
                                      strlen(pers)) == 0);

    if (!successive)
    {
        TEST_ASSERT(mbedtls_rsa_import_raw(&ctx, (input_N->len > 0) ? input_N->x : NULL, input_N->len,
                                           (input_P->len > 0) ? input_P->x : NULL, input_P->len,
                                           (input_Q->len > 0) ? input_Q->x : NULL, input_Q->len,
                                           (input_D->len > 0) ? input_D->x : NULL, input_D->len,
                                           (input_E->len > 0) ? input_E->x : NULL, input_E->len) == 0);
    }
    else
    {
        /* Import N, P, Q, D, E separately.
         * This should make no functional difference. */

        TEST_ASSERT(mbedtls_rsa_import_raw(&ctx, (input_N->len > 0) ? input_N->x : NULL, input_N->len, NULL, 0, NULL, 0,
                                           NULL, 0, NULL, 0) == 0);

        TEST_ASSERT(mbedtls_rsa_import_raw(&ctx, NULL, 0, (input_P->len > 0) ? input_P->x : NULL, input_P->len, NULL, 0,
                                           NULL, 0, NULL, 0) == 0);

        TEST_ASSERT(mbedtls_rsa_import_raw(&ctx, NULL, 0, NULL, 0, (input_Q->len > 0) ? input_Q->x : NULL, input_Q->len,
                                           NULL, 0, NULL, 0) == 0);

        TEST_ASSERT(mbedtls_rsa_import_raw(&ctx, NULL, 0, NULL, 0, NULL, 0, (input_D->len > 0) ? input_D->x : NULL,
                                           input_D->len, NULL, 0) == 0);

        TEST_ASSERT(mbedtls_rsa_import_raw(&ctx, NULL, 0, NULL, 0, NULL, 0, NULL, 0,
                                           (input_E->len > 0) ? input_E->x : NULL, input_E->len) == 0);
    }

    TEST_ASSERT(mbedtls_rsa_complete(&ctx) == res_complete);

    /* On expected success, perform some public and private
     * key operations to check if the key is working properly. */
    if (res_complete == 0)
    {
        if (is_priv)
            TEST_ASSERT(mbedtls_rsa_check_privkey(&ctx) == res_check);
        else
            TEST_ASSERT(mbedtls_rsa_check_pubkey(&ctx) == res_check);

        if (res_check != 0)
            goto exit;

        buf_orig = mbedtls_calloc(1, mbedtls_rsa_get_len(&ctx));
        buf_enc  = mbedtls_calloc(1, mbedtls_rsa_get_len(&ctx));
        buf_dec  = mbedtls_calloc(1, mbedtls_rsa_get_len(&ctx));
        if (buf_orig == NULL || buf_enc == NULL || buf_dec == NULL)
            goto exit;

        TEST_ASSERT(mbedtls_ctr_drbg_random(&ctr_drbg, buf_orig, mbedtls_rsa_get_len(&ctx)) == 0);

        /* Make sure the number we're generating is smaller than the modulus */
        buf_orig[0] = 0x00;

        TEST_ASSERT(mbedtls_rsa_public(&ctx, buf_orig, buf_enc) == 0);

        if (is_priv)
        {
            TEST_ASSERT(mbedtls_rsa_private(&ctx, mbedtls_ctr_drbg_random, &ctr_drbg, buf_enc, buf_dec) == 0);

            TEST_ASSERT(memcmp(buf_orig, buf_dec, mbedtls_rsa_get_len(&ctx)) == 0);
        }
    }

    mbedtls_free(buf_orig);
    mbedtls_free(buf_enc);
    mbedtls_free(buf_dec);

    mbedtls_rsa_free(&ctx);

    mbedtls_ctr_drbg_free(&ctr_drbg);
    mbedtls_entropy_free(&entropy);

    return EXIT_SUCCESS;

exit:
    return EXIT_FAILURE;
}
/* END_CASE */

int32_t board_mbedtls_rsa_self_test(int32_t verbose)
{
    TEST_ASSERT(mbedtls_rsa_self_test(verbose) == 0);

    return EXIT_SUCCESS;
}
/* END_CASE */
