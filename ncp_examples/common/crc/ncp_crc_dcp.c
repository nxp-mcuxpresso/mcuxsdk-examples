/*
 * Copyright 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * The BSD-3-Clause license can be found at https://spdx.org/licenses/BSD-3-Clause.html
 */
#if CONFIG_CRC32_HW_ACCELERATE
#include "fsl_dcp.h"
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

    dcp_config_t dcpConfig;
    /* Initialize DCP */
    DCP_GetDefaultConfig(&dcpConfig);
    /* Reset and initialize DCP */
    DCP_Init(DCP, &dcpConfig);
}

uint32_t ncp_tlv_chksum(uint8_t *buf, uint16_t len)
{
    status_t status;
    dcp_handle_t m_handle;
    AT_NONCACHEABLE_SECTION_INIT(static uint8_t output[4]);

    OSA_MutexLock((osa_mutex_handle_t)s_crc_mutex, osaWaitForever_c);

    memset(&m_handle, 0, sizeof(m_handle));
    m_handle.channel    = kDCP_Channel0;
    m_handle.keySlot    = kDCP_KeySlot0;
    m_handle.swapConfig = kDCP_NoSwap;

    size_t outLength = sizeof(output);
    memset(&output, 0, outLength);

#if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U) && defined(DCP_USE_DCACHE) && (DCP_USE_DCACHE == 1U)
    DCACHE_CleanByRange((uint32_t)buf, len);
#endif
    status = DCP_HASH(DCP, &m_handle, kDCP_Crc32, buf, len, output, &outLength);

#if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U) && defined(DCP_USE_DCACHE) && (DCP_USE_DCACHE == 1U)
    DCACHE_InvalidateByRange((uint32_t)output, outLength);
#endif

    assert(status == kStatus_Success);
    assert(outLength == 4u);

    unsigned int result = (output[0] << 24) | (output[1] << 16) | (output[2] << 8) | output[3];
    (void)status;

    OSA_MutexUnlock((osa_mutex_handle_t)s_crc_mutex);

    return result;
}

void ncp_tlv_chksum_reset(void)
{
    OSA_MutexLock((osa_mutex_handle_t)s_crc_mutex, osaWaitForever_c);

    DCP_Deinit(DCP);

    dcp_config_t dcpConfig;
    /* Initialize DCP */
    DCP_GetDefaultConfig(&dcpConfig);
    /* Reset and initialize DCP */
    DCP_Init(DCP, &dcpConfig);

    OSA_MutexUnlock((osa_mutex_handle_t)s_crc_mutex);
}
#endif