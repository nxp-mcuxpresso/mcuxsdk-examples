/*
 * Copyright 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

#include "board.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define CTIMER                  CTIMER0 /* Timer 1 */
#define CTIMER_MAT_OUT          kCTIMER_Match_0 /* Match output 0 */
#define CTIMER_CLK_FREQ         CLOCK_GetCTimerClkFreq(0U)
#define EXAMPLE_DMA_BASEADDR    DMA0
#define DEMO_DMA_CHANNEL        (0U)
#define USE_LPC_DMA             (1)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
/*******************************************************************************
 * EOF
 ******************************************************************************/