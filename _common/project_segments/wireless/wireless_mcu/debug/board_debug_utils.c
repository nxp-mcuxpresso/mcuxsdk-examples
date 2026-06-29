/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/************************************************************************************
 * Include
 ************************************************************************************/
#include <stdbool.h>
#include <stdint.h>
#include "board_debug_utils.h"

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

uint32_t BOARD_DBG_Base64Encode(const uint8_t *src, uint32_t src_len, char *dst)
{
    const char b64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    uint32_t in_idx  = 0U;
    uint32_t out_idx = 0U;

    while (in_idx < src_len)
    {
        uint32_t octet_a = (uint32_t)src[in_idx];
        in_idx++;
        uint32_t octet_b = (in_idx < src_len) ? (uint32_t)src[in_idx] : 0U;
        bool     have_b  = (in_idx < src_len);
        in_idx++;
        uint32_t octet_c = (in_idx < src_len) ? (uint32_t)src[in_idx] : 0U;
        bool     have_c  = (in_idx < src_len);
        in_idx++;

        uint32_t triple = (octet_a << 16) | (octet_b << 8) | octet_c;

        dst[out_idx]      = b64_table[(triple >> 18) & 0x3FU];
        dst[out_idx + 1U] = b64_table[(triple >> 12) & 0x3FU];
        dst[out_idx + 2U] = (have_b != false) ? b64_table[(triple >> 6) & 0x3FU] : '=';
        dst[out_idx + 3U] = (have_c != false) ? b64_table[triple & 0x3FU] : '=';
        out_idx += 4U;
    }

    return out_idx;
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
