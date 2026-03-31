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
#define EXAMPLE_EDMA_DMA_MUX DMAMUX_0
#define EXAMPLE_ALWAYS_ON_REQUEST_SOURCE kDmaRequestMux0AlwaysOn62
#define EXAMPLE_DMA_BASEADDR EDMA
#define DEMO_DMA_CHANNEL_0   0
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
