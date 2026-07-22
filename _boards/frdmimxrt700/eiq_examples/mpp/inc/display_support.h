/*
 * Copyright 2024-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _DISPLAY_SUPPORT_H_
#define _DISPLAY_SUPPORT_H_

#include "fsl_dc_fb.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* @TEST_ANCHOR */

/* Panel selection */
#define DEMO_PANEL_WAVESHARE_5INCH_DSI 0 /* Waveshare 5inch DSI LCD, 800x480, ILI6122 */
#define DEMO_PANEL_RM67162             1 /* RM67162 circular smart panel */

/* Override any SDK Kconfig-generated DEMO_PANEL value — MPP uses its own panel IDs */
#ifdef DEMO_PANEL
#undef DEMO_PANEL
#endif
#define DEMO_PANEL DEMO_PANEL_WAVESHARE_5INCH_DSI

/* Panel dimensions */
#if (DEMO_PANEL_WAVESHARE_5INCH_DSI == DEMO_PANEL)
#define DEMO_PANEL_WIDTH  (800)
#define DEMO_PANEL_HEIGHT (480)
#elif (DEMO_PANEL_RM67162 == DEMO_PANEL)
#define DEMO_PANEL_WIDTH  (400)
#define DEMO_PANEL_HEIGHT (400)
#endif

/* Frame buffer */
#define DEMO_BUFFER_COUNT 1

#ifndef DEMO_USE_XRGB8888
#define DEMO_USE_XRGB8888 0
#endif

#if DEMO_USE_XRGB8888
#define DEMO_BUFFER_PIXEL_FORMAT   kVIDEO_PixelFormatXRGB8888
#define DEMO_BUFFER_BYTE_PER_PIXEL 4
#else
#define DEMO_BUFFER_PIXEL_FORMAT   kVIDEO_PixelFormatRGB565
#define DEMO_BUFFER_BYTE_PER_PIXEL 2
#endif

#define DEMO_BUFFER_WIDTH  DEMO_PANEL_WIDTH
#define DEMO_BUFFER_HEIGHT DEMO_PANEL_HEIGHT

/* Where the frame buffer is shown in the screen. */
#define DEMO_BUFFER_START_X 0U
#define DEMO_BUFFER_START_Y 0U

#define DEMO_BUFFER_STRIDE_BYTE (DEMO_BUFFER_WIDTH * DEMO_BUFFER_BYTE_PER_PIXEL)

/* Frame buffer alignment: 64-byte for cache line */
#define FRAME_BUFFER_ALIGN 64

#define DEMO_BUFFER_FIXED_ADDRESS 0

/*
 * MIPI DSI board pins — these are defined in the SDK board.h.
 * Only define here if not already provided by board.h.
 */
#ifndef BOARD_MIPI_RST_GPIO
#define BOARD_MIPI_RST_GPIO   GPIO3
#endif
#ifndef BOARD_MIPI_RST_PIN
#define BOARD_MIPI_RST_PIN    4
#endif
#ifndef BOARD_MIPI_POWER_GPIO
#define BOARD_MIPI_POWER_GPIO GPIO1
#endif
#ifndef BOARD_MIPI_POWER_PIN
#define BOARD_MIPI_POWER_PIN  10
#endif
/* Alias for code using shorter PWR naming */
#define BOARD_MIPI_PWR_GPIO   BOARD_MIPI_POWER_GPIO
#define BOARD_MIPI_PWR_PIN    BOARD_MIPI_POWER_PIN
#ifndef BOARD_MIPI_BL_GPIO
#define BOARD_MIPI_BL_GPIO    GPIO1
#endif
#ifndef BOARD_MIPI_BL_PIN
#define BOARD_MIPI_BL_PIN     14
#endif
#ifndef BOARD_MIPI_TE_GPIO
#define BOARD_MIPI_TE_GPIO    GPIO3
#endif
#ifndef BOARD_MIPI_TE_PIN
#define BOARD_MIPI_TE_PIN     5
#endif

/* Touch I2C: LPI2C8 */
#ifndef BOARD_TOUCH_I2C
#define BOARD_TOUCH_I2C       LPI2C8
#endif
#define BOARD_TOUCH_I2C_ADDR  0x5DU  /* GT911 default address */

extern const dc_fb_t g_dc;

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

void BOARD_PrepareDisplayController(void);
void BOARD_DisplayTEPinHandler(void);

#if defined(__cplusplus)
}
#endif

#endif /* _DISPLAY_SUPPORT_H_ */
