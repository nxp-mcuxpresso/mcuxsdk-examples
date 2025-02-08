/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*(${macro:start}*/
#define LED_NUMBERS  4U
#define LED_1_INIT() LED_BLUE_INIT(LOGIC_LED_OFF)
#define LED_1_ON()   LED_BLUE_ON()
#define LED_1_OFF()  LED_BLUE_OFF()

#define LED_2_INIT() RGB_LED_RED_INIT(LOGIC_LED_OFF)
#define LED_2_ON()   RGB_LED_RED_ON()
#define LED_2_OFF()  RGB_LED_RED_OFF()

#define LED_3_INIT() RGB_LED_GREEN_INIT(LOGIC_LED_OFF)
#define LED_3_ON()   RGB_LED_GREEN_ON()
#define LED_3_OFF()  RGB_LED_GREEN_OFF()

#define LED_4_INIT() RGB_LED_BLUE_INIT(LOGIC_LED_OFF)
#define LED_4_ON()   RGB_LED_BLUE_ON()
#define LED_4_OFF()  RGB_LED_BLUE_OFF()
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void Led_Init(void);
/*${prototype:end}*/
#endif
