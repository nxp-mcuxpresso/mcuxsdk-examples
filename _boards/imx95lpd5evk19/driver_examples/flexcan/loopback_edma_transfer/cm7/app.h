/*
 * Copyright 2023 NXP
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
#define EXAMPLE_CAN           CAN1
#define TX_MESSAGE_BUFFER_NUM (9)

#define EXAMPLE_CAN_DMA         (DMA3)
#define EXAMPLE_CAN_DMA_CHANNEL Dma3RequestMuxCAN1

/* FLEXCAN_DMA_REQUEST_SOURCE is not used in driver, just avoid build error introduced by compatibility */
#define FLEXCAN_DMA_REQUEST_SOURCE (0)

#define FLEXCAN_CLOCK_ROOT         (hal_clock_can1)
#define EXAMPLE_CAN_CLK_FREQ       HAL_ClockGetIpFreq(FLEXCAN_CLOCK_ROOT)
#define USE_IMPROVED_TIMING_CONFIG (1U)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
