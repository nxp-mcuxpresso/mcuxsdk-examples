/*
 * Copyright 2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define LED_INIT() LED_ORANGE_INIT(LOGIC_LED_OFF)
#define LED_ON()   LED_ORANGE_ON()
#define LED_OFF()  LED_ORANGE_OFF()

#define DEMO_CMP_BASE         CMP0
#define DEMO_CMP_USER_CHANNEL 1U /* PTD2, CMP0_IN1, A63 on Primary Elevator Tower */
#define DEMO_CMP_DAC_CHANNEL  7U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
