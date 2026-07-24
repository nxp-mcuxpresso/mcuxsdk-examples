/*
 * Copyright 2022, 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#if defined(RT2660_PRESILICON_DEVELOPMENT) && (RT2660_PRESILICON_DEVELOPMENT == 1)
#define EXAMPLE_CAN                   HSP__FLEXCAN_2
#define EXAMPLE_FLEXCAN_RxWarningIRQn HSP_FLEXCAN2_IRQn
#define EXAMPLE_FLEXCAN_BusOffIRQn    HSP_FLEXCAN2_IRQn
#define EXAMPLE_FLEXCAN_ErrorIRQn     HSP_FLEXCAN2_IRQn
#define EXAMPLE_FLEXCAN_MBIRQn        HSP_FLEXCAN2_IRQn
#define EXAMPLE_FLEXCAN_IRQHandler    HSP_FLEXCAN2_IRQHandler
#else
#define EXAMPLE_CAN                   HSP__FLEXCAN_1
#define EXAMPLE_FLEXCAN_RxWarningIRQn HSP_FLEXCAN1_IRQn
#define EXAMPLE_FLEXCAN_BusOffIRQn    HSP_FLEXCAN1_IRQn
#define EXAMPLE_FLEXCAN_ErrorIRQn     HSP_FLEXCAN1_IRQn
#define EXAMPLE_FLEXCAN_MBIRQn        HSP_FLEXCAN1_IRQn
#define EXAMPLE_FLEXCAN_IRQHandler    HSP_FLEXCAN1_IRQHandler
#endif
/* Considering that the first valid MB must be used as Reserved TX MB for ERR005829. */
#define RX_QUEUE_BUFFER_BASE  (1U)
#define RX_QUEUE_BUFFER_SIZE  (4U)
#define TX_MESSAGE_BUFFER_NUM (8U)

#define USE_CANFD (1)

/* Get frequency of flexcan clock */
#if defined(RT2660_PRESILICON_DEVELOPMENT) && (RT2660_PRESILICON_DEVELOPMENT == 1)
#define EXAMPLE_CAN_CLK_FREQ CLOCK_GetRootClockFreq(kCLOCK_Root_MAIN_flexcan2_fclk)
#else
#define EXAMPLE_CAN_CLK_FREQ CLOCK_GetRootClockFreq(kCLOCK_Root_MAIN_flexcan1_fclk)
#endif
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
