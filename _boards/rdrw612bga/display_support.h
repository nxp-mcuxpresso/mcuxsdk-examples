/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _DISPLAY_SUPPORT_H_
#define _DISPLAY_SUPPORT_H_

#include "fsl_dc_fb.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_PANEL_ADAFRUIT_2_8        0 /* Adafruit 2.8" captative panel */
#define DEMO_PANEL_LCD_PAR_S035        1 /* LCD_PAR_S035 panel 8080.*/

/* @TEST_ANCHOR */

/* Configure this macro in Kconfig or directly in the generated mcux_config.h. */
#ifndef DEMO_PANEL
#define DEMO_PANEL DEMO_PANEL_LCD_PAR_S035
#endif

#if (DEMO_PANEL == DEMO_PANEL_LCD_PAR_S035)
#define DEMO_PANEL_WIDTH             480
#define DEMO_PANEL_HEIGHT            320
#else
#define DEMO_PANEL_WIDTH             320
#define DEMO_PANEL_HEIGHT            240
#endif

/* If the panel is DPI type, define it to 0. */
#define DEMO_PANEL_TYPE_DBI          1

#define DEMO_BUFFER_FIXED_ADDRESS 0

/* Where the frame buffer is shown in the screen. */
#define DEMO_BUFFER_WIDTH   DEMO_PANEL_WIDTH
#define DEMO_BUFFER_HEIGHT  DEMO_PANEL_HEIGHT
#define DEMO_BUFFER_START_X 0U
#define DEMO_BUFFER_START_Y 0U

#define DEMO_BUFFER_PIXEL_FORMAT   kVIDEO_PixelFormatRGB565
#define DEMO_BUFFER_BYTE_PER_PIXEL 2u

#define DEMO_BUFFER_STRIDE_BYTE (DEMO_BUFFER_WIDTH * DEMO_BUFFER_BYTE_PER_PIXEL)

extern const dc_fb_t g_dc;

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

status_t BOARD_PrepareDisplayController(void);
status_t BOARD_PrepareTouchPanel(void);

status_t BOARD_InitTouchPanel(void);
status_t BOARD_GetTouchPanelPoint(int *x, int *y);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _DISPLAY_SUPPORT_H_ */
