/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _DISPLAY_SUPPORT_H_
#define _DISPLAY_SUPPORT_H_

#include "fsl_dc_fb.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_PANEL_LCM_RGB_5INCH 19 /* NXP LCM_RGB_5INCH-CT DPI Display */
#define DEMO_PANEL_LCD_PAR_S035  8  /* LCD_PAR_S035 panel */
#define DEMO_PANEL_RK055MHD091   2  /* 720 * 1280, RK055MHD091A0-CTG(RK055HDMIPI4MA0) */

/* @TEST_ANCHOR */
/* Configure this macro in Kconfig or directly in the generated mcux_config.h. */
#ifndef DEMO_PANEL
#define DEMO_PANEL DEMO_PANEL_LCM_RGB_5INCH
#endif

#define DEMO_FRAME_RATE 50U
#define FRAME_BUFFER_ALIGN 16

/* Pixel format macro mapping. */
#define DEMO_BUFFER_RGB565   0
#define DEMO_BUFFER_RGB888   1
#define DEMO_BUFFER_ARGB8888 2

#ifndef DEMO_BUFFER_FORMAT
#define DEMO_BUFFER_FORMAT DEMO_BUFFER_RGB565
#endif

#if (DEMO_BUFFER_FORMAT == DEMO_BUFFER_RGB565)
#define DEMO_BUFFER_PIXEL_FORMAT   kVIDEO_PixelFormatRGB565
#define DEMO_BUFFER_BYTE_PER_PIXEL 2
#elif (DEMO_BUFFER_FORMAT == DEMO_BUFFER_RGB888)
#define DEMO_BUFFER_PIXEL_FORMAT   kVIDEO_PixelFormatRGB888
#define DEMO_BUFFER_BYTE_PER_PIXEL 3
#else
#define DEMO_BUFFER_PIXEL_FORMAT   kVIDEO_PixelFormatARGB8888
#define DEMO_BUFFER_BYTE_PER_PIXEL 4
#endif

/* Buffer alignment requirement. */
#define DEMO_ALIGN_ADDR(addr, align) \
        ((((addr) / (align) * (align)) == (addr)) ? (addr) : ((addr) / (align) * (align) + (align)))
#define DEMO_BUFFER_STRIDE_BYTE      \
        DEMO_ALIGN_ADDR((DEMO_PANEL_WIDTH * DEMO_BUFFER_BYTE_PER_PIXEL), FRAME_BUFFER_ALIGN)

#if (DEMO_PANEL == DEMO_PANEL_LCD_PAR_S035)

#define DEMO_BUFFER_COUNT  1 /* 1 is enough for DBI interface display. */

/* For RGB565/RGB888/ARGB8888 the frame buffer size is 0x4b000/0x70800/0x96000   */
#if DEMO_BUFFER_FIXED_ADDRESS
#define DEMO_BUFFER0_ADDR 0x89010000U
#endif

#define DEMO_PANEL_WIDTH  480U
#define DEMO_PANEL_HEIGHT 320U
#define DEMO_BUFFER_WIDTH  DEMO_PANEL_WIDTH
#define DEMO_BUFFER_HEIGHT DEMO_PANEL_HEIGHT

/* Where the frame buffer is shown in the screen. */
#define DEMO_BUFFER_START_X 0U
#define DEMO_BUFFER_START_Y 0U

#elif (DEMO_PANEL == DEMO_PANEL_RK055MHD091)

#define DEMO_BUFFER_COUNT 2   /* 2 is enough for DPI interface display. */

/* For RGB565/RGB888/ARGB8888 the frame buffer size is 0x1C2000/0x2A3000/0x384000 */
#if DEMO_BUFFER_FIXED_ADDRESS
#define DEMO_BUFFER0_ADDR 0x89010000U
#define DEMO_BUFFER1_ADDR 0x89410000U
#endif

#define DEMO_PANEL_WIDTH   720
#define DEMO_PANEL_HEIGHT  1280
#define DEMO_BUFFER_WIDTH  DEMO_PANEL_WIDTH
#define DEMO_BUFFER_HEIGHT DEMO_PANEL_HEIGHT

/* Where the frame buffer is shown in the screen. */
#define DEMO_BUFFER_START_X 0U
#define DEMO_BUFFER_START_Y 0U

#elif (DEMO_PANEL == DEMO_PANEL_LCM_RGB_5INCH)

#define DEMO_BUFFER_COUNT 2  /* 2 is enough for DPI interface display. */

/* For RGB565/RGB888/ARGB8888 the frame buffer size is 0xBB800/0x119400/0x177000 */
#if DEMO_BUFFER_FIXED_ADDRESS
#define DEMO_BUFFER0_ADDR 0x89010000U
#define DEMO_BUFFER1_ADDR 0x89190000U
#endif

#define DEMO_PANEL_WIDTH   800U
#define DEMO_PANEL_HEIGHT  480U
#define DEMO_BUFFER_WIDTH  DEMO_PANEL_WIDTH
#define DEMO_BUFFER_HEIGHT DEMO_PANEL_HEIGHT

/* Where the frame buffer is shown in the screen. */
#define DEMO_BUFFER_START_X 0U
#define DEMO_BUFFER_START_Y 0U

#endif

extern const dc_fb_t g_dc;

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

status_t BOARD_PrepareDisplayController(void);
void BOARD_DisplayTEPinHandler(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _DISPLAY_SUPPORT_H_ */
