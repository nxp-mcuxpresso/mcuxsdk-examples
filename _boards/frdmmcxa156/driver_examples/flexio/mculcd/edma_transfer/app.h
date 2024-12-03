/*
 * Copyright 2024 NXP
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

#define DEMO_PANEL_WIDTH  480U
#define DEMO_PANEL_HEIGHT 320U

/* Macros for the LCD controller. */
#define BOARD_LCD_RST_GPIO GPIO3
#define BOARD_LCD_RST_PIN  0
#define BOARD_LCD_CS_GPIO  GPIO2
#define BOARD_LCD_CS_PIN   19
#define BOARD_LCD_RS_GPIO  GPIO2
#define BOARD_LCD_RS_PIN   17

#define DEMO_FLEXIO              FLEXIO0
#define DEMO_FLEXIO_CLOCK_FREQ   CLOCK_GetFlexioClkFreq()
#define DEMO_FLEXIO_BAUDRATE_BPS 160000000U

/* Macros for FlexIO shifter, timer, and pins. */
#define DEMO_FLEXIO_WR_PIN           31
#define DEMO_FLEXIO_RD_PIN           28
#define DEMO_FLEXIO_DATA_PIN_START   0
#define DEMO_FLEXIO_TX_START_SHIFTER 0
#define DEMO_FLEXIO_RX_START_SHIFTER 0
#define DEMO_FLEXIO_TX_END_SHIFTER   3
#define DEMO_FLEXIO_RX_END_SHIFTER   3
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
