/*
 * Copyright 2025 NXP
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

#define LPSPI_MASTER_CLK_FREQ                  CLOCK_GetFreq(kCLOCK_Lpspi1Clk)
#define EXAMPLE_LPSPI_MASTER_BASEADDR          (LPSPI_1)
#define EXAMPLE_LPSPI_MASTER_PCS_FOR_INIT      (kLPSPI_Pcs1)
#define EXAMPLE_LPSPI_MASTER_PCS_FOR_TRANSFER  (kLPSPI_MasterPcs1)
#define EXAMPLE_LPSPI_RX_MASTER_DMA_MUX_BASE   DMAMUX_0
#define EXAMPLE_LPSPI_TX_MASTER_DMA_MUX_BASE   DMAMUX_0
#define EXAMPLE_LPSPI_MASTER_DMA_BASE          EDMA
#define EXAMPLE_LPSPI_MASTER_DMAMUX_TX_CHANNEL 0U
#define EXAMPLE_LPSPI_MASTER_DMAMUX_RX_CHANNEL 1U
#define EXAMPLE_LPSPI_MASTER_DMA_TX_CHANNEL    0U
#define EXAMPLE_LPSPI_MASTER_DMA_RX_CHANNEL    1U
#define EXAMPLE_LPSPI_MASTER_DMA_TX_REQUEST_SOURCE kDmaRequestMux0LPSPI1Tx
#define EXAMPLE_LPSPI_MASTER_DMA_RX_REQUEST_SOURCE kDmaRequestMux0LPSPI1Rx
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
