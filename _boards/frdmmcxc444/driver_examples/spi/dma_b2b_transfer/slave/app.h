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
#define EXAMPLE_SPI_SLAVE      SPI1
#define EXAMPLE_DMA            DMA0
#define EXAMPLE_DMAMUX         DMAMUX0
#define EXAMPLE_SPI_TX_CHANNEL 0U
#define EXAMPLE_SPI_RX_CHANNEL 1U
#define EXAMPLE_SPI_TX_SOURCE  kDmaRequestMux0SPI1Tx
#define EXAMPLE_SPI_RX_SOURCE  kDmaRequestMux0SPI1Rx
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
