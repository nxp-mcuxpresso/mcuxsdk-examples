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

/* @TEST_ANCHOR */

#define DEMO_PANEL_RK055AHD091 0 /* 720 * 1280, RK055AHD091-CTG(RK055HDMIPI4M) */
#define DEMO_PANEL_RK055IQH091 1 /* 540 * 960,  RK055IQH091-CTG */
#define DEMO_PANEL_RK055MHD091 2 /* 720 * 1280, RK055MHD091A0-CTG(RK055HDMIPI4MA0) */
#define DEMO_PANEL_RASPI_7INCH 5 /* 800 * 480, Raspberry Pi 7" */

#define DEMO_DISPLAY_CONTROLLER_ELCDIF  0
#define DEMO_DISPLAY_CONTROLLER_LCDIFV2 1

/* Configure this macro in Kconfig or directly in the generated mcux_config.h. */
#ifndef DEMO_PANEL
/*
 * FRDM-IMXRT1152 panel selection.
 * Hardware kit -> panel/driver-IC mapping (per MCUXpresso SDK display_support.h):
 *   RK055HDMIPI4M   (legacy)  = RK055AHD091-CTG   -> RM68200 -> DEMO_PANEL_RK055AHD091 (0)
 *   RK055HDMIPI4MA0 (current) = RK055MHD091A0-CTG -> HX8394  -> DEMO_PANEL_RK055MHD091 (2)
 * This board uses the current RK055HDMIPI4MA0, so default to RK055MHD091 (HX8394).
 *
 * Verified against FRDM-1150 schematic SCH-96440 (MIPI LCD page 17):
 *  - MIPI connector J107 (40-pin FPC) routes 2 DSI data lanes -> DEMO_MIPI_DSI_LANE_NUM=2 (already set).
 *  - Panel control via PCAL6524 (LPI2C4 @0x22): LCD_RST_B=P0.3, Backlight_CTL=P0.2, LCM_PWR_EN=P1.3.
 *  - Touch on LPI2C1 (CTP_SDA/SCL), CTP_RST_B=P1.4, CTP_INT=P2.3.
 * All of the above already match board.h/display_support.c, so selecting the panel is sufficient.
 *
 * Honor the CONFIG_* symbols emitted by build_mpp.py (-p <panel> -> -DCONFIG_<name>=y).
 */
#if defined(CONFIG_RK055AHD091) && (CONFIG_RK055AHD091 == 1)
#define DEMO_PANEL DEMO_PANEL_RK055AHD091
#elif defined(CONFIG_RK055IQH091) && (CONFIG_RK055IQH091 == 1)
#define DEMO_PANEL DEMO_PANEL_RK055IQH091
#elif defined(CONFIG_RASPI_7INCH) && (CONFIG_RASPI_7INCH == 1)
#define DEMO_PANEL DEMO_PANEL_RASPI_7INCH
#else /* CONFIG_RK055MHD091A0 or unset -> current RK055HDMIPI4MA0 kit */
#define DEMO_PANEL DEMO_PANEL_RK055MHD091
#endif
#endif

/* Configure this macro in Kconfig or directly in the generated mcux_config.h. */
#ifndef DEMO_DISPLAY_CONTROLLER
/* Use LCDIFV2 by default, could use ELCDIF by changing this macro. */
#define DEMO_DISPLAY_CONTROLLER DEMO_DISPLAY_CONTROLLER_LCDIFV2
#endif

#define DEMO_BUFFER_FIXED_ADDRESS 0

#if DEMO_BUFFER_FIXED_ADDRESS
#define DEMO_BUFFER0_ADDR 0x80000000
#define DEMO_BUFFER1_ADDR 0x80200000
#endif

/*
 * Use the MIPI dumb panel
 */

/* Definitions for the frame buffer. */
#define DEMO_BUFFER_COUNT 2 /* 2 is enough for DPI interface display. */

/* Don't change here, change in mcux_config.h */
#ifndef DEMO_USE_XRGB8888
#define DEMO_USE_XRGB8888 0
#endif

/* Use LCDIF LUT (or named color palette) which is 8-bit per-pixel */
/* Don't change here, change in mcux_config.h */
#ifndef DEMO_USE_LUT8
#define DEMO_USE_LUT8 0
#endif

#if DEMO_USE_XRGB8888
#define DEMO_BUFFER_PIXEL_FORMAT   kVIDEO_PixelFormatXRGB8888
#define DEMO_BUFFER_BYTE_PER_PIXEL 4
#elif DEMO_USE_LUT8
#define DEMO_BUFFER_PIXEL_FORMAT   kVIDEO_PixelFormatLUT8
#define DEMO_BUFFER_BYTE_PER_PIXEL 1
#else
#define DEMO_BUFFER_PIXEL_FORMAT   kVIDEO_PixelFormatRGB565
#define DEMO_BUFFER_BYTE_PER_PIXEL 2
#endif

#if ((DEMO_PANEL_RK055AHD091 == DEMO_PANEL) || (DEMO_PANEL_RK055MHD091 == DEMO_PANEL))

#define DEMO_PANEL_WIDTH  (720)
#define DEMO_PANEL_HEIGHT (1280)

#elif (DEMO_PANEL_RK055IQH091 == DEMO_PANEL)

#define DEMO_PANEL_WIDTH  (540)
#define DEMO_PANEL_HEIGHT (960)

#elif (DEMO_PANEL_RASPI_7INCH == DEMO_PANEL)

#define DEMO_PANEL_WIDTH  (800)
#define DEMO_PANEL_HEIGHT (480)

#endif

#define DEMO_FB_WIDTH  DEMO_PANEL_WIDTH
#define DEMO_FB_HEIGHT DEMO_PANEL_HEIGHT
#define DEMO_BUFFER_WIDTH  DEMO_FB_WIDTH
#define DEMO_BUFFER_HEIGHT DEMO_FB_HEIGHT

/* Where the frame buffer is shown in the screen. */
#define DEMO_BUFFER_START_X 0U
#define DEMO_BUFFER_START_Y 0U

#define DEMO_FB_STRIDE(x) ((x * DEMO_BUFFER_BYTE_PER_PIXEL + FRAME_BUFFER_ALIGN - 1) & ~(FRAME_BUFFER_ALIGN - 1))
#define DEMO_BUFFER_STRIDE_BYTE DEMO_FB_STRIDE(DEMO_FB_WIDTH)

/* There is not frame buffer aligned requirement, consider the 64-bit AXI data
 * bus width and 32-byte cache line size, the frame buffer alignment is set to
 * 32 byte.
 */
#define FRAME_BUFFER_ALIGN 32

extern const dc_fb_t g_dc;

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

status_t BOARD_PrepareDisplayController(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _DISPLAY_SUPPORT_H_ */
