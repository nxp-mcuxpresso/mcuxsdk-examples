/*
 * Copyright 2021 NXP
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
#define EXAMPLE_SPI_SLAVE SPI1
#define EXAMPLE_DMA       DMA0

#define EXAMPLE_TX_DMAMUX               DMAMUX3
#define EXAMPLE_RX_DMAMUX               DMAMUX2
#define EXAMPLE_SPI_TX_DMAMUX_CHANNEL   0U
#define EXAMPLE_SPI_RX_DMAMUX_CHANNEL   0U
#define EXAMPLE_SPI_TX_DMA_CHANNEL      3U
#define EXAMPLE_SPI_RX_DMA_CHANNEL      2U
#define EXAMPLE_SPI_TX_SOURCE           kDmaRequestMux2SPI1Tx
#define EXAMPLE_SPI_RX_SOURCE           kDmaRequestMux2SPI1Rx
#define EXAMPLE_SPI_MASTER_SOURCE_CLOCK kCLOCK_CoreSysClk

// slaveCallback
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
