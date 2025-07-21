/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef SBL_CONFIG_H__
#define SBL_CONFIG_H__

/* Board specific register for flash remap functionality */
#define FLASH_REMAP_OFFSET_REG 0x400CC428 /* RT1160 flash remap offset register */


/*******************************************************************/
/* Use default configuration if setup from Kconfig is not provided */
/*******************************************************************/
#ifndef CONFIG_BOOT_CUSTOM_DEVICE_SETUP

/* MCUBoot Flash Config */

#define CONFIG_MCUBOOT_MAX_IMG_SECTORS 800

/*
 * MCUBoot upgrade mode
 *
 * The default MCUBoot configuration is to use swap mechanism. In case the flash
 * remapping functionality is supported by processor the alternative mechanism
 * using direct-xip mode can be used and evaluated by user.
 */
#define CONFIG_MCUBOOT_FLASH_REMAP_ENABLE

#define CONFIG_BOOT_BOOTSTRAP

/* Crypto Config */

#define CONFIG_BOOT_SIGNATURE
#define CONFIG_BOOT_USE_PSA_CRYPTO
#define CONFIG_BOOT_SIGNATURE_TYPE_ECDSA_P256

#endif /* CONFIG_BOOT_CUSTOM_DEVICE_SETUP */

#endif
