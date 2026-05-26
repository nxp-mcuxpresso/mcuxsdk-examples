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
#define DEMO_PANEL_RASPI_7INCH 5 /* Raspberry Pi panel 7 inch */
// #define DEMO_PANEL_CO5300      6 /* NXP ZC143AC72MIPI MIPI Circular Display */

/* @TEST_ANCHOR */
/* Configure this macro in Kconfig or directly in the generated mcux_config.h. */
#ifndef DEMO_PANEL
#define DEMO_PANEL DEMO_PANEL_RASPI_7INCH
#endif

#define DEMO_ALIGN_ADDR(addr, align) ((((addr) / (align) * (align)) == (addr)) ? (addr) : ((addr) / (align) * (align) + (align)))

#if (DEMO_PANEL_RASPI_7INCH == DEMO_PANEL)

#define DEMO_BUFFER_FIXED_ADDRESS 1
/*
 * To get best performance, frame buffer should be in dedicate SRAM partition.
 * But due to the high resolution of MIPI panel and limitted SRAM size, here we put
 * frame buffer in on-board PSRAM.
 */

/*
 * Use the MIPI dumb panel
 */

/* Definitions for the frame buffer. */
#define DEMO_BUFFER_COUNT         2   /* 2 is enough for DPI interface display. */
#define FRAME_BUFFER_ALIGN        64U  /* For RGB565 or XRGB8888, LCDIF and VGLite alignment requirement */

#define DEMO_BUFFER0_ADDR DEMO_ALIGN_ADDR(0x60000000U, FRAME_BUFFER_ALIGN)
#define DEMO_BUFFER1_ADDR DEMO_ALIGN_ADDR(0x60200000U, FRAME_BUFFER_ALIGN)

#define DEMO_BUFFER_PIXEL_FORMAT   kVIDEO_PixelFormatRGB565
#define DEMO_BUFFER_BYTE_PER_PIXEL 2

#define DEMO_PANEL_WIDTH  (800)
#define DEMO_PANEL_HEIGHT (480)

#define DEMO_FB_WIDTH   DEMO_PANEL_WIDTH
#define DEMO_FB_HEIGHT  DEMO_PANEL_HEIGHT
#define DEMO_BUFFER_WIDTH   DEMO_FB_WIDTH
#define DEMO_BUFFER_HEIGHT  DEMO_FB_HEIGHT

/* Where the frame buffer is shown in the screen. */
#define DEMO_BUFFER_START_X 0U
#define DEMO_BUFFER_START_Y 0U

#define DEMO_BUFFER_STRIDE_BYTE DEMO_ALIGN_ADDR((DEMO_FB_WIDTH * DEMO_BUFFER_BYTE_PER_PIXEL), 64U)

#elif (DEMO_PANEL_CO5300 == DEMO_PANEL)

/* Pixel format macro mapping. */
#define DEMO_CO5300_BUFFER_RGB565   0
#define DEMO_CO5300_BUFFER_RGB888   1

#ifndef DEMO_CO5300_BUFFER_FORMAT
#define DEMO_CO5300_BUFFER_FORMAT DEMO_CO5300_BUFFER_RGB565
#endif

#if (DEMO_CO5300_BUFFER_FORMAT == DEMO_CO5300_BUFFER_RGB565)

#define DEMO_BUFFER_PIXEL_FORMAT   kVIDEO_PixelFormatRGB565
#define DEMO_BUFFER_BYTE_PER_PIXEL 2
#define FRAME_BUFFER_ALIGN         64U   /* LCDIF and VGLite are considered. */

#elif (DEMO_CO5300_BUFFER_FORMAT == DEMO_CO5300_BUFFER_RGB888)

#define DEMO_BUFFER_PIXEL_FORMAT   kVIDEO_PixelFormatRGB888
#define DEMO_BUFFER_BYTE_PER_PIXEL 3
#define FRAME_BUFFER_ALIGN         192U   /* LCDIF and VGLite are considered. */

#endif /* DEMO_CO5300_BUFFER_FORMAT */

/* Use fixed address to place buffer on PSRAM. */
#define DEMO_BUFFER_FIXED_ADDRESS 1

/*
 * Place frame buffer in on-board PSRAM.
 */
#define DEMO_BUFFER0_ADDR DEMO_ALIGN_ADDR(0x60000000U, FRAME_BUFFER_ALIGN)
#define DEMO_BUFFER1_ADDR DEMO_ALIGN_ADDR(0x60200000U, FRAME_BUFFER_ALIGN)

/* Definitions for the frame buffer. */
/* 1 is enough, use 2 could render background buffer while display the foreground buffer. */
#define DEMO_BUFFER_COUNT  2

#define DEMO_PANEL_WIDTH  (480U)
#define DEMO_PANEL_HEIGHT (466U)

#define DEMO_FB_WIDTH   466U
#define DEMO_FB_HEIGHT  466U /* Circular panel, only 466x466 pixels are shown. */
#define DEMO_BUFFER_WIDTH   DEMO_FB_WIDTH
#define DEMO_BUFFER_HEIGHT  DEMO_FB_HEIGHT

/* Where the frame buffer is shown in the screen. */
#define DEMO_BUFFER_START_X 6U /* The hardware actually starts to show from pixel 7. */
#define DEMO_BUFFER_START_Y 0U

#if (DEMO_CO5300_BUFFER_FORMAT == DEMO_CO5300_BUFFER_RGB565)
#define DEMO_BUFFER_STRIDE_BYTE DEMO_ALIGN_ADDR((DEMO_FB_WIDTH * DEMO_BUFFER_BYTE_PER_PIXEL), 64U)
#else
/* For RGB888 format, the stride shall also be divisible by 3. */
#define DEMO_BUFFER_STRIDE_BYTE DEMO_ALIGN_ADDR((DEMO_FB_WIDTH * DEMO_BUFFER_BYTE_PER_PIXEL), (64U * 3U))
#endif

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
