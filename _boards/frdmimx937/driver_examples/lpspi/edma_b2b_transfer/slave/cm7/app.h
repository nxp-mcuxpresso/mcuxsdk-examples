/*
 * Copyright 2026 NXP
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
#define EXAMPLE_LPSPI_SLAVE_BASEADDR         LPSPI7
#define EXAMPLE_LPSPI_SLAVE_IRQN             LPSPI7_IRQn
#define EXAMPLE_LPSPI_SLAVE_PCS_FOR_INIT     kLPSPI_Pcs0
#define EXAMPLE_LPSPI_SLAVE_PCS_FOR_TRANSFER kLPSPI_SlavePcs0

#define LPSPI_SLAVE_CLOCK_ROOT kCLOCK_lpspi7

#define DEMO_LPSPI_TRANSMIT_EDMA_CHANNEL   Dma5RequestMuxLPSPI7Tx
#define DEMO_LPSPI_RECEIVE_EDMA_CHANNEL    Dma5RequestMuxLPSPI7Rx
#define EXAMPLE_LPSPI_SLAVE_DMA_BASE       EDMA5_2
#define EXAMPLE_LPSPI_SLAVE_DMA_RX_CHANNEL 0
#define EXAMPLE_LPSPI_SLAVE_DMA_TX_CHANNEL 1
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
