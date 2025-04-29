/*
 * Copyright 2025 NXP
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
#define DRIVER_SLAVE_SPI                     Driver_SPI1
#define EXAMPLE_LPSPI_DEALY_COUNT            0xfffffU
#define EXAMPLE_LPSPI_SLAVE_DMA_MUX_BASEADDR DMAMUX_0
#define EXAMPLE_LPSPI_SLAVE_DMA_BASEADDR     EDMA
#define EXAMPLE_LPSPI_CLOCK_FREQ             (CLOCK_GetFreq(kCLOCK_Lpspi1Clk))
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
