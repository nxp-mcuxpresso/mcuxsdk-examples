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

/*
The memory is allocated as follows:
    Default:
    - BOOTLOADER:  0x020000 bytes @ 0x08000000
    - APP_ACT:     0x440000 bytes @ 0x08020000
    - APP_CAND:    0x440000 bytes @ 0x08460000
    Encrypted XIP - three slot mode:
    - BOOTLOADER:  0x020000 bytes @ 0x08000000
    - EXEC_ACT:    0x440000 bytes @ 0x08020000
    - APP_ACT:     0x440000 bytes @ 0x08460000
    - APP_CAND:    0x440000 bytes @ 0x088A0000
    - ENC_META:    0x001000 bytes @ 0x08CE0000
    Encrypted XIP - overwrite-only mode:
    - BOOTLOADER:  0x020000 bytes @ 0x08000000
    - APP_ACT:     0x440000 bytes @ 0x08020000
    - APP_CAND:    0x440000 bytes @ 0x08460000
    - ENC_META:    0x001000 bytes @ 0x088A0000
*/

//Todo temporary decreased to 1MB slot size for testing purpose
#define BOOT_FLASH_BASE     0x08000000

#if !defined(CONFIG_ENCRYPT_XIP_EXT_ENABLE)
/* Overwrite-only, swap or direct-xip mode with flash remapping */

#define BOOT_FLASH_ACT_APP  0x08020000
#define BOOT_FLASH_CAND_APP 0x08460000

#elif defined(CONFIG_ENCRYPT_XIP_EXT_OVERWRITE_ONLY)
/* Encrypted XIP extension: modified overwrite-only mode */

#define BOOT_FLASH_ACT_APP  0x08020000
#define BOOT_FLASH_CAND_APP 0x08460000
#define BOOT_FLASH_ENC_META 0x088A0000
#define BOOT_FLASH_EXEC_APP BOOT_FLASH_ACT_APP

#else
/* Encrypted XIP extension: Three slot mode */

#define BOOT_FLASH_EXEC_APP 0x08020000
#define BOOT_FLASH_ACT_APP  0x08460000
#define BOOT_FLASH_CAND_APP 0x088A0000
#define BOOT_FLASH_ENC_META 0x08CE0000

#endif /* !defined(CONFIG_ENCRYPT_XIP_EXT_ENABLE) */

#endif /* _FLASH_PARTITIONING_H_ */
