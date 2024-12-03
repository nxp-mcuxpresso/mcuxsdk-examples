/*
 * Copyright 2024 NXP
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
#define EXAMPLE_SPI_MASTER              SPI0
#define EXAMPLE_DMA                     DMA0
#define EXAMPLE_DMAMUX                  DMAMUX0
#define EXAMPLE_SPI_TX_CHANNEL          0U
#define EXAMPLE_SPI_RX_CHANNEL          1U
#define EXAMPLE_SPI_TX_SOURCE           kDmaRequestMux0SPI0Tx
#define EXAMPLE_SPI_RX_SOURCE           kDmaRequestMux0SPI0Rx
#define EXAMPLE_SPI_MASTER_SOURCE_CLOCK kCLOCK_BusClk
#define EXAMPLE_SPI_MASTER_CLK_FREQ     CLOCK_GetFreq(kCLOCK_BusClk)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
