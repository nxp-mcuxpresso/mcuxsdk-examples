/*
 * Copyright 2023-2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LVGL_SUPPORT_BOARD_H
#define LVGL_SUPPORT_BOARD_H

#include <stdint.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* LCD panel. */
#define BOARD_LCD_RST_GPIO GPIO3
#define BOARD_LCD_RST_PIN  0
#define BOARD_LCD_TE_GPIO  GPIO2
#define BOARD_LCD_TE_PIN   21
#define BOARD_LCD_CS_GPIO  GPIO2
#define BOARD_LCD_CS_PIN   19
#define BOARD_LCD_RS_GPIO  GPIO2
#define BOARD_LCD_RS_PIN   17
#define BOARD_LCD_INT_PORT PORT2
#define BOARD_LCD_INT_GPIO GPIO2
#define BOARD_LCD_INT_PIN  15

#define BOARD_LCD_INT_IRQn GPIO2_IRQn
#define BOARD_LCD_INT_IRQHandler GPIO2_IRQHandler

/* Macros for FlexIO interfacing the LCD */
#define BOARD_FLEXIO              FLEXIO0
#define BOARD_FLEXIO_CLOCK_FREQ   CLOCK_GetFlexioClkFreq()
#define BOARD_FLEXIO_BAUDRATE_BPS 160000000U

/* Macros for FlexIO shifter, timer, and pins. */
#define BOARD_FLEXIO_WR_PIN           31
#define BOARD_FLEXIO_RD_PIN           28
#define BOARD_FLEXIO_DATA_PIN_START   0
#define BOARD_FLEXIO_TX_START_SHIFTER 0
#define BOARD_FLEXIO_RX_START_SHIFTER 0
#define BOARD_FLEXIO_TX_END_SHIFTER   3
#define BOARD_FLEXIO_RX_END_SHIFTER   3
#define BOARD_FLEXIO_TIMER            0

/* Macros for the touch controller. */
#define BOARD_TOUCH_I2C            LPI2C2
#define BOARD_TOUCH_I2C_CLOCK_FREQ CLOCK_GetLpi2cClkFreq(2u)
#define BOARD_TOUCH_I2C_BAUDRATE   100000U

/*******************************************************************************
 * API
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__cplusplus)
}
#endif

#endif /*LVGL_SUPPORT_H */
