/*
 * Copyright 2022 NXP
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

/* Available PAD names on board */
#define PAD_TSI_MUTUAL_CAP_1_NAME "KEY3"

/* IRQ related redefinitions for specific SOC */
#define TSI0_IRQHandler TSI_END_OF_SCAN_IRQHandler
#define TSI0_IRQn       TSI_END_OF_SCAN_IRQn

/* Define the delta value to indicate a touch event */
#define TOUCH_DELTA_VALUE 100U

/* TSI indication led of electrode 1 */
#define LED1_INIT()   LED_BLUE_INIT(LOGIC_LED_OFF)
#define LED1_TOGGLE() LED_BLUE_TOGGLE()
#define LED2_INIT()   LED_RED_INIT(LOGIC_LED_OFF)

/* Get source clock for LPTMR driver */
#define LPTMR_SOURCE_CLOCK (16000)
/* Define LPTMR microseconds count value */
#define LPTMR_USEC_COUNT (100000)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
