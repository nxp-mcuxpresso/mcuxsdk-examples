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
/* FlexCAN instance for receiving CAN frames */
#define EXAMPLE_CAN_RX                FLEXCAN_0
#define EXAMPLE_FLEXCAN_RX_IRQn       FlexCAN0_1_IRQn
#define EXAMPLE_FLEXCAN_RX_IRQHandler FlexCAN0_1_IRQHandler

/* FlexCAN instance for transmitting CAN frames */
#define EXAMPLE_CAN_TX                FLEXCAN_1
#define EXAMPLE_FLEXCAN_TX_IRQn       FlexCAN1_1_IRQn
#define EXAMPLE_FLEXCAN_TX_IRQHandler FlexCAN1_1_IRQHandler

#define RX_MESSAGE_BUFFER_NUM (1)
#define TX_MESSAGE_BUFFER_NUM (0)

/* Enable CAN FD mode */
#define USE_CANFD (1)

/* Get frequency of FlexCAN clocks */
#define EXAMPLE_CAN_RX_CLK_FREQ (CLOCK_GetFreq(kCLOCK_Flexcan0Clk))
#define EXAMPLE_CAN_TX_CLK_FREQ (CLOCK_GetFreq(kCLOCK_Flexcan1Clk))

/* Set USE_IMPROVED_TIMING_CONFIG macro to use API to calculate improved CAN FD timing values */
#define USE_IMPROVED_TIMING_CONFIG (1U)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
