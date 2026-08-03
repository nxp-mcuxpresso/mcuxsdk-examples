/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LVGL_SUPPORT_H
#define LVGL_SUPPORT_H

#include <stdint.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* LCD panel resolution for LCD-PAR-S035 (ST7796S, landscape 480x320). */
#define LCD_WIDTH             480U
#define LCD_HEIGHT            320U
#define LCD_FB_BYTE_PER_PIXEL 2U

/* Virtual buffer height: ~1/10 of screen height. */
#define LCD_VIRTUAL_BUF_HEIGHT (LCD_HEIGHT / 10U)
/* Total pixels in one virtual buffer row stripe. */
#define LCD_VIRTUAL_BUF_SIZE   (LCD_WIDTH * LCD_VIRTUAL_BUF_HEIGHT)

/*******************************************************************************
 * API
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

void lv_port_disp_init(void);
void lv_port_indev_init(void);

/*
 * Called from the touch GPIO interrupt handler (PINT or GPIO IRQ).
 * Sets an internal pending flag so the LVGL read callback can poll GT911.
 */
void BOARD_TouchIntHandler(void);

#if defined(__cplusplus)
}
#endif

#endif /* LVGL_SUPPORT_H */
