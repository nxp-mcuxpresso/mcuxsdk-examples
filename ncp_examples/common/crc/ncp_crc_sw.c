/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * The BSD-3-Clause license can be found at https://spdx.org/licenses/BSD-3-Clause.html
 */
#if !CONFIG_CRC32_HW_ACCELERATE
#include "ncp_crc.h"

#define CRC32_POLY   0x04c11db7

static uint32_t crc32_table[256] = {0,};
void ncp_tlv_chksum_init(void)
{
    int i, j;
    unsigned int c;
    for (i = 0; i < 256; ++i)
    {
        for (c = i << 24, j = 8; j > 0; --j)
            c = c & 0x80000000 ? (c << 1) ^ CRC32_POLY : (c << 1);
        crc32_table[i] = c;
    }
}

uint32_t ncp_tlv_chksum(uint8_t *buf, uint16_t len)
{
    uint8_t *p;
    unsigned int crc;
    crc = 0xffffffff;
    uint8_t pad_cnt = 0;

    /*In order to adapt the DCP calculation process of the MCU host RT1060
     in hardware acceleration mode, padding 0 at the end of input*/
    for(pad_cnt = 0; pad_cnt < 4; pad_cnt++)
    {
        if((len + pad_cnt) % 4 != 0)
        {
            buf[len + pad_cnt] = 0;
        }
        else
        {
            break;
        }
    }

    for (p = buf; (len + pad_cnt) > 0; ++p, --(len + pad_cnt))
        crc = (crc << 8) ^ (crc32_table[(crc >> 24) ^ *p]);
    return crc;
}

void ncp_tlv_chksum_reset(void)
{
    return;
}
#endif