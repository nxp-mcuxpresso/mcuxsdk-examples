/*
 * Copyright 2024 NXP
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
#define EXAMPLE_DMA_BASEADDR   EDMA4
#define EXAMPLE_DMA_CLOCK_ROOT kCLOCK_Root_WakeupAxi
#define DEMO_DMA_CHANNEL_0     0
#define DEMO_DMA_CHANNEL_1     1
#define DEMO_DMA_CHANNEL_2     2
#define APP_DMA_IRQ            EDMA4_CH2_IRQn
#define APP_DMA_IRQ_HANDLER    EDMA4_CH2_IRQHandler

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
