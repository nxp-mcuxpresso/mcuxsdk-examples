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
#define EXAMPLE_LPSPI_MASTER_BASEADDR         LPSPI1
#define LPSPI_MASTER_CLOCK_ROOT               hal_clock_lpspi1
#define LPSPI_MASTER_CLK_FREQ                 (HAL_ClockGetIpFreq(LPSPI_MASTER_CLOCK_ROOT))
#define EXAMPLE_LPSPI_MASTER_PCS_FOR_INIT     kLPSPI_Pcs0
#define EXAMPLE_LPSPI_MASTER_PCS_FOR_TRANSFER kLPSPI_MasterPcs0

#define DEMO_LPSPI_TRANSMIT_EDMA_CHANNEL    Dma3RequestMuxLPSPI1Tx
#define DEMO_LPSPI_RECEIVE_EDMA_CHANNEL     Dma3RequestMuxLPSPI1Rx
#define EXAMPLE_LPSPI_MASTER_DMA_BASE       DMA3
#define EXAMPLE_LPSPI_MASTER_DMA_RX_CHANNEL Dma3RequestMuxLPSPI1Rx
#define EXAMPLE_LPSPI_MASTER_DMA_TX_CHANNEL Dma3RequestMuxLPSPI1Tx

#define EXAMPLE_LPSPI_DEALY_COUNT 0xfffff
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
