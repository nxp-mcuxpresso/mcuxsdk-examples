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
/* Master related */
#define DRIVER_MASTER_SPI                 Driver_SPI3
#define EXAMPLE_LPSPI_DEALY_COUNT         0xfffffU
#define EXAMPLE_LPSPI_MASTER_DMA_BASEADDR DMA0
#define EXAMPLE_LPSPI_DMA_CLOCK           kCLOCK_Dma0
#define EXAMPLE_LPSPI_CLOCK_FREQ          CLOCK_GetLPFlexCommClkFreq(3u)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif
