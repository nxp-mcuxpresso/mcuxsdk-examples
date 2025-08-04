/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _EMWIN_SUPPORT_H_
#define _EMWIN_SUPPORT_H_

#include <stdbool.h>

/* Macros for the LCD controller. */
#ifndef BOARD_LCD_S035
#define BOARD_LCD_S035 1
#endif

/* LCD panel. */
#define BOARD_LCD_RST_GPIO GPIO4
#define BOARD_LCD_RST_PIN  3
#define BOARD_LCD_TE_GPIO  GPIO2
#define BOARD_LCD_TE_PIN   25
#define BOARD_LCD_CS_GPIO  GPIO3
#define BOARD_LCD_CS_PIN   16
#define BOARD_LCD_RS_GPIO  GPIO4
#define BOARD_LCD_RS_PIN   1
#define BOARD_LCD_INT_PORT PORT2
#define BOARD_LCD_INT_GPIO GPIO2
#define BOARD_LCD_INT_PIN  24

/* Macros for FlexIO interfacing the LCD */
#define BOARD_FLEXIO              FLEXIO0
#define BOARD_FLEXIO_CLOCK_FREQ   CLOCK_GetFlexioClkFreq()
#define BOARD_FLEXIO_BAUDRATE_BPS 160000000U

/* Macros for FlexIO shifter, timer, and pins. */
#define BOARD_FLEXIO_WR_PIN           7
#define BOARD_FLEXIO_RD_PIN           8
#define BOARD_FLEXIO_DATA_PIN_START   16
#define BOARD_FLEXIO_TX_START_SHIFTER 0
#define BOARD_FLEXIO_RX_START_SHIFTER 0
#define BOARD_FLEXIO_TX_END_SHIFTER   3
#define BOARD_FLEXIO_RX_END_SHIFTER   3
#define BOARD_FLEXIO_TIMER            0

/* Macros for the touch touch controller. */
#define BOARD_TOUCH_I2C            LPI2C3
#define BOARD_TOUCH_I2C_CLOCK_FREQ CLOCK_GetLpi2cClkFreq(3u)
#define BOARD_TOUCH_I2C_BAUDRATE   100000U

#define BOARD_LCD_READABLE 1

#define LCD_WIDTH  320
#define LCD_HEIGHT 480

/* Define scale factors */
#define GUI_SCALE_FACTOR   1
#define GUI_SCALE_FACTOR_X 1
#define GUI_SCALE_FACTOR_Y 2

/* Use larger fonts */
#define GUI_NORMAL_FONT (&GUI_Font24_ASCII)
#define GUI_LARGE_FONT  (&GUI_Font32B_ASCII)

#define GUI_NUMBYTES 0x8000 /*! Amount of memory assigned to the emWin library */

void BOARD_SetCSPin(bool set);
void BOARD_SetRSPin(bool set);

extern int BOARD_Touch_Poll(void);

#endif
