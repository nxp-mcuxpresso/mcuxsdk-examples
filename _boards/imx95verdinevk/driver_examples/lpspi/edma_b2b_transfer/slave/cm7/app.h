/*
 * Copyright 2024 NXP
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
#define EXAMPLE_LPSPI_SLAVE_BASEADDR         LPSPI1
#define EXAMPLE_LPSPI_SLAVE_IRQN             LPSPI1_IRQn
#define EXAMPLE_LPSPI_SLAVE_PCS_FOR_INIT     kLPSPI_Pcs0
#define EXAMPLE_LPSPI_SLAVE_PCS_FOR_TRANSFER kLPSPI_SlavePcs0

#define LPSPI_SLAVE_CLOCK_ROOT hal_clock_lpspi1

#define DEMO_LPSPI_TRANSMIT_EDMA_CHANNEL   Dma3RequestMuxLPSPI1Tx
#define DEMO_LPSPI_RECEIVE_EDMA_CHANNEL    Dma3RequestMuxLPSPI1Rx
#define EXAMPLE_LPSPI_SLAVE_DMA_BASE       DMA3
#define EXAMPLE_LPSPI_SLAVE_DMA_RX_CHANNEL Dma3RequestMuxLPSPI1Rx
#define EXAMPLE_LPSPI_SLAVE_DMA_TX_CHANNEL Dma3RequestMuxLPSPI1Tx
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
