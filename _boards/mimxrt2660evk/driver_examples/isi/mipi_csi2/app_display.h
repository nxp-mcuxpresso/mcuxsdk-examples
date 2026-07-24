/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_DISPLAY_H_
#define _APP_DISPLAY_H_

#include "fsl_common.h"

typedef void (*app_display_callback_t)(uint32_t frameBuffer);

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Initialize the display controller and submit the first frame buffer.
 *
 * @param frameBuffer  Initial frame buffer address.
 * @param callback     Called when a frame buffer is released.
 */
void APP_InitDisplay(uint32_t frameBuffer, app_display_callback_t callback);

bool APP_IsDisplayFramePending(void);

/*!
 * @brief Submit a new frame buffer to the display.
 *
 * @param frameBuffer  Frame buffer address to display next.
 */
void APP_SetDisplayFrameBuffer(uint32_t frameBuffer);

#if defined(__cplusplus)
}
#endif

#endif /* _APP_DISPLAY_H_ */
