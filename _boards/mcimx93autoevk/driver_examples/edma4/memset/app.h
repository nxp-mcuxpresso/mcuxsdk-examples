/*
 * Copyright 2023 NXP
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
#define EXAMPLE_DMA_BASEADDR   DMA4
#define EXAMPLE_DMA_CLOCK_ROOT kCLOCK_Root_WakeupAxi
#define EXAMPLE_DMA_CLOCK_GATE kCLOCK_Edma2
#define DEMO_DMA_CHANNEL_0     0U
#define APP_DMA_IRQ            DMA4_0_1_IRQn
#define APP_DMA_IRQ_HANDLER    DMA4_0_1_IRQHandler
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
