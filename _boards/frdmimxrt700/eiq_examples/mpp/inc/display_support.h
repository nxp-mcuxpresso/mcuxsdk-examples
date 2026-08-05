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

/* Panel selection: use the same macro names and values as the SDK display_support.h */
#define DEMO_PANEL_RASPI_7INCH 5 /* Raspberry Pi 7-inch DSI LCD, 800x480, TC358762 */

/* Set default panel if not already defined (e.g. by Kconfig or build system) */
#ifndef DEMO_PANEL
#define DEMO_PANEL DEMO_PANEL_RASPI_7INCH
#endif

/* Panel dimensions */
#if (DEMO_PANEL_RASPI_7INCH == DEMO_PANEL)
#define DEMO_PANEL_WIDTH  (800)
#define DEMO_PANEL_HEIGHT (480)
#endif

/* Address alignment helper (matches SDK display_support.h) */
#define DEMO_ALIGN_ADDR(addr, align) \
    ((((addr) / (align) * (align)) == (addr)) ? (addr) : ((addr) / (align) * (align) + (align)))

/* Frame buffer: use fixed PSRAM addresses so fbdev does not consume ncache SRAM */
#define DEMO_BUFFER_FIXED_ADDRESS 1
#define DEMO_BUFFER_COUNT         2
/* Frame buffers placed in PSRAM starting at 0x60000000 */
#define FRAME_BUFFER_ALIGN        64U
#define DEMO_BUFFER0_ADDR         DEMO_ALIGN_ADDR(0x60000000U, FRAME_BUFFER_ALIGN)
#define DEMO_BUFFER1_ADDR         DEMO_ALIGN_ADDR(0x60200000U, FRAME_BUFFER_ALIGN)

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

status_t BOARD_PrepareDisplayController(void);
void BOARD_DisplayTEPinHandler(void);

#if defined(__cplusplus)
}
#endif

#endif /* _DISPLAY_SUPPORT_H_ */
