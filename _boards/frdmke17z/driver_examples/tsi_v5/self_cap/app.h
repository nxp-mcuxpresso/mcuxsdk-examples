/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2023 NXP
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
#define PAD_TSI_ELECTRODE_1_NAME "E1"

/* Disable ELECTRODE_2 as ELECTRODE_2 and ELECTRODE_1 are on different TSI instance. */
#define PAD_TSI_ELECTRODE_2_ENABLED 0U

/* Define the delta percentage to caculate delta value */
#define TOUCH_DELTA_PERCENT (3.5F / 100.0F)

/* TSI indication led of electrode 1 */
#define LED1_INIT()   LED_GREEN_INIT(LOGIC_LED_OFF)
#define LED1_TOGGLE() LED_GREEN_TOGGLE()

/* Get source clock for LPTMR driver */
#define LPTMR_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_LpoClk)
/* Define LPTMR microseconds count value */
#define LPTMR_USEC_COUNT (260000U)

/* Define the index of the TRGMUX register for TSI trigger */
#define TSI_TRGMUX_REG_INDEX kTRGMUX_Tsi0
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
