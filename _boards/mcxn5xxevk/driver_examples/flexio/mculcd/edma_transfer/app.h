/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/

#define DEMO_PANEL_WIDTH  800U
#define DEMO_PANEL_HEIGHT 480U

/* Macros for the LCD controller. */
#define DEMO_SSD1963_XTAL_FREQ     10000000U
#define DEMO_SSD1963_PCLK_FREQ     30000000U
#define DEMO_SSD1963_HSW           48U
#define DEMO_SSD1963_HFP           40U
#define DEMO_SSD1963_HBP           0U
#define DEMO_SSD1963_VSW           3U
#define DEMO_SSD1963_VFP           13U
#define DEMO_SSD1963_VBP           18U
#define DEMO_SSD1963_POLARITY_FLAG 0U
#define DEMO_SSD1963_RST_GPIO      GPIO4
#define DEMO_SSD1963_RST_PIN       7
#define DEMO_SSD1963_CS_GPIO       GPIO0
#define DEMO_SSD1963_CS_PIN        12
#define DEMO_SSD1963_RS_GPIO       GPIO0
#define DEMO_SSD1963_RS_PIN        7

#define DEMO_FLEXIO              FLEXIO0
#define DEMO_FLEXIO_CLOCK_FREQ   CLOCK_GetFlexioClkFreq()
#define DEMO_FLEXIO_BAUDRATE_BPS 160000000U /* 16-bit data bus, baud rate on each pin is 1MHz. */

/* Macros for FlexIO shifter, timer, and pins. */
#define DEMO_FLEXIO_WR_PIN           1
#define DEMO_FLEXIO_RD_PIN           0
#define DEMO_FLEXIO_DATA_PIN_START   16
#define DEMO_FLEXIO_TX_START_SHIFTER 0
#define DEMO_FLEXIO_RX_START_SHIFTER 0
#define DEMO_FLEXIO_TX_END_SHIFTER   7
#define DEMO_FLEXIO_RX_END_SHIFTER   7
#define DEMO_FLEXIO_TIMER            0

/* Macros for DMA */
#define DEMO_EDMA                  DMA0
#define DEMO_FLEXIO_TX_DMA_CHANNEL 0
#define DEMO_FLEXIO_TX_DMA_REQUEST kDma0RequestMuxFlexIO0ShiftRegister0Request

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_SetCSPin(bool set);
void BOARD_SetRSPin(bool set);
void BOARD_SetResetPin(bool set);
/*${prototype:end}*/

#endif /* _APP_H_ */
