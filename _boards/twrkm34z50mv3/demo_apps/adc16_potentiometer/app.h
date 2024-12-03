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
#define DEMO_ADC16_BASEADDR      ADC
#define DEMO_ADC16_CHANNEL_GROUP 0U
#define DEMO_ADC16_USER_CHANNEL  19U
#define DEMO_LPTMR_IRQn          LPTMR_IRQn
#define DEMO_LPTMR_IRQHandler    LPTMR_IRQHandler
#define LPTMR_CLK_FREQ           CLOCK_GetFreq(kCLOCK_LpoClk)
#define LPTMR0                   LPTMR
/*${macro:end}*/

/*******************************************************************************

 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
