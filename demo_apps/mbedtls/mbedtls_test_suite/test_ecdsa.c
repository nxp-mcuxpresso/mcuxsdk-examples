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

#include "mbedtls/ecdsa.h"

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

int32_t board_ecdsa_prim_random(int32_t id)
{
    mbedtls_ecp_group grp;
    mbedtls_ecp_point Q;
    mbedtls_mpi d, r, s;
    rnd_pseudo_info rnd_info;
    unsigned char buf[66];

    mbedtls_ecp_group_init(&grp);
    mbedtls_ecp_point_init(&Q);
    mbedtls_mpi_init(&d);
    mbedtls_mpi_init(&r);
    mbedtls_mpi_init(&s);
    memset(&rnd_info, 0x00, sizeof(rnd_pseudo_info));
    memset(buf, 0, sizeof(buf));

    /* prepare material for signature */
    TEST_ASSERT(rnd_pseudo_rand(&rnd_info, buf, sizeof(buf)) == 0);
    TEST_ASSERT(mbedtls_ecp_group_load(&grp, id) == 0);
    TEST_ASSERT(mbedtls_ecp_gen_keypair(&grp, &d, &Q, &rnd_pseudo_rand, &rnd_info) == 0);

    TEST_ASSERT(mbedtls_ecdsa_sign(&grp, &r, &s, &d, buf, sizeof(buf), &rnd_pseudo_rand, &rnd_info) == 0);
    TEST_ASSERT(mbedtls_ecdsa_verify(&grp, buf, sizeof(buf), &Q, &r, &s) == 0);

    mbedtls_ecp_group_free(&grp);
    mbedtls_ecp_point_free(&Q);
    mbedtls_mpi_free(&d);
    mbedtls_mpi_free(&r);
    mbedtls_mpi_free(&s);

    return EXIT_SUCCESS;
}

int32_t board_ecdsa_prim_test_vectors(int32_t id,
                                      const char *d_str,
                                      const char *xQ_str,
                                      const char *yQ_str,
                                      const data_t *rnd_buf,
                                      const data_t *hash,
                                      const char *r_str,
                                      const char *s_str,
                                      int32_t board_result)
{
    mbedtls_ecp_group grp;
    mbedtls_ecp_point Q;
    mbedtls_mpi d, r, s, r_check, s_check;
    rnd_buf_info rnd_info;

    mbedtls_ecp_group_init(&grp);
    mbedtls_ecp_point_init(&Q);
    mbedtls_mpi_init(&d);
    mbedtls_mpi_init(&r);
    mbedtls_mpi_init(&s);
    mbedtls_mpi_init(&r_check);
    mbedtls_mpi_init(&s_check);

    TEST_ASSERT(mbedtls_ecp_group_load(&grp, id) == 0);
    TEST_ASSERT(mbedtls_ecp_point_read_string(&Q, 16, xQ_str, yQ_str) == 0);
    TEST_ASSERT(mbedtls_mpi_read_string(&d, 16, d_str) == 0);
    TEST_ASSERT(mbedtls_mpi_read_string(&r_check, 16, r_str) == 0);
    TEST_ASSERT(mbedtls_mpi_read_string(&s_check, 16, s_str) == 0);
    rnd_info.buf    = rnd_buf->x;
    rnd_info.length = rnd_buf->len;

    /* Fix rnd_buf->x by shifting it left if necessary */
    if (grp.nbits % 8 != 0)
    {
        unsigned char shift = 8 - (grp.nbits % 8);
        size_t i;

        for (i = 0; i < rnd_info.length - 1; i++)
            rnd_buf->x[i] = rnd_buf->x[i] << shift | rnd_buf->x[i + 1] >> (8 - shift);

        rnd_buf->x[rnd_info.length - 1] <<= shift;
    }

    TEST_ASSERT(mbedtls_ecdsa_sign(&grp, &r, &s, &d, hash->x, hash->len, rnd_buffer_rand, &rnd_info) == board_result);

    if (board_result == 0)
    {
        TEST_ASSERT(mbedtls_mpi_cmp_mpi(&r, &r_check) == 0);
        TEST_ASSERT(mbedtls_mpi_cmp_mpi(&s, &s_check) == 0);

        TEST_ASSERT(mbedtls_ecdsa_verify(&grp, hash->x, hash->len, &Q, &r_check, &s_check) == 0);
    }

    mbedtls_ecp_group_free(&grp);
    mbedtls_ecp_point_free(&Q);
    mbedtls_mpi_free(&d);
    mbedtls_mpi_free(&r);
    mbedtls_mpi_free(&s);
    mbedtls_mpi_free(&r_check);
    mbedtls_mpi_free(&s_check);

    return EXIT_SUCCESS;
}

int32_t board_ecdsa_det_test_vectors(
    int32_t id, const char *d_str, int32_t md_alg, const char *msg, const char *r_str, const char *s_str)
{
    mbedtls_ecp_group grp;
    mbedtls_mpi d, r, s, r_check, s_check;
    unsigned char hash[MBEDTLS_MD_MAX_SIZE];
    size_t hlen;
    const mbedtls_md_info_t *md_info;

    mbedtls_ecp_group_init(&grp);
    mbedtls_mpi_init(&d);
    mbedtls_mpi_init(&r);
    mbedtls_mpi_init(&s);
    mbedtls_mpi_init(&r_check);
    mbedtls_mpi_init(&s_check);
    memset(hash, 0, sizeof(hash));

    TEST_ASSERT(mbedtls_ecp_group_load(&grp, id) == 0);
    TEST_ASSERT(mbedtls_mpi_read_string(&d, 16, d_str) == 0);
    TEST_ASSERT(mbedtls_mpi_read_string(&r_check, 16, r_str) == 0);
    TEST_ASSERT(mbedtls_mpi_read_string(&s_check, 16, s_str) == 0);

    md_info = mbedtls_md_info_from_type(md_alg);
    TEST_ASSERT(md_info != NULL);
    hlen = mbedtls_md_get_size(md_info);
    mbedtls_md(md_info, (const unsigned char *)msg, strlen(msg), hash);

    TEST_ASSERT(mbedtls_ecdsa_sign_det(&grp, &r, &s, &d, hash, hlen, md_alg) == 0);

    TEST_ASSERT(mbedtls_mpi_cmp_mpi(&r, &r_check) == 0);
    TEST_ASSERT(mbedtls_mpi_cmp_mpi(&s, &s_check) == 0);

    mbedtls_ecp_group_free(&grp);
    mbedtls_mpi_free(&d);
    mbedtls_mpi_free(&r);
    mbedtls_mpi_free(&s);
    mbedtls_mpi_free(&r_check);
    mbedtls_mpi_free(&s_check);

    return EXIT_SUCCESS;
}

int32_t board_ecdsa_write_read_random(int32_t id)
{
    mbedtls_ecdsa_context ctx;
    rnd_pseudo_info rnd_info;
    unsigned char hash[32];
    unsigned char sig[200];
    size_t sig_len, i;

    mbedtls_ecdsa_init(&ctx);
    memset(&rnd_info, 0x00, sizeof(rnd_pseudo_info));
    memset(hash, 0, sizeof(hash));
    memset(sig, 0x2a, sizeof(sig));

    /* prepare material for signature */
    TEST_ASSERT(rnd_pseudo_rand(&rnd_info, hash, sizeof(hash)) == 0);

    /* generate signing key */
    TEST_ASSERT(mbedtls_ecdsa_genkey(&ctx, id, &rnd_pseudo_rand, &rnd_info) == 0);

    /* generate and write signature, then read and verify it */
    TEST_ASSERT(mbedtls_ecdsa_write_signature(&ctx, MBEDTLS_MD_SHA256, hash, sizeof(hash), sig, &sig_len,
                                              &rnd_pseudo_rand, &rnd_info) == 0);
    TEST_ASSERT(mbedtls_ecdsa_read_signature(&ctx, hash, sizeof(hash), sig, sig_len) == 0);

    /* check we didn't write past the announced length */
    for (i = sig_len; i < sizeof(sig); i++)
        TEST_ASSERT(sig[i] == 0x2a);

    /* try verification with invalid length */
    TEST_ASSERT(mbedtls_ecdsa_read_signature(&ctx, hash, sizeof(hash), sig, sig_len - 1) != 0);
    TEST_ASSERT(mbedtls_ecdsa_read_signature(&ctx, hash, sizeof(hash), sig, sig_len + 1) != 0);

    /* try invalid sequence tag */
    sig[0]++;
    TEST_ASSERT(mbedtls_ecdsa_read_signature(&ctx, hash, sizeof(hash), sig, sig_len) != 0);
    sig[0]--;

    /* try modifying r */
    sig[10]++;
    TEST_ASSERT(mbedtls_ecdsa_read_signature(&ctx, hash, sizeof(hash), sig, sig_len) != 0);
    sig[10]--;

    /* try modifying s */
    sig[sig_len - 1]++;
    TEST_ASSERT(mbedtls_ecdsa_read_signature(&ctx, hash, sizeof(hash), sig, sig_len) != 0);
    sig[sig_len - 1]--;

    mbedtls_ecdsa_free(&ctx);

    return EXIT_SUCCESS;
}
