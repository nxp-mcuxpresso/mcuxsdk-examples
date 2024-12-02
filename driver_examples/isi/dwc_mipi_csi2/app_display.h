/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef APP_DISPLAY_H_
#define APP_DISPLAY_H_

#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*
 * The callback function is called when the display controller updated the use
 * the new frame buffer. The previous active frame buffer address is returned
 * so that it could be submited to the camera buffer queue.
 */
typedef void (*app_display_callback_t)(uint32_t frameBuffer);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif
/*
 * Init and start the display controller with the init frame buffer.
 */
void APP_InitDisplay(uint32_t frameBuffer, app_display_callback_t callback);

/*
 * Check whether the new setting frame buffer is still pending.
 */
bool APP_IsDisplayFramePending(void);

/*
 * Set the new frame buffer to display controller.
 */
void APP_SetDisplayFrameBuffer(uint32_t frameBuffer);

/*
 * Display controller IRQ handler that should be installed into the vector table.
 */
void APP_DisplayIRQHandler(void);

#if defined(__cplusplus)
}
#endif

#endif /* _APP_DISPLAY_H_ */
