/*
 * Copyright 2022-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * The BSD-3-Clause license can be found at https://spdx.org/licenses/BSD-3-Clause.html
 */

#ifndef _NCP_CRC_H_
#define _NCP_CRC_H_

#include "fsl_common.h"
#include "fsl_os_abstraction.h"
#if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)
#include "fsl_cache.h"
#endif
#include "ncp_log.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define CHECKSUM_LEN 4

#if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)
#define DCACHE_LINESIZE_BYTE FSL_FEATURE_L1DCACHE_LINESIZE_BYTE
/* This macro computes cache line aligned size for data size */
#define DCACHE_ALIGNED_SIZE(size) \
    ((((size) + FSL_FEATURE_L1DCACHE_LINESIZE_BYTE - 1U) / FSL_FEATURE_L1DCACHE_LINESIZE_BYTE) * FSL_FEATURE_L1DCACHE_LINESIZE_BYTE)
#endif

/*******************************************************************************
 * API
 ******************************************************************************/

void ncp_tlv_chksum_init(void);
uint32_t ncp_tlv_chksum(uint8_t *buf, uint16_t len);
void ncp_tlv_chksum_reset(void);
#endif
