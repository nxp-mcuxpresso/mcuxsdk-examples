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
#define GPT_IRQ_ID                 GPT1_IRQn
#define EXAMPLE_GPT                GPT1
#define EXAMPLE_GPT_IRQHandler     GPT1_IRQHandler

#define EXAMPLE_CAN                CAN3
#define EXAMPLE_FLEXCAN_IRQn       CAN3_IRQn
#define EXAMPLE_FLEXCAN_IRQHandler CAN3_IRQHandler
#define RX_MESSAGE_BUFFER_NUM      (9)
#define TX_MESSAGE_BUFFER_NUM      (8)

/* Get source clock for GPT driver */
#define EXAMPLE_GPT_CLK_FREQ CLOCK_GetRootClockFreq(kCLOCK_Root_Gpt1)
/* Get frequency of flexcan clock */
#define EXAMPLE_CAN_CLK_FREQ CLOCK_GetRootClockFreq(kCLOCK_Root_Can3)
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
