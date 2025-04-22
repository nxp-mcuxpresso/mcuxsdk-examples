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
#define DRIVER_MASTER_SPI                 Driver_SPI1
#define EXAMPLE_LPSPI_DEALY_COUNT         0xfffffU
#define EXAMPLE_LPSPI_MASTER_DMA_BASEADDR DMA0
#define EXAMPLE_LPSPI_DMA_CLOCK           kCLOCK_Dma0
#define EXAMPLE_LPSPI_CLOCK_FREQ          CLOCK_GetLpspiClkFreq(1u)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
