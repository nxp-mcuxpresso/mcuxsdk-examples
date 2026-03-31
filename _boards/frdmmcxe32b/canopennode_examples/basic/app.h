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
#define EXAMPLE_TIMER               PIT_0
#define EXAMPLE_TIMER_IRQn          PIT0_IRQn
#define EXAMPLE_TIMER_IRQHandler    PIT0_IRQHandler

#define EXAMPLE_CAN                 FLEXCAN_0

#define EXAMPLE_LPUART              LPUART_5

/* Get source clock for Timer */
#define EXAMPLE_TIMER_CLK_FREQ CLOCK_GetFreq(kCLOCK_Pit0Clk)
/* Get frequency of flexcan clock */
#define EXAMPLE_CAN_CLK_FREQ CLOCK_GetFreq(kCLOCK_Flexcan0Clk)
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
