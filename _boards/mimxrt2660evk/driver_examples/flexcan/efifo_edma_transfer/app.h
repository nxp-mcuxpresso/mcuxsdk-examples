/*
 * Copyright 2021, 2026 NXP
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
#define EXAMPLE_CAN                HSP__FLEXCAN_1
#define EXAMPLE_CAN_DMA            MAIN__EDMA3
#define FLEXCAN_DMA_REQUEST_SOURCE kDmaRequestMux1HspFlexCAN1
#define EXAMPLE_CAN_DMA_CHANNEL    (0)

#define TX_MESSAGE_BUFFER_NUM      (0U)
#define EXAMPLE_CAN_CLK_FREQ       CLOCK_GetRootClockFreq(kCLOCK_Root_MAIN_flexcan1_fclk)
#define USE_IMPROVED_TIMING_CONFIG (1)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
