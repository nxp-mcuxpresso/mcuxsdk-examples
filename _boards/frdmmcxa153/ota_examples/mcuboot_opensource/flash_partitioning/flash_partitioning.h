/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FLASH_PARTITIONING_H_
#define _FLASH_PARTITIONING_H_

#define BOOT_FLASH_BASE     0x00000000


#if defined(CONFIG_BOOT_CUSTOM_DEVICE_SETUP)
/* Layout setup from Kconfig */

#define BOOT_FLASH_ACT_APP              CONFIG_BOOT_FLASH_ACT_APP_ADDRESS
#define BOOT_FLASH_CAND_APP             CONFIG_BOOT_FLASH_CAND_APP_ADDRESS

#else
/* Default layout setup */

/*
  MCUBoot region       0x0     - 0x08000 : 32kB
  Primary slot         0x8000  - 0x20000 : 96kB (0x18000 bytes)
*/

#define BOOT_FLASH_ACT_APP  0x8000
#define BOOT_FLASH_ACT_SIZE 0x18000

#endif /* CONFIG_BOOT_CUSTOM_DEVICE_SETUP */

#endif
