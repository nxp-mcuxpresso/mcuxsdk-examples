/*
 * Copyright 2025, 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "dsp_support.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define RPMSG_LITE_LINK_ID_CM (RL_PLATFORM_IMXRT700_M33_0_M33_1_LINK_ID)
#define RPMSG_LITE_SHMEM_BASE_CM (void *)(0x20200000)
#if defined(CONFIG_RPMSG_LITE_PINGPONG_SWITCH_COMM) && (CONFIG_RPMSG_LITE_PINGPONG_SWITCH_COMM == 1)
#define RPMSG_LITE_LINK_ID_DSP (RL_PLATFORM_IMXRT700_M33_0_HIFI1_LINK_ID)
#define RPMSG_LITE_SHMEM_BASE_DSP (void *)(0x20588000)
/* Communication Address used in MCMGR start to send address to other Core
 * It start Hifi4 core, and it will send it different address based on CONFIG_RPMSG_LITE_PINGPONG_SWITCH_COMM
 * because Hifi4 will use one address to communicate with CM33_0 and other with CM33_1
 */
#define RPMSG_LITE_SHMEM_BASE_COMM (void *)(0x20200000 + 4096U)
/* Communication Core differs from boot core.
 * MCMGR uses this to send events to different cores based on CONFIG_RPMSG_LITE_PINGPONG_SWITCH_COMM
 * This CM33_0 core will either send events for Hifi4 or Hifi1
 */
#define APP_COMM_CORE kMCMGR_Core3
#else
#define RPMSG_LITE_LINK_ID_DSP (RL_PLATFORM_IMXRT700_M33_0_HIFI4_LINK_ID)
#define RPMSG_LITE_SHMEM_BASE_DSP (void *)(0x20200000 + 4096U)
#define RPMSG_LITE_SHMEM_BASE_COMM RPMSG_LITE_SHMEM_BASE_DSP
#define APP_COMM_CORE APP_DSP_CORE
#endif

#define APP_EVENT_SIZE (4)
#define APP_CM_CORE kMCMGR_Core1
#define APP_DSP_CORE kMCMGR_Core2

/* Address of memory, from which the secondary core will boot */
#define CORE1_BOOT_ADDRESS 0x20600000
#define APP_INVALIDATE_CACHE_FOR_SECONDARY_CORE_IMAGE_MEMORY

#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
extern uint32_t Image$$CORE1_REGION$$Base;
extern uint32_t Image$$CORE1_REGION$$Length;
#define CORE1_IMAGE_START &Image$$CORE1_REGION$$Base
#elif defined(__ICCARM__)
#pragma section = "__core1_image"
#define CORE1_IMAGE_START __section_begin("__core1_image")
#elif (defined(__GNUC__)) && (!defined(__MCUXPRESSO))
extern const char core1_image_start[];
extern const char *core1_image_end;
extern uint32_t core1_image_size;
#define CORE1_IMAGE_START ((void *)core1_image_start)
#define CORE1_IMAGE_SIZE  ((void *)core1_image_size)
#endif
/*${macro:end}*/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);

#ifdef CORE1_IMAGE_COPY_TO_RAM
uint32_t get_core1_image_size(void);
#endif /* CORE1_IMAGE_COPY_TO_RAM */

#ifdef APP_INVALIDATE_CACHE_FOR_SECONDARY_CORE_IMAGE_MEMORY
void invalidate_cache_for_core1_image_memory(uint32_t address, uint32_t size_byte);
#endif /* APP_INVALIDATE_CACHE_FOR_SECONDARY_CORE_IMAGE_MEMORY */
/*${prototype:end}*/

#endif /* _APP_H_ */
