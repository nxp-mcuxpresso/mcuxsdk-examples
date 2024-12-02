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

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "erpc_server_setup.h"
#include "erpc_transport_setup.h"
#include "erpc_error_handler.h"
#include "mbedtls_test_suite_server.h"
#include "mbedtls_test_suite.h"

#include <string.h>

#if BOARD_DEBUG_UART_TYPE == DEBUG_CONSOLE_DEVICE_TYPE_UART
#include "fsl_uart_cmsis.h"
#elif BOARD_DEBUG_UART_TYPE == DEBUG_CONSOLE_DEVICE_TYPE_LPUART
#include "fsl_lpuart_cmsis.h"
#elif BOARD_DEBUG_UART_TYPE == DEBUG_CONSOLE_DEVICE_TYPE_LPSCI
#include "fsl_lpsci_cmsis.h"
#endif

#if defined(MBEDTLS_MEMORY_BUFFER_ALLOC_C)
#include "mbedtls/memory_buffer_alloc.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
typedef struct
{
    unsigned int key[16];
    unsigned int v0, v1;
} rnd_pseudo_info;

typedef struct
{
    unsigned char *buf;
    size_t length;
} rnd_buf_info;
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/* Helper function */

int hexcmp(uint8_t *a, uint8_t *b, uint32_t a_len, uint32_t b_len)
{
    int ret    = 0;
    uint32_t i = 0;

    if (a_len != b_len)
        return (-1);

    for (i = 0; i < a_len; i++)
    {
        if (a[i] != b[i])
        {
            ret = -1;
            break;
        }
    }
    return ret;
}

int unhexify(unsigned char *obuf, const char *ibuf)
{
    unsigned char c, c2;
    int len = strlen(ibuf) / 2;
    assert(strlen(ibuf) % 2 == 0); /* must be even number of bytes */

    while (*ibuf != 0)
    {
        c = *ibuf++;
        if (c >= '0' && c <= '9')
            c -= '0';
        else if (c >= 'a' && c <= 'f')
            c -= 'a' - 10;
        else if (c >= 'A' && c <= 'F')
            c -= 'A' - 10;
        else
            assert(0);

        c2 = *ibuf++;
        if (c2 >= '0' && c2 <= '9')
            c2 -= '0';
        else if (c2 >= 'a' && c2 <= 'f')
            c2 -= 'a' - 10;
        else if (c2 >= 'A' && c2 <= 'F')
            c2 -= 'A' - 10;
        else
            assert(0);

        *obuf++ = (c << 4) | c2;
    }

    return len;
}

unsigned char *unhexify_alloc(const char *ibuf, size_t *olen)
{
    unsigned char *obuf;

    *olen = strlen(ibuf) / 2;

    if (*olen == 0)
        return (zero_alloc(*olen));

    obuf = (unsigned char *)mbedtls_calloc(1, *olen);
    assert(obuf != NULL);

    (void)unhexify(obuf, ibuf);

    return ((unsigned char *)obuf);
}

unsigned char *zero_alloc(size_t len)
{
    void *p;
    size_t actual_len = (len != 0) ? len : 1;

    p = mbedtls_calloc(1, actual_len);
    assert(p != NULL);

    memset(p, 0x00, actual_len);

    return (unsigned char *)(p);
}

void hexify(unsigned char *obuf, const unsigned char *ibuf, int len)
{
    unsigned char l, h;

    while (len != 0)
    {
        h = *ibuf / 16;
        l = *ibuf % 16;

        if (h < 10)
            *obuf++ = '0' + h;
        else
            *obuf++ = 'a' + h - 10;

        if (l < 10)
            *obuf++ = '0' + l;
        else
            *obuf++ = 'a' + l - 10;

        ++ibuf;
        len--;
    }
}

int rnd_pseudo_rand(void *rng_state, unsigned char *output, size_t len)
{
    rnd_pseudo_info *info = (rnd_pseudo_info *)rng_state;
    uint32_t i, *k, sum, delta = 0x9E3779B9;
    unsigned char result[4], *out = output;

    if (rng_state == NULL)
        return (rnd_std_rand(NULL, output, len));

    k = info->key;

    while (len > 0)
    {
        size_t use_len = (len > 4) ? 4 : len;
        sum            = 0;

        for (i = 0; i < 32; i++)
        {
            info->v0 += (((info->v1 << 4) ^ (info->v1 >> 5)) + info->v1) ^ (sum + k[sum & 3]);
            sum += delta;
            info->v1 += (((info->v0 << 4) ^ (info->v0 >> 5)) + info->v0) ^ (sum + k[(sum >> 11) & 3]);
        }

        PUT_UINT32_BE(info->v0, result, 0);
        memcpy(out, result, use_len);
        len -= use_len;
        out += 4;
    }

    return (0);
}

int rnd_std_rand(void *rng_state, unsigned char *output, size_t len)
{
#if !defined(__OpenBSD__)
    size_t i;

    if (rng_state != NULL)
        rng_state = NULL;

    for (i = 0; i < len; ++i)
        output[i] = rand();
#else
    if (rng_state != NULL)
        rng_state = NULL;

    arc4random_buf(output, len);
#endif /* !OpenBSD */

    return (0);
}

int rnd_zero_rand(void *rng_state, unsigned char *output, size_t len)
{
    if (rng_state != NULL)
        rng_state = NULL;

    memset(output, 0, len);

    return (0);
}

int rnd_buffer_rand(void *rng_state, unsigned char *output, size_t len)
{
    rnd_buf_info *info = (rnd_buf_info *)rng_state;
    size_t use_len;

    if (rng_state == NULL)
        return (rnd_std_rand(NULL, output, len));

    use_len = len;
    if (len > info->length)
        use_len = info->length;

    if (use_len)
    {
        memcpy(output, info->buf, use_len);
        info->buf += use_len;
        info->length -= use_len;
    }

    if (len - use_len > 0)
        return (rnd_std_rand(NULL, output + use_len, len - use_len));

    return (0);
}

/* eRPC init */
erpc_transport_t transport = {0};

void erpc_init(void)
{
    transport = erpc_transport_cmsis_uart_init((void *)&ERPC_DEMO_UART);

    /* MessageBufferFactory initialization */
    erpc_mbf_t message_buffer_factory;
    message_buffer_factory = erpc_mbf_dynamic_init();

    erpc_server_init(transport, message_buffer_factory);
}

void board_get_name(char *name)
{
    strcpy(name, BOARD_NAME);
}

int main(int argc, char *argv[])
{
    /*board init*/
    BOARD_InitHardware();

    {
        /* eRPC server side initialization */
        erpc_init();

        /* adding the service to the server */
        erpc_add_service_to_server(create_test_suite_service());
        while (1)
        {
            /* process message */
            erpc_status_t status = erpc_server_poll();

            /* handle error status */
            if (status != kErpcStatus_Success)
            {
                /* print error description */

                /* stop erpc server */
                erpc_server_stop();

                /* exit program loop */
                break;
            }
        }
        while (1)
        {
            char ch = GETCHAR();
            PUTCHAR(ch);
        }
    }
}
