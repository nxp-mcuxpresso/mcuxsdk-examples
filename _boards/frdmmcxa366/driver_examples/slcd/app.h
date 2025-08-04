/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_slcd.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define APP_SLCD_DUTY_CYCLE          kSLCD_1Div4DutyCycle
#define APP_SLCD_LOW_PIN_ENABLED     0xFFFF0000U /* LCD_P16-P31 = 1. */
#define APP_SLCD_HIGH_PIN_ENABLED    0U
#define APP_SLCD_BACK_PANEL_LOW_PIN  0xF0000000U /* LCD_P28-P31 = 1. */
#define APP_SLCD_BACK_PANEL_HIGH_PIN 0U
#define LCD LCD0
/*${macro:end}*/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*${variable:start}*/
extern const uint8_t slcd_lcd_gpio_seg_pin[];
/*${variable:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_SetSlcdBackPlanePhase(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
