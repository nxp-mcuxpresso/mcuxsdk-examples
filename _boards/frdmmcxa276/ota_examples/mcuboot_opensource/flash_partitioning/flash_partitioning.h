/*
 * Copyright 2021 NXP
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FLASH_PARTITIONING_H_
#define _FLASH_PARTITIONING_H_

/*
  MCUBoot region            0x000000 - 0x007FFF : 32kB  (0x08000 bytes)
  Primary slot              0x008000 - 0x07DFFF : 472kB (0x76000 bytes)
  Secondary slot            0x07E000 - 0x0F3FFF : 472kB (0x76000 bytes)
  Reserved                  0x0F4000 - 0x0F5FFF : 8kB   (0x02000 bytes)
  Secure installer (locked) 0x0F6000 - 0x0FDFFF : 32kB  (0x08000 bytes)
  CMPA (reserved)           0x0FE000 - 0x100000 : 8kB   (0x02000 bytes)
*/

#define BOOT_FLASH_BASE     0x00000000
#define BOOT_FLASH_ACT_APP  0x00008000
#define BOOT_FLASH_CAND_APP 0x0007E000

#endif
