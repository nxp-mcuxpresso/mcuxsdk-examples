/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
/*${header:end}*/
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_DMA_CHANNEL       30U
#define EXAMPLE_DMA_CHANNEL_CLOCK kCLOCK_Dma2Ch30
#define EXAMPLE_DMA_BASEADDR      DMA2
#define EXAMPLE_SIM_SEC_BASEADDR  SIM_SEC
#define EXAMPLE_DATA_SRCADDR      ((uint32_t *)0xa8400000)
#define EXAMPLE_DATA_DSTADDR      ((uint32_t *)0xa8400100)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
