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
/*
 Layout setup
*/
#ifndef CONFIG_MCXN_CUSTOM_CFG_MAIN_FLASH_ONLY
/*
  Default configuration
  Bootloader located in Bank 1 IFR 0 Region (0x0100_8000 - 0x0100_FFFF)
  This configuration supports flash remap

  0x0000_0000  +------------------------+ Flash Start
               | Application_Primary    | 1024 kB
  0x0010_0000  +------------------------+
               | Application_Secondary  | 1024 kB
  0x0020_0000  +------------------------+
                          ...
  0x0100_8000  +------------------------+
               | Bootloader             | 32 kB
  0x0100_FFFF  +------------------------+
*/
#define BOOT_FLASH_ACT_APP  0x00000000
#define BOOT_FLASH_CAND_APP 0x00100000
#else
/*
  Custom configuration - see readme file
  Bootloader located in main flash
  This configuration supports mbedTLS by default and optionally encrypted XIP using NPX

  0x0000_0000  +------------------------+ Flash Start
               | Bootloader             | 256 kB
  0x0004_0000  +------------------------+
               | Application_Primary    | 896 kB
  0x0012_0000  +------------------------+
               | Application_Secondary  | 896 kB
  0x0020_0000  +------------------------+
*/
#define BOOT_FLASH_ACT_APP  0x00040000
#define BOOT_FLASH_CAND_APP 0x00120000

#if defined(CONFIG_BOOT_MODE_ENCRYPTED_XIP_OVERWRITE)
#define BOOT_FLASH_SLOT0_ENC_CFG_ADDRESS (BOOT_FLASH_ACT_APP - 8192U) /* Dummy area in this configuration, reserved for future use */
#endif

#endif /* !CONFIG_MCXN_CUSTOM_CFG_MAIN_FLASH_ONLY */

#endif /* CONFIG_BOOT_CUSTOM_DEVICE_SETUP */
#endif /* _FLASH_PARTITIONING_H_ */
