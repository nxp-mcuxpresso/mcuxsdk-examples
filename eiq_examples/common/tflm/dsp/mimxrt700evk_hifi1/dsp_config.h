/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _DSP_CONFIG_H_
#define _DSP_CONFIG_H_

/* Address of RAM, where the image for DSP should be copied */
/* These addresses are accessed by the ARM core and aliased to M33 code memory */
#define DSP_BOOT_ADDRESS (uint32_t *)0x580000
#define DSP_TEXT_ADDRESS (uint32_t *)0x680000
#define DSP_DATA_ADDRESS (uint32_t *)0x20700000

#endif
