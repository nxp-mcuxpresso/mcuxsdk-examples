/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef SBL_CONFIG_H__
#define SBL_CONFIG_H__

/* MCX E2 series has ECC Flash with minimal write size 8 bytes */
#define MCUBOOT_BOOT_MAX_ALIGN 8

/*******************************************************************/
/* Use default configuration if setup from Kconfig is not provided */
/*******************************************************************/
#ifndef CONFIG_BOOT_CUSTOM_DEVICE_SETUP

/*
 * MCUBoot upgrade mode (default)
 * If supported by the device, an alternative upgrade mode can be selected.
 * For more information please see documentation (ota_examples/_doc).
 */
//#define CONFIG_BOOT_MODE_OVERWRITE_ONLY
//#define CONFIG_BOOT_MODE_SINGLE_APPLICATION_SLOT
#define CONFIG_BOOT_MODE_SWAP

/* To be able to bootstrap signed image from debug session */
#define CONFIG_BOOT_BOOTSTRAP

/* CONFIG_MCUBOOT_MAX_IMG_SECTORS >= (AppImageSize / SectorSize) */
#define CONFIG_MCUBOOT_MAX_IMG_SECTORS 200

#define CONFIG_BOOT_SIGNATURE
#define CONFIG_BOOT_SIGNATURE_TYPE_ECDSA_P256
#define MCUBOOT_USE_TINYCRYPT

#endif /* CONFIG_BOOT_CUSTOM_DEVICE_SETUP */

#endif
