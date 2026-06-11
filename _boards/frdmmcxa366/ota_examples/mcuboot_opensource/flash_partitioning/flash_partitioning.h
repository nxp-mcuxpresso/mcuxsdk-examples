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
  MCUBoot region            0x000000 - 0x00BFFF : 48kB  (0x0C000 bytes)
  Primary slot              0x00C000 - 0x07DFFF : 456kB (0x72000 bytes)
  Secondary slot            0x07E000 - 0x0EFFFF : 456kB (0x72000 bytes)
  Secure installer (locked) 0x0F0000 - 0x0FDFFF : 56kB  (0x0E000 bytes)
  CMPA (reserved)           0x0FE000 - 0x100000 : 8kB   (0x02000 bytes)
*/

#define BOOT_FLASH_ACT_APP  0x0000C000
#define BOOT_FLASH_CAND_APP 0x0007E000

#endif /* CONFIG_BOOT_CUSTOM_DEVICE_SETUP */

#endif
