/*
 * Copyright 2025 NXP
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

/* HW flash remap feature is used if MCUBoot is located in IFR region */
#define CONFIG_MCUBOOT_FLASH_REMAP_ENABLE

/* MCUBoot Flash Config */

#define CONFIG_MCUBOOT_MAX_IMG_SECTORS 64

#define CONFIG_BOOT_BOOTSTRAP

/* Crypto Config */

#define CONFIG_BOOT_SIGNATURE
#define CONFIG_BOOT_SIGNATURE_TYPE_ECDSA_P256

/*
 * MCUBoot is located in IFR region -> Use TinyCrypt
 */
#define CONFIG_BOOT_USE_TINYCRYPT

#endif /* CONFIG_BOOT_CUSTOM_DEVICE_SETUP */

/* Config Guards */
#if defined(CONFIG_ENCRYPT_XIP_EXT_ENABLE) && \
    !defined(CONFIG_MCXN_CUSTOM_CFG_MAIN_FLASH_ONLY)
#error "Encrypted XIP using NPX is not supported when MCUBoot is placed in IFR region."
#endif
#if defined(CONFIG_MCUBOOT_FLASH_REMAP_ENABLE) && \
    defined(CONFIG_MCXN_CUSTOM_CFG_MAIN_FLASH_ONLY)
#error "Flash remap is not supported when MCUBoot is placed in main flash region."
#endif

#endif /* SBL_CONFIG_H__ */
