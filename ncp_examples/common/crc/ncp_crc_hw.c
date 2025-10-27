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

    CRC_WriteData(CRC, buf, len);

    /*In order to adapt the DCP calculation process of the MCU host RT1060
        in hardware acceleration mode, padding 0 at the end of input*/
    pad_cnt = (4 - (len % 4)) % 4;
    if (pad_cnt > 0) {
        uint8_t zeros[3] = {0};
        CRC_WriteData(CRC, zeros, pad_cnt);
    }

    crc = CRC_Get32bitResult(CRC);

    return crc;
}

void ncp_tlv_chksum_reset(void)
{
    CRC_Reset(CRC);
}
#endif