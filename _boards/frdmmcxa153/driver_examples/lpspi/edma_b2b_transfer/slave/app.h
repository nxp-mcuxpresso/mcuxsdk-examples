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
#define EXAMPLE_LPSPI_SLAVE_BASEADDR         (LPSPI1)
#define EXAMPLE_LPSPI_SLAVE_PCS_FOR_INIT     (kLPSPI_Pcs1)
#define EXAMPLE_LPSPI_SLAVE_PCS_FOR_TRANSFER (kLPSPI_SlavePcs1)
#define EXAMPLE_LPSPI_SLAVE_DMA_BASE         DMA0
#define EXAMPLE_LPSPI_SLAVE_DMA_RX_CHANNEL   1U
#define EXAMPLE_LPSPI_SLAVE_DMA_TX_CHANNEL   0U
#define DEMO_LPSPI_TRANSMIT_EDMA_CHANNEL     kDma0RequestLPSPI1Tx
#define DEMO_LPSPI_RECEIVE_EDMA_CHANNEL      kDma0RequestLPSPI1Rx
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
