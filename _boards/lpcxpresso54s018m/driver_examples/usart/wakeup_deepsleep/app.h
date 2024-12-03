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
#define DEMO_USART            USART0
#define DEMO_USART_CLK_FREQ   CLOCK_GetFlexCommClkFreq(0)
#define DEMO_USART_IRQHandler FLEXCOMM0_IRQHandler
#define DEMO_USART_IRQn       FLEXCOMM0_IRQn
#define APP_EXCLUDE_FROM_DEEPSLEEP                                                                             \
    (SYSCON_PDRUNCFG_PDEN_SRAMX_MASK | SYSCON_PDRUNCFG_PDEN_SRAM0_MASK | SYSCON_PDRUNCFG_PDEN_SRAM1_2_3_MASK | \
     SYSCON_PDRUNCFG_PDEN_VD6_MASK)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_SwitchBackClockSrc(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
