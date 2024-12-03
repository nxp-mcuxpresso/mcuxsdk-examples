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
#define EXAMPLE_DMA_BASEADDR DMA0
#define DEMO_DMA_CHANNEL_0   0U
#define DEMO_DMA_CHANNEL_1   1
#define DEMO_DMA_CHANNEL_2   2
#define APP_DMA_IRQ          DMA_CH2_IRQn
#define APP_DMA_IRQ_HANDLER  DMA_CH2_IRQHandler
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
