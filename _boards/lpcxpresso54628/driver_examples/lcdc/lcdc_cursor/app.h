/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define APP_LCD            LCD
#define LCD_PANEL_CLK      9000000
#define LCD_PPL            480
#define LCD_HSW            2
#define LCD_HFP            8
#define LCD_HBP            43
#define LCD_LPP            272
#define LCD_VSW            10
#define LCD_VFP            4
#define LCD_VBP            12
#define LCD_POL_FLAGS      kLCDC_InvertVsyncPolarity | kLCDC_InvertHsyncPolarity
#define IMG_HEIGHT         272
#define IMG_WIDTH          480
#define LCD_INPUT_CLK_FREQ CLOCK_GetLcdClkFreq()
#define APP_LCD_IRQHandler LCD_IRQHandler
#define APP_LCD_IRQn       LCD_IRQn
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
