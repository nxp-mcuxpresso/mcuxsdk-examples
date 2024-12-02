/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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
#define APP_SLCD_DUTY_CYCLE          kSLCD_1Div8DutyCycle
#define APP_SLCD_LOW_PIN_ENABLED     0xF7D86000U /* LCD_P31/P30/P29/P28/P26/P25/P24/P23/P22/P20/P19/P14/P13. */
#define APP_SLCD_HIGH_PIN_ENABLED    0x0F04387FU /* LCD_P59/P58/P57/P56/P50/P45/P44/P43/P38/P37/P36/P35/P34/P33/P32. */
#define APP_SLCD_BACK_PANEL_LOW_PIN  0x00586000U /* LCD_P22/20/19/14/13 --> b22/b20/b19/b14/b13 = 1. */
#define APP_SLCD_BACK_PANEL_HIGH_PIN 0x07000000U /* LCD_P58/57/56 --> b26/b25/b24 = 1. */
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
