/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _LCDIF_SUPPORT_H_
#define _LCDIF_SUPPORT_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define DEMO_LCDIF            LCDIF
#define DEMO_LCDIF_IRQn       LCDIF_IRQn
#define DEMO_LCDIF_IRQHandler LCDIF_IRQHandler

#define DEMO_FB0_ADDR 0x60000000U
#define DEMO_FB1_ADDR 0x60400000U

#define MIPI_PANEL_RASPI_7INCH 5 /* 800 * 480 */

#ifndef USE_MIPI_PANEL
#define USE_MIPI_PANEL MIPI_PANEL_RASPI_7INCH
#endif

#if (USE_MIPI_PANEL == MIPI_PANEL_RASPI_7INCH)
#define DEMO_PANEL_HEIGHT 480
#define DEMO_PANEL_WIDTH  800
#define DEMO_HSW          20
#define DEMO_HFP          70
#define DEMO_HBP          23
#define DEMO_VSW          2
#define DEMO_VFP          7
#define DEMO_VBP          21
#else
#define DEMO_PANEL_HEIGHT 960
#define DEMO_PANEL_WIDTH  540
#define DEMO_HSW          2
#define DEMO_HFP          32
#define DEMO_HBP          30
#define DEMO_VSW          2
#define DEMO_VFP          16
#define DEMO_VBP          14
#endif
#define DEMO_POL_FLAGS \
    (kLCDIF_DataEnableActiveHigh | kLCDIF_VsyncActiveLow | kLCDIF_HsyncActiveLow | kLCDIF_DriveDataOnRisingClkEdge)

/* Frame buffer must be 128 byte aligned. */
#define DEMO_FB_ALIGN 128

#if ((!USE_DBI) && (USE_MIPI_PANEL == MIPI_PANEL_RASPI_7INCH))
#define DEMO_MIPI_DSI_LANE_NUM 1
#else
#define DEMO_MIPI_DSI_LANE_NUM 2
#endif
#define DEMO_MIPI_DSI_BIT_PER_PIXEL            24

/* Here the desired DPHY bit clock multiplied by ( 9 / 8 = 1.125) to ensure
 * it is fast enough.
 */
#define DEMO_MIPI_DPHY_BIT_CLK_ENLARGE(origin) (((origin) / 8) * 9)

/* For DC8000, the image buffer stride in memory must be 64 byte aligned. */
#define DEMO_BUFFER_STRIDE_BYTE                (((((DEMO_IMG_WIDTH * DEMO_BYTE_PER_PIXEL) - 1U) / 64U) + 1U) * 64U)

#define DEMO_MIPI_DSI MIPI_DSI_HOST
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
status_t BOARD_InitDisplayInterface(void);
void BOARD_InitLcdifClock(void);

#endif /* _LCDIF_SUPPORT_H_ */
