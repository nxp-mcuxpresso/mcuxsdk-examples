/*
 * Copyright 2026 NXP
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
#define DRIVER_MASTER_SPI                 Driver_SPI16
#define EXAMPLE_LPSPI_MASTER_DMA_BASEADDR DMA0
#define EXAMPLE_LPSPI_CLOCK_FREQ          (CLOCK_GetFreq(kCLOCK_LPSpi16Clk))
#define EXAMPLE_LPSPI_MASTER_DMA_BASE         DMA0
#define DEMO_LPSPI_TRANSMIT_EDMA_CHANNEL      kDmaRequestMuxLpspi16Tx
#define DEMO_LPSPI_RECEIVE_EDMA_CHANNEL       kDmaRequestMuxLpspi16Rx
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif
