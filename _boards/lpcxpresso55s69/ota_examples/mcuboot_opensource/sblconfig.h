/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef SBL_CONFIG_H__
#define SBL_CONFIG_H__

#define SOC_LPC55S69_SERIES

/*******************************************************************/
/* Use default configuration if setup from Kconfig is not provided */
/*******************************************************************/
#ifndef CONFIG_BOOT_CUSTOM_DEVICE_SETUP


/* Flash device parameters */

/* 128kB mcuboot + 192kB AppImage + 192kB AppImageNew */
#define COMPONENT_FLASHIAP_SIZE 524288

/* CONFIG_MCUBOOT_MAX_IMG_SECTORS >= (AppImageSize / SectorSize) */
#define CONFIG_MCUBOOT_MAX_IMG_SECTORS 400

/*
 * MCUBoot upgrade mode (default)
 * If supported by the device, an alternative upgrade mode can be selected.
 * For more information please see documentation (ota_examples/_doc).
 */
/*
 * LPC55S69 with ECC Flash limits use of revert strategies (move/swap).
 * At least with current MCUBoot implementation.
 */
#define CONFIG_BOOT_MODE_OVERWRITE_ONLY
//#define CONFIG_BOOT_MODE_SINGLE_APPLICATION_SLOT

#define CONFIG_BOOT_BOOTSTRAP

/* Crypto */

#define CONFIG_BOOT_SIGNATURE
#define CONFIG_BOOT_SIGNATURE_TYPE_ECDSA_P256
#define CONFIG_BOOT_USE_TINYCRYPT

#endif /* CONFIG_BOOT_CUSTOM_DEVICE_SETUP */

#endif
