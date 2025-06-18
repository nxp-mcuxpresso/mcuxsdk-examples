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
#include "mflash_drv.h"

#define BOOT_FLASH_BASE                 0x60000000

#if defined(CONFIG_BOOT_CUSTOM_DEVICE_SETUP)
/* Layout setup from Kconfig */

#define BOOT_FLASH_ACT_APP              CONFIG_BOOT_FLASH_ACT_APP_ADDRESS
#define BOOT_FLASH_CAND_APP             CONFIG_BOOT_FLASH_CAND_APP_ADDRESS

#if CONFIG_ENCRYPT_XIP_ENABLE
#define BOOT_FLASH_EXEC_APP             CONFIG_BOOT_FLASH_EXEC_APP_ADDRESS
#define BOOT_FLASH_ENC_META             CONFIG_BOOT_FLASH_ENC_META_ADDRESS
#endif

#else
/* Default layout setup

The memory is allocated as follows:
    - BOOTLOADER:  0x020000 bytes @ 0x60000000 - MCUboot
    - APP_ACT:     0x400000 bytes @ 0x60040000 - primary slot
    - APP_CAND:    0x400000 bytes @ 0x60440000 - secondary slot
    Encrypted XIP support:
    - ENC_META:    0x001000 bytes @ 0x60840000 - encrypted XIP metadata
*/

#define BOOT_FLASH_ACT_APP              0x60040000
#define BOOT_FLASH_CAND_APP             0x60440000

#if defined(CONFIG_ENCRYPT_XIP_EXT_ENABLE)
/* Encrypted XIP extension: define metadata and execution region */
#define BOOT_FLASH_ENC_META             0x60840000
#define BOOT_FLASH_EXEC_APP             BOOT_FLASH_ACT_APP
#endif

#endif /* defined(CONFIG_BOOT_CUSTOM_DEVICE_SETUP) */

#endif /* _FLASH_PARTITIONING_H_ */

