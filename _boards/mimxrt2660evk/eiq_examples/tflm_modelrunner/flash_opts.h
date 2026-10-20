/*
 * Copyright 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _FLASH_OPTS_H_
#define _FLASH_OPTS_H_

#include "fsl_xspi.h"
#include "app.h"
#include "board.h"

/* Dedicated PSRAM range for model storage, kept separate from .data/.bss. */
extern uint32_t __MODEL_FLASH_STORAGE_START;
extern uint32_t __MODEL_FLASH_STORAGE_SIZE;
#define FLASH_MODEL_ADDR 0x0U
#define FLASH_BASE_ADDR  ((uint32_t)&__MODEL_FLASH_STORAGE_START)
#define FLASH_MODEL_STORAGE_SIZE ((uint32_t)&__MODEL_FLASH_STORAGE_SIZE)


/*
 * FlashConfig is an xspi_config_t on RT2660 (kept for interface compatibility).
 * The PSRAM is mapped by the boot configuration and accessed as plain memory;
 * no xspi instance is actually driven by the modelrunner.
 */
typedef xspi_config_t FlashConfig;

#if defined(__cplusplus)
extern "C" {
#endif

status_t FlashInit(FlashConfig *config);
status_t FlashErase(FlashConfig *config, uint32_t start, uint32_t length);
status_t FlashProgram(FlashConfig *config, uint32_t start, uint32_t *src, uint32_t length);

#if defined(__cplusplus)
}
#endif

#endif /* _FLASH_OPTS_H_ */
