/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FLASH_PARTITIONING_H_
#define _FLASH_PARTITIONING_H_

#include "mcux_config.h"
#include "sblconfig.h"
#include "mflash_drv.h"

#define BOOT_FLASH_BASE     0x00000000

#if defined(CONFIG_BOOT_CUSTOM_DEVICE_SETUP)
/* Layout setup from Kconfig */

#define BOOT_FLASH_ACT_APP                CONFIG_BOOT_FLASH_ACT_APP_ADDRESS
#define BOOT_FLASH_CAND_APP               CONFIG_BOOT_FLASH_CAND_APP_ADDRESS
#define BOOT_FLASH_SLOT0_ENC_CFG_ADDRESS  CONFIG_BOOT_FLASH_SLOT0_ENC_CFG_ADDRESS

#else
/* Default layout setup

The memory is allocated as follows:
    - BOOTLOADER:      32kB @ 0x00000000 - MCUboot
    - APP_ACT:         32kB @ 0x00008000 - primary slot
    - APP_CAND:        0kB  @ 0x00010000 - dummy secondary slot 
*/
#define BOOT_FLASH_ACT_APP  0x00008000
#define BOOT_FLASH_CAND_APP 0x00010000 //value needed for slot size calculation


#endif /* CONFIG_BOOT_CUSTOM_DEVICE_SETUP */
#endif /* _FLASH_PARTITIONING_H_ */
