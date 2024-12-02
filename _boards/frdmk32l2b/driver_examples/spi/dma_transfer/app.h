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
#define EXAMPLE_SPI_MASTER               SPI0
#define EXAMPLE_SPI_SLAVE                SPI1
#define EXAMPLE_DMA                      DMA0
#define EXAMPLE_DMAMUX                   DMAMUX0
#define EXAMPLE_SPI_MASTER_TX_CHANNEL    0U
#define EXAMPLE_SPI_MASTER_RX_CHANNEL    1U
#define EXAMPLE_SPI_SLAVE_TX_CHANNEL     2U
#define EXAMPLE_SPI_SLAVE_RX_CHANNEL     3U
#define EXAMPLE_DMA_SPI_MASTER_TX_SOURCE kDmaRequestMux0SPI0Tx
#define EXAMPLE_DMA_SPI_MASTER_RX_SOURCE kDmaRequestMux0SPI0Rx
#define EXAMPLE_DMA_SPI_SLAVE_TX_SOURCE  kDmaRequestMux0SPI1Tx
#define EXAMPLE_DMA_SPI_SLAVE_RX_SOURCE  kDmaRequestMux0SPI1Rx
#define EXAMPLE_SPI_MASTER_SOURCE_CLOCK  kCLOCK_BusClk
#define EXAMPLE_SPI_MASTER_CLK_FREQ      CLOCK_GetFreq(kCLOCK_BusClk)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
