/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FLASH_PARTITIONING_H_
#define _FLASH_PARTITIONING_H_

#include "sblconfig.h"

#define BOOT_FLASH_BASE                 0x70000000

#if defined(CONFIG_BOOT_CUSTOM_DEVICE_SETUP)
/* Layout setup from Kconfig */

#define BOOT_FLASH_ACT_APP                CONFIG_BOOT_FLASH_ACT_APP_ADDRESS
#define BOOT_FLASH_CAND_APP               CONFIG_BOOT_FLASH_CAND_APP_ADDRESS

#if CONFIG_BOOT_MODE_ENCRYPTED_XIP_OVERWRITE
#define BOOT_FLASH_SLOT0_ENC_CFG_ADDRESS  CONFIG_BOOT_FLASH_SLOT0_ENC_CFG_ADDRESS
#endif

#else
/* Default layout setup

The memory is allocated as follows:
    - BOOTLOADER:  0x020000 bytes @ 0x70000000 - MCUboot
    - APP_ACT:     0x100000 bytes @ 0x70040000 - primary slot
    - APP_CAND:    0x100000 bytes @ 0x70140000 - secondary slot
    Encrypted XIP support:
    - ENC_META:    0x001000 bytes @ 0x70240000 - encrypted XIP metadata
*/

#define BOOT_FLASH_ACT_APP                0x70040000
#define BOOT_FLASH_CAND_APP               0x70140000

#if defined(CONFIG_BOOT_MODE_ENCRYPTED_XIP_OVERWRITE)
/* Encrypted XIP extension: offset of metadata region */
#define BOOT_FLASH_SLOT0_ENC_CFG_ADDRESS  0x70240000
#endif

#endif /* defined(CONFIG_BOOT_CUSTOM_DEVICE_SETUP) */

#endif /* _FLASH_PARTITIONING_H_ */
