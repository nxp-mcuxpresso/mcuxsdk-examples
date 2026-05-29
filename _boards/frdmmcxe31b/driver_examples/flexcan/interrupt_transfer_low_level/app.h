/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
/* FlexCAN0 used for receiving */
#define EXAMPLE_CAN_RX                FLEXCAN_0
#define EXAMPLE_FLEXCAN_RX_IRQn       FlexCAN0_1_IRQn
#define EXAMPLE_FLEXCAN_RX_IRQHandler FlexCAN0_1_IRQHandler

/* FlexCAN1 used for transmitting */
#define EXAMPLE_CAN_TX                FLEXCAN_1
#define EXAMPLE_FLEXCAN_TX_IRQn       FlexCAN1_1_IRQn
#define EXAMPLE_FLEXCAN_TX_IRQHandler FlexCAN1_1_IRQHandler

#define RX_MESSAGE_BUFFER_NUM (1)
#define TX_MESSAGE_BUFFER_NUM (0)

/* Get frequency of flexcan clock */
#define EXAMPLE_CAN_RX_CLK_FREQ (CLOCK_GetFreq(kCLOCK_Flexcan0Clk))
#define EXAMPLE_CAN_TX_CLK_FREQ (CLOCK_GetFreq(kCLOCK_Flexcan1Clk))
/* Set USE_IMPROVED_TIMING_CONFIG macro to use api to calculates the improved CAN / CAN FD timing values. */
#define USE_IMPROVED_TIMING_CONFIG (1U)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
