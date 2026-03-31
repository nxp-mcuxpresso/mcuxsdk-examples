/*
 * Copyright 2025 NXP
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
#define BOARD_SIUL2_BASE SIUL2
#define BOARD_LCD_RST_GPIO kSIUL2_PTB
#define BOARD_LCD_RST_PIN  15
#define BOARD_LCD_CS_GPIO  kSIUL2_PTB
#define BOARD_LCD_CS_PIN   1
#define BOARD_LCD_RS_GPIO  kSIUL2_PTC
#define BOARD_LCD_RS_PIN   8

#define DEMO_FLEXIO              FLEXIO
#define DEMO_FLEXIO_CLOCK_FREQ   CLOCK_GetFreq(kCLOCK_FlexioClk)
#define DEMO_FLEXIO_BAUDRATE_BPS 40000000U

/* Macros for FlexIO shifter, timer, and pins. */
#define DEMO_FLEXIO_WR_PIN           15
#define DEMO_FLEXIO_RD_PIN           14
#define DEMO_FLEXIO_DATA_PIN_START   16
#define DEMO_FLEXIO_TX_START_SHIFTER 0
#define DEMO_FLEXIO_RX_START_SHIFTER 0
#define DEMO_FLEXIO_TX_END_SHIFTER   3
#define DEMO_FLEXIO_RX_END_SHIFTER   3
#define DEMO_FLEXIO_TIMER            0

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
