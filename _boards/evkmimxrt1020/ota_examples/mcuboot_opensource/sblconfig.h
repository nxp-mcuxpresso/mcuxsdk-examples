/*
 * Copyright 2021 NXP
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
 * MCUBoot upgrade mode (default)
 * If supported by the device, an alternative upgrade mode can be selected.
 * For more information please see documentation (ota_examples/_doc).
 */
//#define CONFIG_BOOT_MODE_OVERWRITE_ONLY
//#define CONFIG_BOOT_MODE_ENCRYPTED_XIP_OVERWRITE
//#define CONFIG_BOOT_MODE_SINGLE_APPLICATION_SLOT
#define CONFIG_BOOT_MODE_SWAP

/* Misc Config */

#define CONFIG_BOOT_BOOTSTRAP
#define CONFIG_BOOT_SERIAL_RECOVERY

/* Crypto Config */

#define CONFIG_BOOT_SIGNATURE
#define CONFIG_BOOT_SIGNATURE_TYPE_ECDSA_P256

#ifndef CONFIG_BOOT_MODE_ENCRYPTED_XIP_OVERWRITE
#define CONFIG_BOOT_USE_PSA_CRYPTO
#else
#define CONFIG_BOOT_USE_MBEDTLS
#define CONFIG_BOOT_ENCRYPT_EC256
#endif

#endif /* CONFIG_BOOT_CUSTOM_DEVICE_SETUP */

#endif
