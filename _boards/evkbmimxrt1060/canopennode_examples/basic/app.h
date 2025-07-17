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
#define EXAMPLE_TIMER               PIT
#define EXAMPLE_TIMER_IRQn          PIT_IRQn
#define EXAMPLE_TIMER_IRQHandler    PIT_IRQHandler

#define EXAMPLE_CAN                 CAN2

#define EXAMPLE_LPUART              LPUART1

/* Get source clock for Timer */
#define EXAMPLE_TIMER_CLK_FREQ CLOCK_GetFreq(kCLOCK_OscClk)
/* Select 60M clock divided by USB1 PLL (480 MHz) as master flexcan clock source */
#define FLEXCAN_CLOCK_SOURCE_SELECT (0U)
/* Clock divider for master flexcan clock source */
#define FLEXCAN_CLOCK_SOURCE_DIVIDER (2U)
/* Get frequency of flexcan clock */
#define EXAMPLE_CAN_CLK_FREQ ((CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 8) / (FLEXCAN_CLOCK_SOURCE_DIVIDER + 1U))
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
