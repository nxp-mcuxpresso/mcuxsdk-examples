/*
 * Copyright 2021 NXP
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
#define APP_SLCD_DUTY_CYCLE      kSLCD_1Div8DutyCycle
#define APP_SLCD_LOW_PIN_ENABLED 0x9ffffe00U
/* LCD_P31/P28/ P27/P26/P25/P24/P23/P22/P21/P20/P19/P18/P17/P16/P15/P14/P13/P12/P11/P10/P9. */
#define APP_SLCD_HIGH_PIN_ENABLED    0x0000083FU /* LCD_P43/P37/P36/P35/P34/P33/P32.*/
#define APP_SLCD_BACK_PANEL_LOW_PIN  0x0001fe00U /* LCD_P16/15/14/13/12/11/10/9 --> b22/b20/b19/b14/b13 = 1. */
#define APP_SLCD_BACK_PANEL_HIGH_PIN 0x00000000U
#define APP_SLCD_ALL_PHASE_ON                                                                    \
    (kSLCD_PhaseAActivate | kSLCD_PhaseBActivate | kSLCD_PhaseCActivate | kSLCD_PhaseDActivate | \
     kSLCD_PhaseEActivate | kSLCD_PhaseFActivate | kSLCD_PhaseGActivate | kSLCD_PhaseHActivate)
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
