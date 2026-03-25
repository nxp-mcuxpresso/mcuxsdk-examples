/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "flash_partitioning.h"
#include "flash_map.h"
#include "mcuboot_config.h"
#include "sysflash/sysflash.h"
#include "mflash_drv.h"

const char *boot_image_names[MCUBOOT_IMAGE_NUMBER] = {"APP"};

struct flash_area boot_flash_map[MCUBOOT_IMAGE_SLOT_NUMBER] = {
    /* Image 0; slot 0 - Main Application Primary Slot  */
    {.fa_id        = 0,
     .fa_device_id = FLASH_DEVICE_ID,
     .fa_off       = BOOT_FLASH_ACT_APP - BOOT_FLASH_BASE,
     .fa_size      = BOOT_FLASH_CAND_APP - BOOT_FLASH_ACT_APP,
     .fa_name      = "APP_PRIMARY"},

    /* Image 0; slot 1 - Main Application Secondary Slot  */
    {.fa_id        = 1,
     .fa_device_id = FLASH_DEVICE_ID,
     .fa_off       = BOOT_FLASH_CAND_APP - BOOT_FLASH_BASE,
     .fa_size      = BOOT_FLASH_CAND_APP - BOOT_FLASH_ACT_APP,
     .fa_name      = "APP_SECONDARY"}};

    
#if defined(CONFIG_BOOT_MODE_ENCRYPTED_XIP_REMAP) || defined(CONFIG_BOOT_MODE_ENCRYPTED_XIP_OVERWRITE)
/* Encrypted XIP metadata storage */
struct flash_area boot_flash_meta_map[2] = {
    /* Image 0; slot 0 IPED configuration  */
    {.fa_id        = 0,
     .fa_device_id = FLASH_DEVICE_ID,
     .fa_off       = BOOT_FLASH_SLOT0_ENC_CFG_ADDRESS - BOOT_FLASH_BASE,
     .fa_size      = MFLASH_SECTOR_SIZE,
     .fa_name      = "ENC_META_SLOT_0"},

    /* Image 0; slot 1 IPED configuration  */
    /* Dummy for overwrite only */
    {.fa_id        = 1,
     .fa_device_id = FLASH_DEVICE_ID,
     .fa_off       = BOOT_FLASH_SLOT1_ENC_CFG_ADDRESS - BOOT_FLASH_BASE,
     .fa_size      = MFLASH_SECTOR_SIZE,
     .fa_name      = "ENC_META_SLOT_1"}};
#endif
