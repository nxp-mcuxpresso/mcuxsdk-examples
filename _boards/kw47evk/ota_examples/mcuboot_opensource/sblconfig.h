/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef SBL_CONFIG_H__
#define SBL_CONFIG_H__

/* KW47 has flash with minimal write size 16 bytes */
#define MCUBOOT_BOOT_MAX_ALIGN 16

/*******************************************************************/
/* Use default configuration if setup from Kconfig is not provided */
/*******************************************************************/
#ifndef CONFIG_BOOT_CUSTOM_DEVICE_SETUP

/* To be able to bootstrap signed image from debug session */
#define CONFIG_BOOT_BOOTSTRAP

#define CONFIG_MCUBOOT_MAX_IMG_SECTORS 256

#define CONFIG_BOOT_SIGNATURE
#define CONFIG_BOOT_SIGNATURE_TYPE_ECDSA_P256
#define CONFIG_BOOT_USE_PSA_CRYPTO

#endif /* CONFIG_BOOT_CUSTOM_DEVICE_SETUP */

#endif