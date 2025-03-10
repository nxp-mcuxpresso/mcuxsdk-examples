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
#define EXAMPLE_LPUART             LPUART8
#define EXAMPLE_LPUART_IRQn        LPUART8_IRQn
#define EXAMPLE_LPUART_IRQHandler  LPUART8_IRQHandler

#define GPT_IRQ_ID                 GPT1_IRQn
#define EXAMPLE_GPT                GPT1
#define EXAMPLE_GPT_IRQHandler     GPT1_IRQHandler

#define EXAMPLE_CAN                CAN1
#define EXAMPLE_FLEXCAN_IRQn       CAN1_IRQn
#define EXAMPLE_FLEXCAN_IRQHandler CAN1_IRQHandler
#define RX_MESSAGE_BUFFER_NUM      (9)
#define TX_MESSAGE_BUFFER_NUM      (8)

/* Get source clock for GPT driver */
#define EXAMPLE_GPT_CLK_FREQ CLOCK_GetRate(kCLOCK_Gpt1)

#define FLEXCAN_CLOCK_ROOT   (kCLOCK_Can1)
#define EXAMPLE_CAN_CLK_FREQ (CLOCK_GetRate(FLEXCAN_CLOCK_ROOT))

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
