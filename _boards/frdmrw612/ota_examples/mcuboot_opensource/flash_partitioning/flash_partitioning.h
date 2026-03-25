/*
 * Copyright 2026 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _FLASH_PARTITIONING_H_
#define _FLASH_PARTITIONING_H_
#include "sblconfig.h"

#define BOOT_FLASH_BASE     0x08000000

#if defined(CONFIG_BOOT_CUSTOM_DEVICE_SETUP)
/* Layout setup from Kconfig */

#define BOOT_FLASH_ACT_APP                CONFIG_BOOT_FLASH_ACT_APP_ADDRESS
#define BOOT_FLASH_CAND_APP               CONFIG_BOOT_FLASH_CAND_APP_ADDRESS

#if CONFIG_BOOT_MODE_ENCRYPTED_XIP_OVERWRITE || CONFIG_BOOT_MODE_ENCRYPTED_XIP_REMAP
#define BOOT_FLASH_SLOT0_ENC_CFG_ADDRESS  CONFIG_BOOT_FLASH_SLOT0_ENC_CFG_ADDRESS
#define BOOT_FLASH_SLOT1_ENC_CFG_ADDRESS  CONFIG_BOOT_FLASH_SLOT1_ENC_CFG_ADDRESS
#endif

#else
/* Default layout setup

The memory is allocated as follows:
    - BOOTLOADER:      0x040000 bytes @ 0x08000000 - MCUboot
    - APP_ACT:         0x200000 bytes @ 0x08040000 - primary slot
    - APP_CAND:        0x200000 bytes @ 0x08240000 - secondary slot
    Encrypted XIP support:
    - ENC_META_SLOT_0: 0x001000 bytes @ 0x08440000 - slot 0 encryption config block
    - ENC_META_SLOT_1: 0x001000 bytes @ 0x08441000 - slot 1 encryption config block
*/

#define BOOT_FLASH_ACT_APP  0x08040000
#define BOOT_FLASH_CAND_APP 0x08240000

#if defined(CONFIG_BOOT_MODE_ENCRYPTED_XIP_OVERWRITE) || defined(CONFIG_BOOT_MODE_ENCRYPTED_XIP_REMAP)
/* Encrypted XIP extension: offset of metadata region */
#define BOOT_FLASH_SLOT0_ENC_CFG_ADDRESS  0x08440000
#define BOOT_FLASH_SLOT1_ENC_CFG_ADDRESS  0x08441000
#endif

#endif /* !defined(CONFIG_BOOT_CUSTOM_DEVICE_SETUP) */

#endif /* _FLASH_PARTITIONING_H_ */
