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
#define EXAMPLE_LPSPI_SLAVE_CLOCK_NAME       (kCLOCK_Lpspi1)
#define EXAMPLE_LPSPI_SLAVE_CLOCK_SOURCE     (kCLOCK_IpSrcFro192M)
#define LPSPI_SLAVE_CLK_FREQ                 (CLOCK_GetIpFreq(EXAMPLE_LPSPI_SLAVE_CLOCK_NAME))
#define EXAMPLE_LPSPI_DEALY_COUNT            0xFFFFF
#define EXAMPLE_LPSPI_SLAVE_BASEADDR         (LPSPI1)
#define EXAMPLE_LPSPI_SLAVE_PCS_FOR_INIT     (kLPSPI_Pcs0)
#define EXAMPLE_LPSPI_SLAVE_PCS_FOR_TRANSFER (kLPSPI_SlavePcs0)
#define EXAMPLE_LPSPI_SLAVE_DMA_BASE         DMA0
#define EXAMPLE_LPSPI_SLAVE_DMA_RX_CHANNEL   0U
#define EXAMPLE_LPSPI_SLAVE_DMA_TX_CHANNEL   1U
#define DEMO_LPSPI_TRANSMIT_EDMA_CHANNEL     kDmaRequestLPSPI1Tx
#define DEMO_LPSPI_RECEIVE_EDMA_CHANNEL      kDmaRequestLPSPI1Rx
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
