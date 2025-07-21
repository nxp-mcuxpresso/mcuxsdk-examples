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

/* MCU SBL core */

#define CONFIG_MCUBOOT_MAX_IMG_SECTORS 800

#define CONFIG_BOOT_BOOTSTRAP

/* Crypto Config */

#define CONFIG_BOOT_SIGNATURE
#define CONFIG_BOOT_SIGNATURE_TYPE_ECDSA_P256
#define CONFIG_BOOT_USE_PSA_CRYPTO

#endif /* CONFIG_BOOT_CUSTOM_DEVICE_SETUP */

#endif
