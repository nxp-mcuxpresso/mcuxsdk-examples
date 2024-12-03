/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _LCDIFV3_SUPPORT_H_
#define _LCDIFV3_SUPPORT_H_

#include "fsl_mipi_dsi.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define DEMO_LCDIFV3            LCDIF
#define DEMO_LCDIFV3_IRQn       LCDIFv3_IRQn
#define DEMO_LCDIFV3_IRQHandler LCDIFv3_IRQHandler
#define DSI_ENABLE_PIN          9U
#define DSI_RESET_PIN           12U
#define DSI_PWM_PIN             10U
#define SRC_ADDR                0x44460000U
#define SRC_ISO_CTRL_OFFSET     0x10CU
#define ESCCLK                  20000000
#define BTA                     10000
#define HSTX                    60000
#define LPRX                    60000
#define FRAME_RATE              60
#define UPDATE_PIC_FLAG         1U

#define X_MX8_DSI_OLED1 0 /* 1080 * 1920 */

#ifndef USE_MIPI_PANEL
#define USE_MIPI_PANEL X_MX8_DSI_OLED1
#endif

/*
 * The pixel clock is (height + VSW + VFP + VBP) * (width + HSW + HFP + HBP) * frame rate.
 * (1920 + 2 + 4 + 10) * (1080 + 2 + 34 + 20) * x Hz = 1936 * 1136 * x Hz = 2,199,296 x = 121000000 Hz
 */

#if (USE_MIPI_PANEL == X_MX8_DSI_OLED1)
#define DEMO_PANEL_HEIGHT 1920
#define DEMO_PANEL_WIDTH  1080
#define DEMO_HSW          2
#define DEMO_HFP          20
#define DEMO_HBP          34
#define DEMO_VSW          2
#define DEMO_VFP          10
#define DEMO_VBP          4
#endif

#define DEMO_POL_FLAGS                                                                   \
    (kLCDIFV3_DataEnableActiveHigh | kLCDIFV3_VsyncActiveLow | kLCDIFV3_HsyncActiveLow | \
     kLCDIFV3_DriveDataOnFallingClkEdge)

/* Frame buffer must be 8 byte aligned. */
#define DEMO_FB_ALIGN     8
#define DEMO_BUFFER_COUNT 2
/* The DEMO_BUFFER_BYTE_PER_PIXEL VALUE is 2 for RGB565, 3 for RGB888. */
#define DEMO_BUFFER_BYTE_PER_PIXEL 2U

/* The MIPI DSI support 4 line. */
#define DEMO_MIPI_DSI_LANE_NUM 4

#define DEMO_FB0_ADDR                 ((uint32_t)s_frameBuffer[0])
#define DEMO_FB1_ADDR                 ((uint32_t)s_frameBuffer[1])
#define DEMO_FB0_BITMAP_ADDR          ((uint32_t)s_pictureBitmap[0])
#define DEMO_FB1_BITMAP_ADDR          ((uint32_t)s_pictureBitmap[1])
#define DEMO_FB0_UPDATE_PIC_FLAG_ADDR ((uint32_t)&s_updatePicFlag[0])
#define DEMO_FB1_UPDATE_PIC_FLAG_ADDR ((uint32_t)&s_updatePicFlag[1])

extern uint8_t s_frameBuffer[DEMO_BUFFER_COUNT][DEMO_PANEL_HEIGHT][DEMO_PANEL_WIDTH][DEMO_BUFFER_BYTE_PER_PIXEL];
extern uint32_t s_pictureBitmap[DEMO_BUFFER_COUNT][DEMO_PANEL_HEIGHT][DEMO_PANEL_WIDTH][DEMO_BUFFER_BYTE_PER_PIXEL];
extern uint32_t s_updatePicFlag[DEMO_BUFFER_COUNT];

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
status_t BOARD_InitDisplayInterface(void);

#endif /* _LCDIFV3_SUPPORT_H_ */
