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
#define EXAMPLE_CAN                CAN0
#define EXAMPLE_FLEXCAN_IRQn       CAN0_ORed_0_15_MB_IRQn
#define EXAMPLE_FLEXCAN_IRQHandler CAN0_ORed_0_15_MB_IRQHandler

#define RX_MESSAGE_BUFFER_NUM (1)
#define TX_MESSAGE_BUFFER_NUM (0)

#define USE_CANFD (1)

/* Get frequency of flexcan clock */
#define EXAMPLE_CAN_CLK_FREQ       CLOCK_GetFreq(kCLOCK_CoreSysClk)
/* Set USE_IMPROVED_TIMING_CONFIG macro to use api to calculates the improved CAN / CAN FD timing values. */
#define USE_IMPROVED_TIMING_CONFIG (1U)
/* Set frequency Protocol Engine clock as system clock. */
#define EXAMPLE_CAN_CLK_SOURCE     kFLEXCAN_ClkSrc1
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
