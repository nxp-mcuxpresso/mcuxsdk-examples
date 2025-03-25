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
#define EXAMPLE_CAN FLEXCAN_0
#define EXAMPLE_CAN_DMA            EDMA
#define EXAMPLE_CAN_DMA_CHANNEL    (0)
#define EXAMPLE_CAN_DMA_REQUEST    kDmaRequestMux0CAN0
#define EXAMPLE_CAN_DMAMUX         DMAMUX_0

#define TX_MESSAGE_BUFFER_NUM (0)

/* Get frequency of flexcan clock */
#define EXAMPLE_CAN_CLK_FREQ (CLOCK_GetFreq(kCLOCK_Flexcan0Clk))
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
