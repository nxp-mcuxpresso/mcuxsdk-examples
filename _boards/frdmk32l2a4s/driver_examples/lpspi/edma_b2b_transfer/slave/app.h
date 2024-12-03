/*
 * Copyright 2019 NXP
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
#define EXAMPLE_LPSPI_SLAVE_BASEADDR         LPSPI0
#define EXAMPLE_LPSPI_SLAVE_IRQN             LPSPI0_IRQn
#define EXAMPLE_LPSPI_SLAVE_PCS_FOR_INIT     kLPSPI_Pcs0
#define EXAMPLE_LPSPI_SLAVE_PCS_FOR_TRANSFER kLPSPI_SlavePcs0

#define EXAMPLE_LPSPI_SLAVE_CLOCK_NAME   (kCLOCK_Lpspi0)
#define EXAMPLE_LPSPI_SLAVE_CLOCK_SOURCE (kCLOCK_IpSrcFircAsync)

#define EXAMPLE_LPSPI_SLAVE_DMA_MUX_BASE          DMAMUX0
#define EXAMPLE_LPSPI_SLAVE_DMA_RX_REQUEST_SOURCE kDmaRequestMux0LPSPI0Rx
#define EXAMPLE_LPSPI_SLAVE_DMA_TX_REQUEST_SOURCE kDmaRequestMux0LPSPI0Tx
#define EXAMPLE_LPSPI_SLAVE_DMA_BASE              DMA0
#define EXAMPLE_LPSPI_SLAVE_DMA_RX_CHANNEL        1U
#define EXAMPLE_LPSPI_SLAVE_DMA_TX_CHANNEL        2U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
