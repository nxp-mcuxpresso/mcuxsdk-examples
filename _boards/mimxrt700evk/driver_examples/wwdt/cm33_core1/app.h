/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define WWDT                WWDT2
#define APP_WDT_IRQn        WDT2_IRQn
#define APP_WDT_IRQ_HANDLER WDT2_IRQHandler
#define APP_LED_TOGGLE      LED_RED_TOGGLE()
#define APP_LED_INIT        LED_RED_INIT(LOGIC_LED_OFF)
#define APP_LED_ON          LED_RED_ON()
#define WDT_CLK_FREQ        CLOCK_GetWdtClkFreq(2U)

#define IS_WWDT_RESET (0U != (RSTCTL3->SYSRSTSTAT & RSTCTL3_SYSRSTSTAT_WWDT2_RESET_MASK))
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
