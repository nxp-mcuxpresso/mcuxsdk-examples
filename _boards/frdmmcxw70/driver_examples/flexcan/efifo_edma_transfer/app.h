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
#define EXAMPLE_CAN                CAN_0
#define EXAMPLE_CAN_DMA            DMA0
#define EXAMPLE_CAN_DMA_CHANNEL    (0)
#define FLEXCAN_DMA_REQUEST_SOURCE (kDmaRequestCAN0)

#define TX_MESSAGE_BUFFER_NUM      (0U)
#define EXAMPLE_CAN_CLK_FREQ       CLOCK_GetFreq(kCLOCK_ScgSysOscClk)
#define USE_IMPROVED_TIMING_CONFIG (1)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
