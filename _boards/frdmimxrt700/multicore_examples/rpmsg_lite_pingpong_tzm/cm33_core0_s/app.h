/*
 * Copyright 2025, 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define RPMSG_LITE_LINK_ID (RL_PLATFORM_IMXRT700_M33_0_M33_1_LINK_ID)

#define NON_SECURE_START 0x08100000

/* Address of memory, from which the secondary core will boot */
#define CORE1_BOOT_ADDRESS 0x30600000
#define CORE1_NS_BOOT_ADDRESS 0x30680000

#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
extern uint32_t Image$$CORE1_REGION$$Base;
extern uint32_t Image$$CORE1_REGION$$Length;
#define CORE1_IMAGE_START &Image$$CORE1_REGION$$Base
extern uint32_t Image$$CORE1_NS_REGION$$Base;
extern uint32_t Image$$CORE1_NS_REGION$$Length;
#define CORE1_NS_IMAGE_START &Image$$CORE1_NS_REGION$$Base
#elif defined(__ICCARM__)
#pragma section = "__core1_image"
#define CORE1_IMAGE_START __section_begin("__core1_image")
#pragma section = "__core1_ns_image"
#define CORE1_NS_IMAGE_START __section_begin("__core1_ns_image")
#elif (defined(__GNUC__)) && (!defined(__MCUXPRESSO))
extern const char core1_image_start[];
extern const char *core1_image_end;
extern uint32_t core1_image_size;
#define CORE1_IMAGE_START ((void *)core1_image_start)
#define CORE1_IMAGE_SIZE  ((void *)core1_image_size)
extern const char core1_ns_image_start[];
extern const char *core1_ns_image_end;
extern uint32_t core1_ns_image_size;
#define CORE1_NS_IMAGE_START ((void *)core1_ns_image_start)
#define CORE1_NS_IMAGE_SIZE  ((void *)core1_ns_image_size)
#endif

/*${macro:end}*/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);

#ifdef CORE1_IMAGE_COPY_TO_RAM
uint32_t get_core1_image_size(void);
uint32_t get_core1_ns_image_size(void);
#endif /* CORE1_IMAGE_COPY_TO_RAM */

#ifdef APP_INVALIDATE_CACHE_FOR_SECONDARY_CORE_IMAGE_MEMORY
void invalidate_cache_for_core1_image_memory(uint32_t address, uint32_t size_byte);
#endif /* APP_INVALIDATE_CACHE_FOR_SECONDARY_CORE_IMAGE_MEMORY */
/*${prototype:end}*/

#endif /* _APP_H_ */
