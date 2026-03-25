/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef SBL_CONFIG_H__
#define SBL_CONFIG_H__

#include "mcux_config.h"

#define CONFIG_MCUBOOT_FLASH_REMAP_BY_SWAP
#define FLASH_REMAP_OFFSET_REG (&(NPX0->REMAP))

/* MCX N10 series has ECC Flash with minimum 16 byte write */
#define MCUBOOT_BOOT_MAX_ALIGN 16

/*******************************************************************/
/* Use default configuration if setup from Kconfig is not provided */
/*******************************************************************/
#ifndef CONFIG_BOOT_CUSTOM_DEVICE_SETUP

#ifndef CONFIG_MCXN_CUSTOM_CFG_MAIN_FLASH_ONLY
/* HW flash remap feature is used if MCUBoot is located in IFR region */
#define CONFIG_BOOT_MODE_FLASH_REMAP
#endif

/* Encrypted XIP support config */

/*
 * Uncomment to enable extension utilizing on-the-fly decryption of encrypted image.
 * Note: This configuration is compatible only with MCUBoot placed in main flash
 * array.
 * For more information please see readme file of mcuboot_opensource example.
 */
//#define CONFIG_BOOT_MODE_ENCRYPTED_XIP_OVERWRITE

/* MCUBoot Flash Config */

/* Slot size being 896 kB divided by 8 kB sector size gives 112 sectors as minimum value */
/* 256 * minimum write alignment 16 B gives 4 kB swap status size */

#define CONFIG_MCUBOOT_MAX_IMG_SECTORS 256

/* Misc Config */

#define CONFIG_BOOT_BOOTSTRAP
//#define CONFIG_BOOT_SERIAL_RECOVERY

/* Crypto Config */

#define CONFIG_BOOT_SIGNATURE
#define CONFIG_BOOT_SIGNATURE_TYPE_ECDSA_P256

#ifdef CONFIG_MCXN_CUSTOM_CFG_MAIN_FLASH_ONLY

/*
 * MCUBoot is located in main flash -> Use mbedTLS
 */
#ifdef CONFIG_BOOT_MODE_ENCRYPTED_XIP_OVERWRITE
#define CONFIG_BOOT_USE_MBEDTLS
#define CONFIG_BOOT_ENCRYPT_EC256
#else
#define CONFIG_BOOT_USE_PSA_CRYPTO
#endif

#else

/*
 * MCUBoot is located in IFR region -> Use TinyCrypt
 */
#define CONFIG_BOOT_USE_TINYCRYPT
#endif /* CONFIG_MCXN_CUSTOM_CFG_MAIN_FLASH_ONLY */

#endif /* CONFIG_BOOT_CUSTOM_DEVICE_SETUP */

/* Config Guards */
#if defined(CONFIG_BOOT_MODE_ENCRYPTED_XIP_OVERWRITE) && \
    !defined(CONFIG_MCXN_CUSTOM_CFG_MAIN_FLASH_ONLY)
#error "Encrypted XIP using NPX is not supported when MCUBoot is placed in IFR region."
#endif
#if defined(CONFIG_MCUBOOT_FLASH_REMAP_ENABLE) && \
    defined(CONFIG_MCXN_CUSTOM_CFG_MAIN_FLASH_ONLY)
#error "Flash remap is not supported when MCUBoot is placed in main flash region."
#endif

#endif /* SBL_CONFIG_H__ */
