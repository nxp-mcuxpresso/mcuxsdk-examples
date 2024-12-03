/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
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
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${header:start}*/
#if defined(MBEDTLS_PLATFORM_C)
#if defined(FREESCALE_KSDK_BM)
#include "board.h"
#include "app.h"
#include "fsl_debug_console.h"
#include "ksdk_mbedtls.h"
#include "mbedtls/version.h"
#include "mbedtls/platform.h"

#else

#endif
#else
#include <stdio.h>
#include <stdlib.h>
#define mbedtls_printf       printf
#define mbedtls_snprintf     snprintf
#define mbedtls_exit         exit
#define MBEDTLS_EXIT_SUCCESS EXIT_SUCCESS
#define MBEDTLS_EXIT_FAILURE EXIT_FAILURE
#endif

#include "fsl_usart_cmsis.h"

#define ERPC_DEFAULT_BUFFER_SIZE (8192)

#define CORE_CLK_FREQ  CLOCK_GetFreq(kCLOCK_CoreSysClk)
#define ERPC_DEMO_UART Driver_USART0

#define TEST_ASSERT(TEST)        \
    do                           \
    {                            \
        if (!(TEST))             \
        {                        \
            return EXIT_FAILURE; \
        }                        \
    } while (0)

#ifndef PUT_UINT32_BE
#define PUT_UINT32_BE(n, b, i)                     \
    {                                              \
        (b)[(i)]     = (unsigned char)((n) >> 24); \
        (b)[(i) + 1] = (unsigned char)((n) >> 16); \
        (b)[(i) + 2] = (unsigned char)((n) >> 8);  \
        (b)[(i) + 3] = (unsigned char)((n));       \
    }
#endif

/*${header:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
int hexcmp(uint8_t *a, uint8_t *b, uint32_t a_len, uint32_t b_len);
int unhexify(unsigned char *obuf, const char *ibuf);
void hexify(unsigned char *obuf, const unsigned char *ibuf, int len);
int rnd_pseudo_rand(void *rng_state, unsigned char *output, size_t len);
int rnd_std_rand(void *rng_state, unsigned char *output, size_t len);
int rnd_buffer_rand(void *rng_state, unsigned char *output, size_t len);
unsigned char *unhexify_alloc(const char *ibuf, size_t *olen);
unsigned char *zero_alloc(size_t len);
int rnd_zero_rand(void *rng_state, unsigned char *output, size_t len);
/*${prototype:end}*/

#endif /* _APP_H_ */
