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

#define BOOT_FLASH_ACT_APP              CONFIG_BOOT_FLASH_ACT_APP_ADDRESS
#define BOOT_FLASH_CAND_APP             CONFIG_BOOT_FLASH_CAND_APP_ADDRESS

#else
/*
 Layout setup
*/
/*
  Default configuration
  Bootloader located in Bank 1 IFR 0 Region (0x0100_8000 - 0x0100_FFFF)
  This configuration supports flash remap

  0x0000_0000  +------------------------+ Flash Start
               | Application_Primary    | 512 kB
  0x0008_0000  +------------------------+
               | Application_Secondary  | 512 kB
  0x0010_0000  +------------------------+
                          ...
  0x0100_8000  +------------------------+
               | Bootloader             | 32 kB
  0x0100_FFFF  +------------------------+
*/
#define BOOT_FLASH_ACT_APP  0x00000000
#define BOOT_FLASH_CAND_APP 0x00080000

#endif /* CONFIG_BOOT_CUSTOM_DEVICE_SETUP */

#endif /* _FLASH_PARTITIONING_H_ */
