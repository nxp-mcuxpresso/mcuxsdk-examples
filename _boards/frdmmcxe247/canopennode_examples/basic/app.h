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
#define EXAMPLE_TIMER               LPIT0
#define EXAMPLE_TIMER_IRQn          LPIT0_Ch0_IRQn
#define EXAMPLE_TIMER_IRQHandler    LPIT0_Ch0_IRQHandler

#define EXAMPLE_CAN                 CAN0

#define EXAMPLE_LPUART              LPUART2

#define EXAMPLE_CAN_CLK_FREQ        CLOCK_GetFreq(kCLOCK_ScgSysOscAsyncDiv2Clk)
#define USE_IMPROVED_TIMING_CONFIG  (1)

/* Get source clock for LPTMR driver */
#define EXAMPLE_TIMER_CLK_FREQ      CLOCK_GetIpFreq(kCLOCK_Lpit0)
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
