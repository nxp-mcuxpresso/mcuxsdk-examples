/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_EDMA_BASEADDR       DMA0
#define DEMO_EDMA_CHANNEL_0      kEDMA_Channel0
#define DEMO_EDMA_CHANNEL_0_IRQn DMACH0_IRQn

#define DEMO_DMAMUX_BASEADDR          DMAMUX
#define DEMO_DMAMUX_CHANNEL_0         kDMAMUX_DMAChannel0
#define DEMO_DMAMUX_ALWAYS_ON_CHANNEL kDmaRequestMuxAlwaysOn63

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
extern void DMA_Channel0_IRQHandler(void);

/*${prototype:end}*/

#endif /* _APP_H_ */
