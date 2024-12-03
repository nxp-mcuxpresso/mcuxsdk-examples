/*
 * Copyright 2021 NXP
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
#define DEMO_HSCMP_BASE             HSCMP0
#define DEMO_HSCMP_USER_CHANNEL     3U
#define DEMO_HSCMP_DAC_CHANNEL      5U
#define DEMO_HSCMP_IRQ_ID           HSCMP0_IRQn
#define LED_INIT()                  LED_RED_INIT(LOGIC_LED_OFF)
#define LED_ON()                    LED_RED_ON()
#define LED_OFF()                   LED_RED_OFF()
#define DEMO_HSCMP_IRQ_HANDLER_FUNC HSCMP0_IRQHandler
#define DEMO_VREF_BASE              VREF
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
