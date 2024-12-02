/*
 * Copyright 2018-2023 NXP
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
#define TSI0_IRQHandler TSI_IRQHandler
#define TSI0_IRQn       TSI_IRQn

/* Define the delta percentage to caculate delta value */
#define TOUCH_DELTA_PERCENT (3.5F / 100.0F)

/* TSI indication led of electrode 1 */
#define LED1_INIT()   LED_BLUE_INIT(LOGIC_LED_OFF)
#define LED1_TOGGLE() LED_BLUE_TOGGLE()
#define LED2_INIT()   LED_RED1_INIT(LOGIC_LED_OFF)

/* Get source clock for LPTMR driver */
#define LPTMR_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_LpoClk)
/* Define LPTMR microseconds count value */
#define LPTMR_USEC_COUNT (220000U)

#define TSI_TRGMUX_REG_INDEX kTRGMUX_Tsi
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
