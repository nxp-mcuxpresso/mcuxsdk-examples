/*
 * Copyright 2023-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LVGL_SUPPORT_H
#define LVGL_SUPPORT_H

#include <stdint.h>
#include "lvgl_support_board.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Don't modify here, modify in file mcux_config.h */
#ifndef BOARD_USE_FLEXIO_SMARTDMA
#define BOARD_USE_FLEXIO_SMARTDMA 0
#endif

#define LCD_WIDTH  480
#define LCD_HEIGHT 320

#define LCD_FB_BYTE_PER_PIXEL 2
/* The virtual buffer for DBI panel, it should be ~1/10 screen size. */
#define LCD_VIRTUAL_BUF_HEIGHT (LCD_HEIGHT / 10)
#define LCD_VIRTUAL_BUF_SIZE   (LCD_WIDTH * LCD_VIRTUAL_BUF_HEIGHT)

/*******************************************************************************
 * API
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

void lv_port_pre_init(void);
void lv_port_disp_init(void);
void lv_port_indev_init(void);

void BOARD_TouchIntHandler(void);

#if defined(__cplusplus)
}
#endif

#endif /*LVGL_SUPPORT_H */
