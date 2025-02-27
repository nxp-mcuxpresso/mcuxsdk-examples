/*
 * Copyright 2023-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef EZHV_SUPPORT_H_
#define EZHV_SUPPORT_H_

/*******************************************************************************
 * DEFINITION
 ******************************************************************************/

#if defined(__CC_ARM)
extern uint32_t Image$$EZHV_REGION$$Base;
extern uint32_t Image$$EZHV_REGION$$Length;
#define ZENV_IMAGE_START &Image$$EZHV_REGION$$Base
#define EZHV_IMAGE_SIZE &Image$$EZHV_REGION$$Length
#elif defined(__ICCARM__)
#define ZENV_IMAGE_START (uint32_t)__section_begin("EZHV_section")
#define EZHV_IMAGE_SIZE \
    (int32_t) __section_end("EZHV_section") - (uint32_t)__section_begin("EZHV_section")
#elif (defined(__ARMCC_VERSION))
extern const char ezhv_image_start[];
extern const char ezhv_image_end[];
#define ZENV_IMAGE_START    ((uint32_t *)ezhv_image_start)
#define EZHV_IMAGE_SIZE     ((uint32_t)ezhv_image_end - (uint32_t)ezhv_image_start)
#elif defined(__GNUC__)
extern const char ezhv_image_start[];
extern int ezhv_image_size;
#define ZENV_IMAGE_START    ((uint32_t *)ezhv_image_start)
#define EZHV_IMAGE_SIZE     ((int32_t)ezhv_image_size)
#endif
/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

void BOARD_EZHV_Init(void *para, uint32_t size);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif /* _EZHV_SUPPORT_H_ */
