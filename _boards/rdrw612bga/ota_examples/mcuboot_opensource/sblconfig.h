/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef SBL_CONFIG_H__
#define SBL_CONFIG_H__


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

/* Board specific register for flash remap functionality */
#define FLASH_REMAP_START_REG           0x40134420      /* RW61x flash remap start address register */
#define FLASH_REMAP_END_REG             0x40134424      /* RW61x flash remap end address register */
#define FLASH_REMAP_OFFSET_REG          0x40134428      /* RW61x flash remap offset register */

/* Encrypted XIP support config */

/*
 * Uncomment to enable extension utilizing on-the-fly decryption of encrypted image.
 * Note: Flash remap feature has to be disabled.
 * For more information please see readme file of mcuboot_opensource example.
 */
//#define CONFIG_ENCRYPT_XIP_EXT_ENABLE

#if defined(CONFIG_ENCRYPT_XIP_EXT_ENABLE) && \
    (!defined(MBEDTLS_MCUX_DISABLE_HW_ALT) || \
      defined(MBEDTLS_MCUX_USE_ELS) ||        \
      defined(MBEDTLS_MCUX_USE_PKC))
#error "There is currently an issue in mbedTLS if hardware acceleration and IPED \
are enabled on RW61x, please remove global defines MBEDTLS_MCUX_USE_ELS and \
MBEDTLS_MCUX_USE_PKC and add MBEDTLS_MCUX_DISABLE_HW_ALT in your build."
#endif

/*
 * Optional:
 * Uncomment to use simpler OVERWRITE_ONLY mode instead of three slot configuration.
 */
//#define CONFIG_ENCRYPT_XIP_EXT_OVERWRITE_ONLY

/*
 * The PRINCE variant used in RW61x is based on AES in Galois/Counter Mode (GCM)
 * Algortihm of encryption unit consumes 1.25 (5/4) time of physical memory.
 * Also we need to take into account one sector for mcuboot trailer.
 * Calculation: 
 * 4.25MB~4352kB slot size = 1088 sectors
 * 1088 sectors - 1 trailer sector = 1087 sectors
 * Aligning down to 1085 sectors so value is a multiple of 5 sectors in size
 * 1085 sectors / 1.25 = 868 sectors ~ 3472 kB is then size of IPED region
 * 3472kB of plaintext generates 868kB of IPED tags. Both values suits
 * the boundaries of pages, sectors and the boundaries of encryption alignment
 */
#define CONFIG_ENCRYPT_XIP_IPED_REGION_SIZE  0x364000

/*
 * Size of write buffer used for overwrite-only mode has to be adjusted if IPED
 * encryption unit is used so size of data chunks written to flash are always
 * a multiple of 4 pages in size.
 */
#define CONFIG_ENCRYPT_XIP_OVERWRITE_ONLY_BUF_SIZE      (4*256)

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

#endif
