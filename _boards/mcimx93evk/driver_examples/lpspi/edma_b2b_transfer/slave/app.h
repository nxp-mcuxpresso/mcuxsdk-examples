/*
 * Copyright 2022 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_LPSPI_SLAVE_BASEADDR         LPSPI3
#define EXAMPLE_LPSPI_SLAVE_IRQN             LPSPI3_IRQn
#define EXAMPLE_LPSPI_SLAVE_PCS_FOR_INIT     kLPSPI_Pcs0
#define EXAMPLE_LPSPI_SLAVE_PCS_FOR_TRANSFER kLPSPI_SlavePcs0

#define LPSPI_SLAVE_CLOCK_ROOT kCLOCK_Root_Lpspi3
#define LPSPI_SLAVE_CLOCK_GATE kCLOCK_Lpspi3

#define DEMO_LPSPI_TRANSMIT_EDMA_CHANNEL   kDma4RequestMuxLPSPI3Tx
#define DEMO_LPSPI_RECEIVE_EDMA_CHANNEL    kDma4RequestMuxLPSPI3Rx
#define EXAMPLE_LPSPI_SLAVE_DMA_BASE       DMA4
#define EXAMPLE_DMA_CLOCK_ROOT             kCLOCK_Root_WakeupAxi
#define EXAMPLE_DMA_CLOCK_GATE             kCLOCK_Edma1
#define EXAMPLE_LPSPI_SLAVE_DMA_RX_CHANNEL kDma4RequestMuxLPSPI3Rx
#define EXAMPLE_LPSPI_SLAVE_DMA_TX_CHANNEL kDma4RequestMuxLPSPI3Tx
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
