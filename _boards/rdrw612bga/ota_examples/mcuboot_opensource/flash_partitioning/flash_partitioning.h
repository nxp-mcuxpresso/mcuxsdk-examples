/*
 * Copyright 2021,2024 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "sblconfig.h"
#include "mflash_drv.h"

#ifndef _FLASH_PARTITIONING_H_
#define _FLASH_PARTITIONING_H_


#define BOOT_FLASH_BASE     0x08000000


#if defined(CONFIG_BOOT_CUSTOM_DEVICE_SETUP)
/* Layout setup from Kconfig */

#define BOOT_FLASH_ACT_APP              CONFIG_BOOT_FLASH_ACT_APP_ADDRESS
#define BOOT_FLASH_CAND_APP             CONFIG_BOOT_FLASH_CAND_APP_ADDRESS

#if CONFIG_ENCRYPT_XIP_ENABLE
#define BOOT_FLASH_EXEC_APP             CONFIG_BOOT_FLASH_EXEC_APP_ADDRESS
#define BOOT_FLASH_ENC_META             CONFIG_BOOT_FLASH_ENC_META_ADDRESS
#endif


/* Default layout setup */

/*
The memory is allocated as follows:
    Default:
    - BOOTLOADER:  0x020000 bytes @ 0x08000000
    - APP_ACT:     0x200000 bytes @ 0x08020000
    - APP_CAND:    0x200000 bytes @ 0x08220000
    Encrypted XIP:
    - BOOTLOADER:  0x020000 bytes @ 0x08000000
    - APP_ACT:     0x200000 bytes @ 0x08020000
    - APP_CAND:    0x200000 bytes @ 0x08220000
    - ENC_META:    0x001000 bytes @ 0x08420000
*/

#else
/* Overwrite-only, swap or direct-xip mode with flash remapping */

#define BOOT_FLASH_ACT_APP  0x08020000
#define BOOT_FLASH_CAND_APP 0x08220000

#if defined(CONFIG_ENCRYPT_XIP_EXT_ENABLE)
/* Encrypted XIP extension: modified overwrite-only mode */
#define BOOT_FLASH_ENC_META 0x08420000
#define BOOT_FLASH_EXEC_APP BOOT_FLASH_ACT_APP
#endif

#endif /* !defined(CONFIG_BOOT_CUSTOM_DEVICE_SETUP) */

#endif /* _FLASH_PARTITIONING_H_ */
