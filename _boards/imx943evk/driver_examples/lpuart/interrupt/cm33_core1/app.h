/*
 * Copyright 2024 NXP
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
#define DEMO_LPUART            LPUART8
#define DEMO_LPUART_INSTANCE_IDX (8U)
#define LPUART_CLOCK_ROOT      kCLOCK_Lpuart8
#define DEMO_LPUART_CLK_FREQ   CLOCK_GetRate(LPUART_CLOCK_ROOT)
#define DEMO_LPUART_IRQn       LPUART8_IRQn
#define DEMO_LPUART_IRQHandler LPUART8_IRQHandler
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
