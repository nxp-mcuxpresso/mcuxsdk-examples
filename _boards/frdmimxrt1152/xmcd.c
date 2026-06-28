/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "xmcd.h"

/* Component ID definition, used by tools. */
#ifndef FSL_COMPONENT_ID
#define FSL_COMPONENT_ID "platform.xmcd"
#endif

#if defined(XIP_BOOT_HEADER_ENABLE) && (XIP_BOOT_HEADER_ENABLE == 1)

#if defined(XIP_BOOT_HEADER_XMCD_ENABLE) && (XIP_BOOT_HEADER_XMCD_ENABLE == 1)

#if defined(__CC_ARM) || defined(__ARMCC_VERSION) || defined(__GNUC__)
__attribute__((section(".boot_hdr.xmcd_data"), used))
#elif defined(__ICCARM__)
#pragma location = ".boot_hdr.xmcd_data"
#endif

const uint32_t xmcd_data[] = {
    0xC002000C, /* FlexSPI instance 2 */
    0xC1000800, /* Option words = 2 */
    0x00000077  /* PINMUX Primary group */
};

#endif /* XIP_BOOT_HEADER_XMCD_ENABLE */
#endif /* XIP_BOOT_HEADER_ENABLE */
