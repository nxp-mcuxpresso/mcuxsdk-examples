/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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
#define WWDT                WWDT0
#define APP_LED_INIT        LED_BLUE_INIT(LOGIC_LED_OFF);
#define APP_LED_ON          (LED_BLUE_ON());
#define APP_LED_TOGGLE      (LED_BLUE_TOGGLE());
#define APP_WDT_IRQn        WWDT0_IRQn
#define APP_WDT_IRQ_HANDLER WWDT0_IRQHandler
#define WDT_CLK_FREQ        CLOCK_GetWwdtClkFreq()
#define IS_WWDT_RESET       (0 != (CMC->SRS & CMC_SRS_WWDT0_MASK))
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
