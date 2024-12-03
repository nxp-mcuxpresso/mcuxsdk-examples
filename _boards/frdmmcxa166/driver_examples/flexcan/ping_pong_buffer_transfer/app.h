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
#define EXAMPLE_CAN                   CAN0
#define EXAMPLE_FLEXCAN_RxWarningIRQn CAN0_IRQn
#define EXAMPLE_FLEXCAN_BusOffIRQn    CAN0_IRQn
#define EXAMPLE_FLEXCAN_ErrorIRQn     CAN0_IRQn
#define EXAMPLE_FLEXCAN_MBIRQn        CAN0_IRQn
#define EXAMPLE_FLEXCAN_IRQHandler    CAN0_IRQHandler

#define RX_QUEUE_BUFFER_BASE  (1U)
#define RX_QUEUE_BUFFER_SIZE  (4U)
#define TX_MESSAGE_BUFFER_NUM (8U)

/* Get frequency of flexcan clock */
#define EXAMPLE_CAN_CLK_FREQ CLOCK_GetFlexcanClkFreq(0)
/* Set USE_IMPROVED_TIMING_CONFIG macro to use api to calculates the improved CAN / CAN FD timing values. */
#define USE_IMPROVED_TIMING_CONFIG (1)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
