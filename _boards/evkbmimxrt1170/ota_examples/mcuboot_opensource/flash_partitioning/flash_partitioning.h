/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FLASH_PARTITIONING_H_
#define _FLASH_PARTITIONING_H_

#define BOOT_FLASH_BASE                 0x30000000

#if defined(CONFIG_BOOT_CUSTOM_DEVICE_SETUP)
/* Layout setup from Kconfig */

#define BOOT_FLASH_ACT_APP              CONFIG_BOOT_FLASH_ACT_APP_ADDRESS
#define BOOT_FLASH_CAND_APP             CONFIG_BOOT_FLASH_CAND_APP_ADDRESS
#define BOOT_FLASH_PADDING_SIZE         CONFIG_BOOT_FLASH_PADDING_SIZE

#if CONFIG_BOOT_MODE_ENCRYPTED_XIP_OVERWRITE
#define BOOT_FLASH_SLOT0_ENC_CFG_ADDRESS  CONFIG_BOOT_FLASH_SLOT0_ENC_CFG_ADDRESS
#endif

#else
/* Default layout setup

The memory is allocated as follows:
    - BOOTLOADER:  0x040000 bytes @ 0x30000000 - MCUboot
    - APP_ACT:     0x200000 bytes @ 0x30040000 - primary slot
    - APP_CAND:    0x200000 bytes @ 0x30240000 - secondary slot
    Encrypted XIP support:
    - ENC_META:    0x001000 bytes @ 0x30440000 - encrypted XIP metadata
*/

#define BOOT_FLASH_ACT_APP              0x30040000

#ifdef CONFIG_MCUBOOT_SWAP_MOVE
#define BOOT_FLASH_CAND_APP             0x30440000
#define BOOT_FLASH_PADDING_SIZE         4096
#define CONFIG_MCUBOOT_MAX_IMG_SECTORS  (((BOOT_FLASH_CAND_APP - BOOT_FLASH_ACT_APP)/BOOT_FLASH_PADDING_SIZE))
#else
#define BOOT_FLASH_CAND_APP             0x30240000
#define BOOT_FLASH_PADDING_SIZE         0
#define CONFIG_MCUBOOT_MAX_IMG_SECTORS  800
#endif

#endif /* CONFIG_BOOT_CUSTOM_DEVICE_SETUP */

#endif
