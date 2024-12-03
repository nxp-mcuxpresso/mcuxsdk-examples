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
#define DEMO_LPCMP_BASE             LPCMP0
#define DEMO_LPCMP_USER_CHANNEL     0U
#define DEMO_LPCMP_DAC_CHANNEL      7U
#define DEMO_LPCMP_IRQ_ID           LPCMP0_IRQn
#define LED_INIT()                  LED3_INIT(LOGIC_LED_OFF)
#define LED_ON()                    LED3_ON()
#define LED_OFF()                   LED3_OFF()
#define DEMO_LPCMP_IRQ_HANDLER_FUNC LPCMP0_IRQHandler
#define DEMO_VREF_BASE              VREF0
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
