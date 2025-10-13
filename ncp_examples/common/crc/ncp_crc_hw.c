/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * The BSD-3-Clause license can be found at https://spdx.org/licenses/BSD-3-Clause.html
 */
#if CONFIG_CRC32_HW_ACCELERATE
#include "fsl_crc.h"
#include "ncp_crc.h"

void ncp_tlv_chksum_init(void)
{
    crc_config_t crcUserConfig;
    crcUserConfig.seed          = 0U;
    crcUserConfig.polynomial    = kCRC_Polynomial_CRC_32;
    crcUserConfig.reverseIn     = false;
    crcUserConfig.reverseOut    = false;
    crcUserConfig.complementIn  = false;
    crcUserConfig.complementOut = false;
    CRC_Init(CRC, &crcUserConfig);
}

uint32_t ncp_tlv_chksum(uint8_t *buf, uint16_t len)
{
    uint32_t crc;
    uint8_t pad_cnt = 0;

    CRC_WriteSeed(CRC, 0xffffffffU);

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

    CRC_WriteData(CRC, buf, len + pad_cnt);
    crc = CRC_Get32bitResult(CRC);

    return crc;
}

void ncp_tlv_chksum_reset(void)
{
    CRC_Reset(CRC);
}
#endif