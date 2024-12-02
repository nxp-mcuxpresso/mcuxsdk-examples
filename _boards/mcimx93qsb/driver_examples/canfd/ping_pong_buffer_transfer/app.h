/*
 * Copyright 2022 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_CAN                   CAN1
#define EXAMPLE_FLEXCAN_RxWarningIRQn CAN1_IRQn
#define EXAMPLE_FLEXCAN_BusOffIRQn    CAN1_IRQn
#define EXAMPLE_FLEXCAN_ErrorIRQn     CAN1_IRQn
#define EXAMPLE_FLEXCAN_MBIRQn        CAN1_IRQn
#define EXAMPLE_FLEXCAN_IRQHandler    CAN1_IRQHandler

#define RX_QUEUE_BUFFER_BASE  (1U)
#define RX_QUEUE_BUFFER_SIZE  (4U)
#define TX_MESSAGE_BUFFER_NUM (8U)

#define USE_CANFD (1)

#define FLEXCAN_CLOCK_ROOT         (kCLOCK_Root_Can1)
#define FLEXCAN_CLOCK_GATE         kCLOCK_Can1
#define EXAMPLE_CAN_CLK_FREQ       CLOCK_GetIpFreq(FLEXCAN_CLOCK_ROOT)
#define USE_IMPROVED_TIMING_CONFIG (1U)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
