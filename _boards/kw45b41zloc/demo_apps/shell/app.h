/*
 * Copyright 2023 NXP
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
#define LED_NUMBERS  2U
#define LED_1_INIT() LED1_INIT(LOGIC_LED_OFF)
#define LED_1_ON()   LED1_ON()
#define LED_1_OFF()  LED1_OFF()

#define LED_2_INIT() LED2_INIT(LOGIC_LED_OFF)
#define LED_2_ON()   LED2_ON()
#define LED_2_OFF()  LED2_OFF()

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void Led_Init(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
