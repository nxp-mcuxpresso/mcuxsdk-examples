/*
 * Copyright 2024 NXP
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

/*
 * Maximum size of IPED region
 *
 * The PRINCE variant used in RW61x is based on AES in Galois/Counter Mode (GCM)
 * Algorithm of encryption unit consumes 1.25 (5/4) time of physical memory.
 * Also we need to take into account one sector reserved for the mcuboot trailer.
 *
 * Calculation: 
 * 2MB~2048kB slot size = 512 sectors
 * 512 sectors - 1 reserved sector for the trailer = 511 sectors
 * Aligning down to 510 sectors (2040kB) so the value is a multiple of 5 sectors
 * in size.
 * 510 sectors / 1.25 = 408 sectors ~ 1632kB is then size of IPED region usable
 * for a payload.
 * 1632kB of plaintext then generates 408kB of IPED tags. Both values suits
 * the boundaries of pages, sectors and the boundaries of encryption alignment.
 */
#define CONFIG_ENCRYPT_XIP_IPED_REGION_MAX_SIZE  0x364000

/*
 * Size of write buffer used for overwrite-only mode has to be adjusted if IPED 
 * encryption unit is used so that the size of data chunks written to flash are 
 * always a multiple of 4 pages in size.
 */
#define CONFIG_ENCRYPT_XIP_OVERWRITE_ONLY_BUF_SIZE      (4*MFLASH_PAGE_SIZE)


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
 * MCUBoot upgrade mode
 *
 * The default MCUBoot configuration is to use swap mechanism. In case the flash
 * remapping functionality is supported by processor the alternative mechanism
 * using direct-xip mode can be used and evaluated by user.
 * Comment this to enable swap mode or when encrypted XIP extension is enabled.
 */
#define CONFIG_MCUBOOT_FLASH_REMAP_ENABLE

/*
 * Enable the encrypted XIP extension. This extension is not compatible with the 
 * flash remapping feature as it implicitly uses overwrite only mode.
 * For more information, please see readme file.
 */
//#define CONFIG_ENCRYPT_XIP_EXT_ENABLE


/* Crypto Config */
// #define CONFIG_BOOT_OTA_TEST
/* uncomment to generate MCU boot for testing without image signature verification */

#ifdef CONFIG_BOOT_OTA_TEST
#define CONFIG_BOOT_NO_SIGNATURE
#endif
#ifndef CONFIG_BOOT_NO_SIGNATURE
#define COMPONENT_MCUBOOT_SECURE
#define CONFIG_BOOT_SIGNATURE
#define CONFIG_BOOT_SIGNATURE_TYPE_RSA
#define CONFIG_BOOT_SIGNATURE_TYPE_RSA_LEN 2048
#endif
#define COMPONENT_MBEDTLS
#define CONFIG_BOOT_BOOTSTRAP

#endif /* CONFIG_BOOT_CUSTOM_DEVICE_SETUP */


/* Config Guards */

#if defined(CONFIG_ENCRYPT_XIP_EXT_ENABLE) && \
    (!defined(MBEDTLS_MCUX_DISABLE_HW_ALT) || \
      defined(MBEDTLS_MCUX_USE_ELS) ||        \
      defined(MBEDTLS_MCUX_USE_PKC))
#error "There is currently an issue in mbedTLS if hardware acceleration and IPED \
are enabled on RW61x, please remove global defines MBEDTLS_MCUX_USE_ELS and \
MBEDTLS_MCUX_USE_PKC and add MBEDTLS_MCUX_DISABLE_HW_ALT in your build to use \
software port layer."
#endif

#endif
