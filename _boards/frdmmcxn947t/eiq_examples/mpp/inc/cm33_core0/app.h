/*
 * Copyright 2020, 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _BOARD_INIT_H_
#define _BOARD_INIT_H_

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

#ifdef MCMGR_USED
#include "mcmgr.h"
#endif /* MCMGR_USED */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#ifdef RPMSG_USED
#define RPMSG_LITE_LINK_ID (RL_PLATFORM_MCXNX4X_M33_M33_LINK_ID)
#endif /* RPMSG_USED */

#ifdef BOOT_SECONDARY_CORE
/* Address of memory, from which the secondary core will boot */
#ifndef CORE1_BOOT_ADDRESS
#define CORE1_BOOT_ADDRESS 0x20030000
#endif

#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
extern uint32_t Image$$CORE1_REGION$$Base;
extern uint32_t Image$$CORE1_REGION$$Length;
#define CORE1_IMAGE_START &Image$$CORE1_REGION$$Base
#elif defined(__ICCARM__)
extern unsigned char core1_image_start[];
#define CORE1_IMAGE_START core1_image_start
#elif (defined(__GNUC__)) && (!defined(__MCUXPRESSO))
extern const char core1_image_start[];
extern const char *core1_image_end;
extern uint32_t core1_image_size;
#define CORE1_IMAGE_START ((void *)core1_image_start)
#define CORE1_IMAGE_SIZE  ((void *)core1_image_size)
#endif
#endif /* BOOT_SECONDARY_CORE */

void BOARD_Init();

#ifdef CORE1_IMAGE_COPY_TO_RAM
uint32_t get_core1_image_size(void);
#endif /* CORE1_IMAGE_COPY_TO_RAM */

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _BOARD_INIT_H_ */
