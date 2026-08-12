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
#define EXAMPLE_LPSPI_MASTER_BASEADDR         LPSPI3
#define LPSPI_MASTER_CLOCK_ROOT               kCLOCK_Root_Lpspi3
#define LPSPI_MASTER_CLOCK_GATE               kCLOCK_Lpspi3
#define LPSPI_MASTER_CLK_FREQ                 (CLOCK_GetIpFreq(LPSPI_MASTER_CLOCK_ROOT))
#define EXAMPLE_LPSPI_MASTER_PCS_FOR_INIT     kLPSPI_Pcs0
#define EXAMPLE_LPSPI_MASTER_PCS_FOR_TRANSFER kLPSPI_MasterPcs0

#define DEMO_LPSPI_TRANSMIT_EDMA_CHANNEL    kDma4RequestMuxLPSPI3Tx
#define DEMO_LPSPI_RECEIVE_EDMA_CHANNEL     kDma4RequestMuxLPSPI3Rx
#define EXAMPLE_LPSPI_MASTER_DMA_BASE       DMA4
#define EXAMPLE_DMA_CLOCK_ROOT              kCLOCK_Root_WakeupAxi
#define EXAMPLE_DMA_CLOCK_GATE              kCLOCK_Edma1
#define EXAMPLE_LPSPI_MASTER_DMA_RX_CHANNEL kDma4RequestMuxLPSPI3Rx
#define EXAMPLE_LPSPI_MASTER_DMA_TX_CHANNEL kDma4RequestMuxLPSPI3Tx

#define EXAMPLE_LPSPI_DEALY_COUNT 0xfffff
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
