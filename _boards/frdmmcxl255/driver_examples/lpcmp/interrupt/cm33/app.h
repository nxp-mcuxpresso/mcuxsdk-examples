/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

#include "board.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_LPCMP_BASE             ACMP0
#define DEMO_LPCMP_USER_CHANNEL     3U
#define DEMO_LPCMP_DAC_CHANNEL      7U
#define DEMO_LPCMP_IRQ_ID           ACMP0_IRQn
#define DEMO_LPCMP_REFERENCE        kLPCMP_VrefSourceVin1 /* VDD as reference*/
#define LED_INIT()                  LED_BLUE_INIT(LOGIC_LED_OFF)
#define LED_ON()                    LED_BLUE_ON()
#define LED_OFF()                   LED_BLUE_OFF()
#define DEMO_LPCMP_IRQ_HANDLER_FUNC ACMP0_IRQHandler
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
