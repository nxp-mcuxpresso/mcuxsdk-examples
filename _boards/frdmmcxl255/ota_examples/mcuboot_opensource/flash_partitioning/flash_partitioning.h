/*
 * Copyright 2025 NXP
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FLASH_PARTITIONING_H_
#define _FLASH_PARTITIONING_H_

#include "mcuboot_config.h"


#define BOOT_FLASH_BASE     0x00000000


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

/*
  MCUBoot region   0x000000 - 0x010000 : 64kB  (0x10000 bytes)
  Primary slot     0x010000 - 0x038000 : 160kB (0x28000 bytes)
  Secondary slot   0x038000 - 0x060000 : 160kB (0x28000 bytes)
*/

#define BOOT_FLASH_ACT_APP  0x00010000
#define BOOT_FLASH_CAND_APP 0x00038000

#ifdef MCUBOOT_SWAP_USING_MOVE

/* For SWAP mode the slot sizes need to be adjusted for presence of trailer
 * with swap status in Primary (ACT) slot. Therefore the size of the primary slot
 * needs to be greater than the secondary slot.
 */

/* 1 extra sector for primary slot to fit MCUboot's swapping algorithm requirements */
#define BOOT_FLASH_ACT_SIZE   0x28000
#define BOOT_FLASH_CAND_SIZE  0x26000

#else

/* other modes (direct-xip, overwrite-only) have equal slot sizes */
#define BOOT_FLASH_ACT_SIZE   0x28000
#define BOOT_FLASH_CAND_SIZE  0x28000

#endif


#endif /* CONFIG_BOOT_CUSTOM_DEVICE_SETUP */

#endif
