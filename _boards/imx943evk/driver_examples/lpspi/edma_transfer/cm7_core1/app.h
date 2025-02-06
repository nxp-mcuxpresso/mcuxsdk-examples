/*
 * Copyright 2024 NXP
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
#define EXAMPLE_LPSPI_MASTER_BASEADDR (LPSPI8)
#define EXAMPLE_LPSPI_MASTER_DMA_BASE (EDMA3_BASE)
#define EXAMPLE_LPSPI_MASTER_DMA_RX_CHANNEL 1U
#define EXAMPLE_LPSPI_MASTER_DMA_TX_CHANNEL 2U
#define EXAMPLE_LPSPI_MASTER_DMA_RX_REQ kDma3RequestMuxLpspi8Rx
#define EXAMPLE_LPSPI_MASTER_DMA_TX_REQ kDma3RequestMuxLpspi8Tx

#define EXAMPLE_LPSPI_MASTER_PCS_FOR_INIT (kLPSPI_Pcs0)
#define EXAMPLE_LPSPI_MASTER_PCS_FOR_TRANSFER (kLPSPI_MasterPcs0)

#define EXAMPLE_LPSPI_MASTER_CLK_ID (hal_clock_lpspi8)

/* Slave related */
#define EXAMPLE_LPSPI_SLAVE_BASEADDR (LPSPI3)
#define EXAMPLE_LPSPI_SLAVE_DMA_BASE (EDMA3_BASE)
#define EXAMPLE_LPSPI_SLAVE_DMA_RX_CHANNEL 3U
#define EXAMPLE_LPSPI_SLAVE_DMA_TX_CHANNEL 4U
#define EXAMPLE_LPSPI_SLAVE_DMA_RX_REQ kDma3RequestMuxLpspi3Rx
#define EXAMPLE_LPSPI_SLAVE_DMA_TX_REQ kDma3RequestMuxLpspi3Tx

#define EXAMPLE_LPSPI_SLAVE_PCS_FOR_INIT (kLPSPI_Pcs0)
#define EXAMPLE_LPSPI_SLAVE_PCS_FOR_TRANSFER (kLPSPI_SlavePcs0)

#define EXAMPLE_LPSPI_SLAVE_CLK_ID (hal_clock_lpspi3)

#define EXAMPLE_LPSPI_MASTER_CLOCK_FREQ                                        \
  HAL_ClockGetRate(EXAMPLE_LPSPI_MASTER_CLK_ID)
#define EXAMPLE_LPSPI_SLAVE_CLOCK_FREQ                                         \
  HAL_ClockGetRate(EXAMPLE_LPSPI_SLAVE_CLK_ID)

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif
