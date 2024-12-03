/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_common.h"
/*${header:end}*/
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_DMA_BASEADDR EDMA5_2
#define DEMO_DMA_CHANNEL_0   0U
#define APP_DMA_IRQ          DMA5_2_0_1_IRQn
#define APP_DMA_IRQ_HANDLER  DMA5_2_0_1_IRQHandler
#define BUFFER_LENGTH 4U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
