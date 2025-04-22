/*
 * Copyright 2024 NXP
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
#define DEMO_LPCMP_BASE             CMP0
#define DEMO_LPCMP_USER_CHANNEL     1U
#define DEMO_LPCMP_DAC_CHANNEL      7U
#define DEMO_LPCMP_IRQ_ID           CMP0_IRQn
#define LED_INIT()                  LED_RED_INIT(LOGIC_LED_OFF)
#define LED_ON()                    LED_RED_ON()
#define LED_OFF()                   LED_RED_OFF()
#define DEMO_LPCMP_IRQ_HANDLER_FUNC CMP0_IRQHandler
#define DEMO_LPCMP_REFERENCE        kLPCMP_VrefSourceVin1
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
