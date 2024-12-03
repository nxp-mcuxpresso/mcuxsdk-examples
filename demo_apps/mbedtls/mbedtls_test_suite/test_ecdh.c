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

#include "mbedtls/ecdh.h"

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

int32_t board_ecdh_primitive_random(int32_t id)
{
    mbedtls_ecp_group grp;
    mbedtls_ecp_point qA, qB;
    mbedtls_mpi dA, dB, zA, zB;
    rnd_pseudo_info rnd_info;

    mbedtls_ecp_group_init(&grp);
    mbedtls_ecp_point_init(&qA);
    mbedtls_ecp_point_init(&qB);
    mbedtls_mpi_init(&dA);
    mbedtls_mpi_init(&dB);
    mbedtls_mpi_init(&zA);
    mbedtls_mpi_init(&zB);
    memset(&rnd_info, 0x00, sizeof(rnd_pseudo_info));

    TEST_ASSERT(mbedtls_ecp_group_load(&grp, id) == 0);

    TEST_ASSERT(mbedtls_ecdh_gen_public(&grp, &dA, &qA, &rnd_pseudo_rand, &rnd_info) == 0);
    TEST_ASSERT(mbedtls_ecdh_gen_public(&grp, &dB, &qB, &rnd_pseudo_rand, &rnd_info) == 0);
    TEST_ASSERT(mbedtls_ecdh_compute_shared(&grp, &zA, &qB, &dA, &rnd_pseudo_rand, &rnd_info) == 0);
    TEST_ASSERT(mbedtls_ecdh_compute_shared(&grp, &zB, &qA, &dB, NULL, NULL) == 0);

    TEST_ASSERT(mbedtls_mpi_cmp_mpi(&zA, &zB) == 0);

    mbedtls_ecp_group_free(&grp);
    mbedtls_ecp_point_free(&qA);
    mbedtls_ecp_point_free(&qB);
    mbedtls_mpi_free(&dA);
    mbedtls_mpi_free(&dB);
    mbedtls_mpi_free(&zA);
    mbedtls_mpi_free(&zB);

    return EXIT_SUCCESS;
}

int32_t board_ecdh_primitive_testvec(int32_t id,
                                     const data_t *rnd_buf_A,
                                     const char *xA_str,
                                     const char *yA_str,
                                     const data_t *rnd_buf_B,
                                     const char *xB_str,
                                     const char *yB_str,
                                     const char *z_str)
{
    mbedtls_ecp_group grp;
    mbedtls_ecp_point qA, qB;
    mbedtls_mpi dA, dB, zA, zB, check;
    rnd_buf_info rnd_info_A, rnd_info_B;

    mbedtls_ecp_group_init(&grp);
    mbedtls_ecp_point_init(&qA);
    mbedtls_ecp_point_init(&qB);
    mbedtls_mpi_init(&dA);
    mbedtls_mpi_init(&dB);
    mbedtls_mpi_init(&zA);
    mbedtls_mpi_init(&zB);
    mbedtls_mpi_init(&check);

    TEST_ASSERT(mbedtls_ecp_group_load(&grp, id) == 0);

    rnd_info_A.buf    = rnd_buf_A->x;
    rnd_info_A.length = rnd_buf_A->len;

    /* Fix rnd_buf_A->x by shifting it left if necessary */
    if (grp.nbits % 8 != 0)
    {
        unsigned char shift = 8 - (grp.nbits % 8);
        size_t i;

        for (i = 0; i < rnd_info_A.length - 1; i++)
            rnd_buf_A->x[i] = rnd_buf_A->x[i] << shift | rnd_buf_A->x[i + 1] >> (8 - shift);

        rnd_buf_A->x[rnd_info_A.length - 1] <<= shift;
    }

    rnd_info_B.buf    = rnd_buf_B->x;
    rnd_info_B.length = rnd_buf_B->len;

    /* Fix rnd_buf_B->x by shifting it left if necessary */
    if (grp.nbits % 8 != 0)
    {
        unsigned char shift = 8 - (grp.nbits % 8);
        size_t i;

        for (i = 0; i < rnd_info_B.length - 1; i++)
            rnd_buf_B->x[i] = rnd_buf_B->x[i] << shift | rnd_buf_B->x[i + 1] >> (8 - shift);

        rnd_buf_B->x[rnd_info_B.length - 1] <<= shift;
    }

    TEST_ASSERT(mbedtls_ecdh_gen_public(&grp, &dA, &qA, rnd_buffer_rand, &rnd_info_A) == 0);
    TEST_ASSERT(!mbedtls_ecp_is_zero(&qA));
    TEST_ASSERT(mbedtls_mpi_read_string(&check, 16, xA_str) == 0);
    TEST_ASSERT(mbedtls_mpi_cmp_mpi(&qA.X, &check) == 0);
    TEST_ASSERT(mbedtls_mpi_read_string(&check, 16, yA_str) == 0);
    TEST_ASSERT(mbedtls_mpi_cmp_mpi(&qA.Y, &check) == 0);

    TEST_ASSERT(mbedtls_ecdh_gen_public(&grp, &dB, &qB, rnd_buffer_rand, &rnd_info_B) == 0);
    TEST_ASSERT(!mbedtls_ecp_is_zero(&qB));
    TEST_ASSERT(mbedtls_mpi_read_string(&check, 16, xB_str) == 0);
    TEST_ASSERT(mbedtls_mpi_cmp_mpi(&qB.X, &check) == 0);
    TEST_ASSERT(mbedtls_mpi_read_string(&check, 16, yB_str) == 0);
    TEST_ASSERT(mbedtls_mpi_cmp_mpi(&qB.Y, &check) == 0);

    TEST_ASSERT(mbedtls_mpi_read_string(&check, 16, z_str) == 0);
    TEST_ASSERT(mbedtls_ecdh_compute_shared(&grp, &zA, &qB, &dA, NULL, NULL) == 0);
    TEST_ASSERT(mbedtls_mpi_cmp_mpi(&zA, &check) == 0);
    TEST_ASSERT(mbedtls_ecdh_compute_shared(&grp, &zB, &qA, &dB, NULL, NULL) == 0);
    TEST_ASSERT(mbedtls_mpi_cmp_mpi(&zB, &check) == 0);

    mbedtls_ecp_group_free(&grp);
    mbedtls_ecp_point_free(&qA);
    mbedtls_ecp_point_free(&qB);
    mbedtls_mpi_free(&dA);
    mbedtls_mpi_free(&dB);
    mbedtls_mpi_free(&zA);
    mbedtls_mpi_free(&zB);
    mbedtls_mpi_free(&check);

    return EXIT_SUCCESS;
}

int32_t board_ecdh_exchange(int32_t id)
{
    mbedtls_ecdh_context srv, cli;
    unsigned char buf[1000];
    const unsigned char *vbuf;
    size_t len;
    rnd_pseudo_info rnd_info;

    mbedtls_ecdh_init(&srv);
    mbedtls_ecdh_init(&cli);
    memset(&rnd_info, 0x00, sizeof(rnd_pseudo_info));

    TEST_ASSERT(mbedtls_ecp_group_load(&srv.grp, id) == 0);

    memset(buf, 0x00, sizeof(buf));
    vbuf = buf;
    TEST_ASSERT(mbedtls_ecdh_make_params(&srv, &len, buf, 1000, &rnd_pseudo_rand, &rnd_info) == 0);
    TEST_ASSERT(mbedtls_ecdh_read_params(&cli, &vbuf, buf + len) == 0);

    memset(buf, 0x00, sizeof(buf));
    TEST_ASSERT(mbedtls_ecdh_make_public(&cli, &len, buf, 1000, &rnd_pseudo_rand, &rnd_info) == 0);
    TEST_ASSERT(mbedtls_ecdh_read_public(&srv, buf, len) == 0);

    TEST_ASSERT(mbedtls_ecdh_calc_secret(&srv, &len, buf, 1000, &rnd_pseudo_rand, &rnd_info) == 0);
    TEST_ASSERT(mbedtls_ecdh_calc_secret(&cli, &len, buf, 1000, NULL, NULL) == 0);
    TEST_ASSERT(mbedtls_mpi_cmp_mpi(&srv.z, &cli.z) == 0);

    mbedtls_ecdh_free(&srv);
    mbedtls_ecdh_free(&cli);

    return EXIT_SUCCESS;
}
