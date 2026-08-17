/*
 * Copyright 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __MIMXRT2660EVK_USDHC_CONFIG__
#define __MIMXRT2660EVK_USDHC_CONFIG__

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    uint32_t pad0                [0x200U / 4U];
    uint32_t secondary_img_table [0x014U / 4U];
    uint32_t pad1                [0x1ECU / 4U];
    uint32_t xmcd_data           [0x300U / 4U];
    uint32_t pad2                [0x100U / 4U];
} usdhc_boot_mcb_t;

#endif /* __MIMXRT2660EVK_USDHC_CONFIG__ */
