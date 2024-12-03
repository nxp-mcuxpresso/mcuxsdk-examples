/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "hal_clock.h"
/*${header:end}*/
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_DMA_BASEADDR DMA3
#define DEMO_DMA_CHANNEL_0   0U
#define APP_DMA_IRQ          DMA3_0_IRQn
#define APP_DMA_IRQ_HANDLER  DMA3_0_IRQHandler
#define EXAMPLE_DMA_CLOCK_ROOT hal_clock_m33
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
