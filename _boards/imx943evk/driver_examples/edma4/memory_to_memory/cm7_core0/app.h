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
#define EXAMPLE_DMA_BASEADDR EDMA2
#define DEMO_DMA_CHANNEL_0   0U
#define APP_DMA_IRQ            EDMA2_CH0_IRQn
#define APP_DMA_IRQ_HANDLER  EDMA2_CH0_IRQHandler
#define BUFFER_LENGTH 4U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
