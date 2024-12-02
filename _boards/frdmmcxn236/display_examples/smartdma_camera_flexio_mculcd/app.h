/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_PANEL_WIDTH  480U
#define DEMO_PANEL_HEIGHT 320U

#define DEMO_BUFFER_WIDTH  DEMO_PANEL_WIDTH / 2U
#define DEMO_BUFFER_HEIGHT DEMO_PANEL_HEIGHT

#define DEMO_CAMERA_RESOLUTION kVIDEO_ResolutionQVGA /* 320*240 */

#define DEMO_SMARTDMA_API kSMARTDMA_CameraWholeFrameQVGA

#define DEMO_LCD_RST_GPIO GPIO4
#define DEMO_LCD_RST_PIN  7U
#define DEMO_LCD_CS_GPIO  GPIO4
#define DEMO_LCD_CS_PIN   14U
#define DEMO_LCD_RS_GPIO  GPIO0
#define DEMO_LCD_RS_PIN   24U

#define DEMO_FLEXIO              FLEXIO0
#define DEMO_FLEXIO_CLOCK_FREQ   CLOCK_GetFlexioClkFreq()
#define DEMO_FLEXIO_BAUDRATE_BPS 160000000U /* 16-bit data bus, baud rate on each pin is 1MHz. */

/* Macros for FlexIO shifter, timer, and pins. */
#define DEMO_FLEXIO_WR_PIN           28U
#define DEMO_FLEXIO_RD_PIN           27U
#define DEMO_FLEXIO_DATA_PIN_START   4U
#define DEMO_FLEXIO_TX_START_SHIFTER 0U
#define DEMO_FLEXIO_RX_START_SHIFTER 0U
#define DEMO_FLEXIO_TX_END_SHIFTER   7U
#define DEMO_FLEXIO_RX_END_SHIFTER   7U
#define DEMO_FLEXIO_TIMER            0U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
