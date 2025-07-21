/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef SBL_CONFIG_H__
#define SBL_CONFIG_H__

/*******************************************************************/
/* Use default configuration if setup from Kconfig is not provided */
/*******************************************************************/
#ifndef CONFIG_BOOT_CUSTOM_DEVICE_SETUP

/* MCUBoot Flash Config */

#define CONFIG_MCUBOOT_MAX_IMG_SECTORS 800

/*
 * Enable the encrypted XIP extension. This extension is not compatible with the 
 * flash remapping feature as it implicitly uses overwrite only mode.
 * For more information, please see readme file.
 */
//#define CONFIG_ENCRYPT_XIP_EXT_ENABLE

#define CONFIG_BOOT_BOOTSTRAP

/* Crypto Config */

#define CONFIG_BOOT_SIGNATURE
#define CONFIG_BOOT_SIGNATURE_TYPE_ECDSA_P256

#ifndef CONFIG_ENCRYPT_XIP_EXT_ENABLE
#define CONFIG_BOOT_USE_PSA_CRYPTO
#else
#define CONFIG_BOOT_USE_MBEDTLS
#define CONFIG_BOOT_ENCRYPT_EC256
#endif

#endif /* CONFIG_BOOT_CUSTOM_DEVICE_SETUP */

#endif
