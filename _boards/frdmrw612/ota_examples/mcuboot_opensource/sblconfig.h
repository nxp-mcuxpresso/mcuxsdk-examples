/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef SBL_CONFIG_H__
#define SBL_CONFIG_H__
#include "mflash_drv.h"

/* Board specific register for flash remap functionality */
#define FLASH_REMAP_START_REG           0x40134420      /* RW61x flash remap start address register */
#define FLASH_REMAP_END_REG             0x40134424      /* RW61x flash remap end address register */
#define FLASH_REMAP_OFFSET_REG          0x40134428      /* RW61x flash remap offset register */

/*******************************************************************/
/* Use default configuration if setup from Kconfig is not provided */
/*******************************************************************/
#ifndef CONFIG_BOOT_CUSTOM_DEVICE_SETUP


/* MCUBoot Flash Config */

#define CONFIG_MCUBOOT_MAX_IMG_SECTORS 1090u

/*
 * Number of image pairs is 1 in the case of the monolithic application.
 * This is mandated by the MATTER specification.
 */
#define CONFIG_UPDATEABLE_IMAGE_NUMBER 1

/*
 * MCUBoot upgrade mode (default)
 * If supported by the device, an alternative upgrade mode can be selected.
 * For more information please see documentation (ota_examples/_doc).
 */
//#define CONFIG_BOOT_MODE_OVERWRITE_ONLY
#define CONFIG_BOOT_MODE_FLASH_REMAP
//#define CONFIG_BOOT_MODE_ENCRYPTED_XIP_OVERWRITE
//#define CONFIG_BOOT_MODE_ENCRYPTED_XIP_REMAP       /* Experimental */
//#define CONFIG_BOOT_MODE_SINGLE_APPLICATION_SLOT
//#define CONFIG_BOOT_MODE_SWAP

/* Misc Config */

#define CONFIG_BOOT_BOOTSTRAP
#define CONFIG_BOOT_SERIAL_RECOVERY

/* Crypto Config */
/* uncomment to generate MCU boot for testing without image signature verification */
// #define CONFIG_BOOT_OTA_TEST

#ifdef CONFIG_BOOT_OTA_TEST
#define CONFIG_BOOT_NO_SIGNATURE
#endif
#ifndef CONFIG_BOOT_NO_SIGNATURE
#define CONFIG_BOOT_SIGNATURE
#endif

#define CONFIG_BOOT_SIGNATURE_TYPE_ECDSA_P256

#if !(defined(CONFIG_BOOT_MODE_ENCRYPTED_XIP_OVERWRITE) || defined(CONFIG_BOOT_MODE_ENCRYPTED_XIP_REMAP))
#define CONFIG_BOOT_USE_PSA_CRYPTO
#else
#define CONFIG_BOOT_USE_MBEDTLS
#if defined(CONFIG_BOOT_MODE_ENCRYPTED_XIP_OVERWRITE)
#define CONFIG_BOOT_ENCRYPT_EC256
#endif
#endif

#endif /* CONFIG_BOOT_CUSTOM_DEVICE_SETUP */

#endif /* SBL_CONFIG_H__ */