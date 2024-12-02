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
#define DEMO_USART                 USART0
#define DEMO_USART_CLK_SRC         kCLOCK_Flexcomm0
#define DEMO_USART_CLK_FREQ        CLOCK_GetFlexCommClkFreq(0)
#define DEMO_USART_IRQHandler      FLEXCOMM0_IRQHandler
#define DEMO_USART_IRQn            FLEXCOMM0_IRQn
#define APP_EXCLUDE_FROM_DEEPSLEEP (kPDRUNCFG_PD_DCDC | kPDRUNCFG_PD_FRO192M | kPDRUNCFG_PD_FRO32K)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_SwitchBackClockSrc(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
