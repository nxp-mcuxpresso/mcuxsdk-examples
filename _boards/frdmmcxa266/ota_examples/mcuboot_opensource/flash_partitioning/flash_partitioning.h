/*
 * Copyright 2025 NXP
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
  MCUBoot region            0x000000 - 0x010000 : 64kB  (0x10000 bytes)
  Primary slot              0x010000 - 0x082000 : 456kB (0x72000 bytes)
  Secondary slot            0x082000 - 0x0F4000 : 456kB (0x72000 bytes)
  Reserved                  0x0F4000 - 0x0F6000 : 8kB   (0x02000 bytes)
  Secure installer (locked) 0x0F6000 - 0x0FE000 : 32kB  (0x08000 bytes)
  CMPA (reserved)           0x0FE000 - 0x100000 : 8kB   (0x02000 bytes)
*/

#define BOOT_FLASH_ACT_APP  0x00010000
#define BOOT_FLASH_CAND_APP 0x00082000

#endif /* CONFIG_BOOT_CUSTOM_DEVICE_SETUP */

#endif
