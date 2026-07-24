/*
 * Copyright 2022, 2026 NXP
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
#define DEMO_LPTMR_BASE   VBAT__LPTMR
#define DEMO_LPTMR_IRQn   VBAT_LPTMR_IRQn
#define LPTMR_LED_HANDLER VBAT_LPTMR_IRQHandler
/* Get source clock for LPTMR driver */
#define LPTMR_SOURCE_CLOCK (32u * 1000u)
/* Define LPTMR microseconds counts value */
#define LPTMR_USEC_COUNT 1000000U

#define LED_INIT()   USER_LED_INIT(LOGIC_LED_OFF)
#define LED_TOGGLE() USER_LED_TOGGLE()
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
