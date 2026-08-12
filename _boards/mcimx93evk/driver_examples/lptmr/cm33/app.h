/*
 * Copyright 2022 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_LPTMR_BASE   LPTMR1
#define DEMO_LPTMR_IRQn   LPTMR1_IRQn
#define LPTMR_LED_HANDLER LPTMR1_IRQHandler
/* Get source clock for LPTMR driver */
#define LPTMR_SOURCE_CLOCK 32000U
/* Define LPTMR microseconds counts value */
#define LPTMR_USEC_COUNT 1000000U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
