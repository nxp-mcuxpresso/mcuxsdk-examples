/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
/* Get source clock for LPIT driver */
#define DEMO_LPUART            LPUART2
#define LPUART_CLOCK_ROOT      kCLOCK_Root_Lpuart2
#define LPUART_CLOCK_GATE      kCLOCK_Lpuart2
#define DEMO_LPUART_CLK_FREQ   CLOCK_GetIpFreq(LPUART_CLOCK_ROOT)
#define DEMO_LPUART_IRQn       LPUART2_IRQn
#define DEMO_LPUART_IRQHandler LPUART2_IRQHandler
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
