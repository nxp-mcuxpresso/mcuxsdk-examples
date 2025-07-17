/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_TIMER               LPTMR0
#define EXAMPLE_TIMER_IRQn          LPTMR0_IRQn
#define EXAMPLE_TIMER_IRQHandler    LPTMR0_IRQHandler

#define EXAMPLE_CAN                 CAN0

#define EXAMPLE_LPUART              LPUART0

/* Get source clock for Timer */
#define EXAMPLE_TIMER_CLK_FREQ (16000U)
/* Get frequency of flexcan clock */
#define EXAMPLE_CAN_CLK_FREQ CLOCK_GetFlexcanClkFreq()
/* Set USE_IMPROVED_TIMING_CONFIG macro to use api to calculates the improved CAN / CAN FD timing values. */
#define USE_IMPROVED_TIMING_CONFIG (1U)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void EXAMPLE_UARTProcess(void);
void EXAMPLE_InitTimer(void);
void EXAMPLE_StartTimer(void);
void EXAMPLE_StopTimer(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
