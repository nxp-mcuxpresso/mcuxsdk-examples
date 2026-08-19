/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_CAN           CAN2
#define TX_MESSAGE_BUFFER_NUM (0)

#define EXAMPLE_CAN_DMA            DMA4
#define EXAMPLE_CAN_DMA_CHANNEL    0
#define EXAMPLE_DMA_CLOCK_ROOT     kCLOCK_Root_WakeupAxi
#define EXAMPLE_DMA_CLOCK_GATE     kCLOCK_Edma1
#define FLEXCAN_DMA_REQUEST_SOURCE kDma4RequestMuxCAN2

#define FLEXCAN_CLOCK_ROOT         (kCLOCK_Root_Can2)
#define FLEXCAN_CLOCK_GATE         kCLOCK_Can2
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
