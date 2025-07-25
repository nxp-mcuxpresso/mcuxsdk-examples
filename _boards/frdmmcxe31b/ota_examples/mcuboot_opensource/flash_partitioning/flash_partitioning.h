/*
 * Copyright 2025 NXP
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FLASH_PARTITIONING_H_
#define _FLASH_PARTITIONING_H_

#include "mcuboot_config.h"

#define BOOT_FLASH_BASE     0x00400000


#if defined(CONFIG_BOOT_CUSTOM_DEVICE_SETUP)
/* Layout setup from Kconfig */

#define BOOT_FLASH_ACT_APP              CONFIG_BOOT_FLASH_ACT_APP_ADDRESS
#define BOOT_FLASH_CAND_APP             CONFIG_BOOT_FLASH_CAND_APP_ADDRESS

/* TODO - add to Kconfig
#define BOOT_FLASH_ACT_SIZE             CONFIG_BOOT_FLASH_ACT_SIZE
#define BOOT_FLASH_CAND_SIZE            CONFIG_BOOT_FLASH_CAND_SIZE
*/

#else
/* Default layout setup */

/* The layout is designed according to MCXE31's flash capabilities.
 * It has 4x 1MB blocks where first 768kB within a block is protected
 * using a single super-sector.

  MCUBoot region    0x0040_1400 - 0x004C_0000  : 763kB
  Primary slot      0x0050_0000 - 0x005C_0000  : 768kB (0xC0000 bytes)
  Secondary slot    0x0060_0000 - 0x006C_0000  : 768kB (0xC0000 bytes)
*/

#define BOOT_FLASH_ACT_APP  0x00500000
#define BOOT_FLASH_CAND_APP 0x00600000

#ifdef MCUBOOT_SWAP_USING_MOVE

/* For SWAP mode the slot sizes need to be adjusted for presence of trailer
 * with swap status in Primary (ACT) slot. Therefore the size of the primary slot
 * needs to be greater than the secondary slot.
 */

/* 1 extra sector for primary slot to fit MCUboot's swapping algorithm requirements */
#define BOOT_FLASH_ACT_SIZE   0xC0000
#define BOOT_FLASH_CAND_SIZE  0xBE000

#else

/* other modes (direct-xip, overwrite-only) have equal slot sizes */
#define BOOT_FLASH_ACT_SIZE   0xC0000
#define BOOT_FLASH_CAND_SIZE  0xC0000

#endif


#endif /* CONFIG_BOOT_CUSTOM_DEVICE_SETUP */

#endif
