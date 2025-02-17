/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _DISPLAY_SUPPORT_H_
#define _DISPLAY_SUPPORT_H_

#include "fsl_dc_fb.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define DEMO_DCIF            DCIF
#define DEMO_DCIF_IRQ        DCIF_CPU_DOMAIN2_IRQn
#define DEMO_DCIF_IRQHandler DCIF_CPU_DOMAIN2_IRQHandler

#define LCD_SPEC             1 /* 800 * 1280 */

#ifndef DEMO_PANEL
#define DEMO_PANEL           LCD_SPEC
#endif

#if (DEMO_PANEL == LCD_SPEC)
/*
 * The pixel clock is (height + VSW + VFP + VBP) * (width + HSW + HFP + HBP) * frame rate.
 * (800 + 6 + 2 + 15) * (1280 + 32 + 48 + 80) * x 60 = 823 * 1440 * x 60Hz = 71107200 Hz
 */
/* 1280*800 LVDS pannel */
#define DEMO_PANEL_HEIGHT    800
#define DEMO_PANEL_WIDTH     1280
#define DEMO_BUFFER_HEIGHT   800
#define DEMO_BUFFER_WIDTH    1280
#define DEMO_HSW             32
#define DEMO_HFP             48
#define DEMO_HBP             80
#define DEMO_VSW             6
#define DEMO_VFP             2
#define DEMO_VBP             15
#endif

#define DEMO_POL_FLAGS \
    (kDCIF_DataEnableActiveHigh | kDCIF_VsyncActiveLow | kDCIF_HsyncActiveLow | kDCIF_DriveDataOnFallingClkEdge)

/* Frame buffer must be 32 byte aligned. */
#define FRAME_BUFFER_ALIGN 32

#define DEMO_BUFFER_START_X (0U)
#define DEMO_BUFFER_START_Y (0U)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void BOARD_PrepareDisplayController(void);

extern const dc_fb_t g_dc;
#endif /* _DISPLAY_SUPPORT_H_ */