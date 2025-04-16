/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Headers
 ******************************************************************************/
/*${header:start}*/
#include "fsl_dmamux.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define BOARD_FLEXIO_BASE   (FLEXIO)
#define FLEXIO_SPI_MOSI_PIN 0U
#define FLEXIO_SPI_MISO_PIN 1U
#define FLEXIO_SPI_SCK_PIN  2U
#define FLEXIO_SPI_CSn_PIN  3U

#define EXAMPLE_FLEXIO_SPI_DMA_BASEADDR EDMA
#define EXAMPLE_FLEXIO_SPI_TX_DMAMUX_BASEADDR DMAMUX_0
#define EXAMPLE_FLEXIO_SPI_RX_DMAMUX_BASEADDR DMAMUX_0
#define FLEXIO_SPI_TX_DMAMUX_CHANNEL    (0U)
#define FLEXIO_SPI_RX_DMAMUX_CHANNEL    (1U)
#define FLEXIO_SPI_TX_DMA_CHANNEL       (0U)
#define FLEXIO_SPI_RX_DMA_CHANNEL       (1U)
#define FLEXIO_TX_SHIFTER_INDEX         (0U)
#define FLEXIO_RX_SHIFTER_INDEX         (2U)
#define EXAMPLE_TX_DMA_SOURCE           kDmaRequestMux0FLEXIOChannel0
#define EXAMPLE_RX_DMA_SOURCE           kDmaRequestMux0FLEXIOChannel2
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
