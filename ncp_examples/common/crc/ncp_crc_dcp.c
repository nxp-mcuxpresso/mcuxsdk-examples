/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * The BSD-3-Clause license can be found at https://spdx.org/licenses/BSD-3-Clause.html
 */
#if CONFIG_CRC32_HW_ACCELERATE
#include "fsl_dcp.h"
#include "ncp_crc.h"

void ncp_tlv_chksum_init(void)
{
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
    m_handle.channel    = kDCP_Channel0;
    m_handle.keySlot    = kDCP_KeySlot0;
    m_handle.swapConfig = kDCP_NoSwap;

    size_t outLength = sizeof(output);
    memset(&output, 0, outLength);

    status = DCP_HASH(DCP, &m_handle, kDCP_Crc32, buf, len, output, &outLength);
    assert(status == kStatus_Success);
    assert(outLength == 4u);

    unsigned int result = (output[0] << 24) | (output[1] << 16) | (output[2] << 8) | output[3];

    return result;
}

void ncp_tlv_chksum_reset(void)
{
    ncp_tlv_chksum_init();
}
#endif