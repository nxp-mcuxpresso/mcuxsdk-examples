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
#define LED_INIT() LED_ORANGE_INIT(0U)
#define LED_ON()   LED_ORANGE_ON()
#define LED_OFF()  LED_ORANGE_OFF()

#define DEMO_CMP_BASE             CMP0
#define DEMO_CMP_USER_CHANNEL     1U /* PTD2, CMP0_IN1, A63 on Primary Elevator Tower */
#define DEMO_CMP_DAC_CHANNEL      7U
#define DEMO_CMP_IRQ_ID           CMP0_CMP1_IRQn
#define DEMO_CMP_IRQ_HANDLER_FUNC CMP0_CMP1_IRQHandler
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
