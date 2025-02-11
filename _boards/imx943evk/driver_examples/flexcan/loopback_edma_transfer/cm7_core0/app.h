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
#define EXAMPLE_CAN           CAN1
#define TX_MESSAGE_BUFFER_NUM (9)

#define EXAMPLE_CAN_DMA         (EDMA1)
#define EXAMPLE_CAN_DMA_CHANNEL (kDma1RequestMuxCan1)
#define FLEXCAN_DMA_REQUEST_SOURCE (kDma1RequestMuxCan1)

#define FLEXCAN_CLOCK_ROOT         (hal_clock_can1)
#define EXAMPLE_CAN_CLK_FREQ       HAL_ClockGetRate(FLEXCAN_CLOCK_ROOT)
#define USE_IMPROVED_TIMING_CONFIG (1U)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
