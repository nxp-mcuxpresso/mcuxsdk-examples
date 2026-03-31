/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LVGL_SUPPORT_BOARD_H
#define LVGL_SUPPORT_BOARD_H

#include <stdint.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#ifndef LVGL_USE_SIUL2
#define LVGL_USE_SIUL2 1
#endif

/* LCD panel. */
#define BOARD_SIUL2_BASE SIUL2
#define BOARD_LCD_RST_GPIO kSIUL2_PTB
#define BOARD_LCD_RST_PIN  15U
#define BOARD_LCD_RST_MSCR 47U
#define BOARD_LCD_TE_GPIO  kSIUL2_PTC
#define BOARD_LCD_TE_PIN   10U
#define BOARD_LCD_CS_GPIO  kSIUL2_PTB
#define BOARD_LCD_CS_PIN   1U
#define BOARD_LCD_CS_MSCR  33U
#define BOARD_LCD_RS_GPIO  kSIUL2_PTC
#define BOARD_LCD_RS_PIN   8U
#define BOARD_LCD_RS_MSCR  72U
#define BOARD_LCD_INT_GPIO kSIUL2_PTB
#define BOARD_LCD_INT_PIN  14U
#define BOARD_LCD_INT_MSCR 46U
#define BOARD_LCD_INT_IMCR 44U
#define BOARD_LCD_INT_EIRQ 28U


#define BOARD_LCD_INT_IRQn SIUL2_3_IRQn
#define BOARD_LCD_INT_IRQHandler SIUL2_3_IRQHandler

/* Macros for FlexIO interfacing the LCD */
#define BOARD_FLEXIO              FLEXIO
#define BOARD_FLEXIO_CLOCK_FREQ   CLOCK_GetFreq(kCLOCK_FlexioClk)
#define BOARD_FLEXIO_BAUDRATE_BPS 40000000U

/* Macros for FlexIO shifter, timer, and pins. */
#define BOARD_FLEXIO_WR_PIN           15
#define BOARD_FLEXIO_RD_PIN           14
#define BOARD_FLEXIO_DATA_PIN_START   16
#define BOARD_FLEXIO_TX_START_SHIFTER 0
#define BOARD_FLEXIO_RX_START_SHIFTER 0
#define BOARD_FLEXIO_TX_END_SHIFTER   3
#define BOARD_FLEXIO_RX_END_SHIFTER   3
#define BOARD_FLEXIO_TIMER            0

/* Macros for the touch controller. */
#define BOARD_TOUCH_I2C            LPI2C_1
#define BOARD_TOUCH_I2C_CLOCK_FREQ CLOCK_GetFreq(kCLOCK_Lpi2c1Clk)
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

#endif /* LVGL_SUPPORT_BOARD_H */
