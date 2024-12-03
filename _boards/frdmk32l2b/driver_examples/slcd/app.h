/*
 * Copyright 2019 NXP
 * All rights reserved.
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
#define APP_SLCD_LOW_PIN_ENABLED     0x0d10c000U /* LCD_P27/26/24/20 -> b27/26/24/20 = 1. */
#define APP_SLCD_HIGH_PIN_ENABLED    0x18001d00U /* LCD_P44/43/42/40 -> b12/11/10/8 = 1. */
#define APP_SLCD_BACK_PANEL_LOW_PIN  0x0000c000U /* LCD_P15/P14 -> b15/b14 = 1. */
#define APP_SLCD_BACK_PANEL_HIGH_PIN 0x18000000U /* LCD_P60/P59 -> b28/27 = 1. */
/*${macro:end}*/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*${variable:start}*/
extern slcd_clock_config_t slcdClkConfig;
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
