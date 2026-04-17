/*
 * Copyright 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * The BSD-3-Clause license can be found at https://spdx.org/licenses/BSD-3-Clause.html
 */
#if CONFIG_CRC32_HW_ACCELERATE
#include "fsl_crc.h"
#include "ncp_crc.h"

NCP_LOG_MODULE_REGISTER(ncp_crc, CONFIG_LOG_NCP_CRC_LEVEL);

OSA_MUTEX_HANDLE_DEFINE(s_crc_mutex);
static bool s_crc_mutex_created = false;

void ncp_tlv_chksum_init(void)
{
    if (!s_crc_mutex_created)
    {
        if (OSA_MutexCreate((osa_mutex_handle_t)s_crc_mutex) != KOSA_StatusSuccess)
        {
            NCP_LOG_ERR("Failed to create CRC mutex!");
            return;
        }
        s_crc_mutex_created = true;
    }

    crc_config_t crcUserConfig;
    crcUserConfig.seed          = 0xffffffffU;
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

    OSA_MutexLock((osa_mutex_handle_t)s_crc_mutex, osaWaitForever_c);

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

    OSA_MutexUnlock((osa_mutex_handle_t)s_crc_mutex);

    return crc;
}

void ncp_tlv_chksum_reset(void)
{
    OSA_MutexLock((osa_mutex_handle_t)s_crc_mutex, osaWaitForever_c);

    crc_config_t crcUserConfig;
    crcUserConfig.seed          = 0xffffffffU;
    crcUserConfig.polynomial    = kCRC_Polynomial_CRC_32;
    crcUserConfig.reverseIn     = false;
    crcUserConfig.reverseOut    = false;
    crcUserConfig.complementIn  = false;
    crcUserConfig.complementOut = false;
    CRC_Init(CRC, &crcUserConfig);

    OSA_MutexUnlock((osa_mutex_handle_t)s_crc_mutex);
}
#endif