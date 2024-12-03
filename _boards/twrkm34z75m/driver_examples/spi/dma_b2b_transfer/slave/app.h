/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_SPI_SLAVE        SPI0
#define EXAMPLE_DMA              DMA0
#define EXAMPLE_DMAMUX           DMAMUX
#define EXAMPLE_SPI_TX_CHANNEL   3U
#define EXAMPLE_SPI_RX_CHANNEL   2U
#define EXAMPLE_SPI_TX_SOURCE    kDmaRequestMux0SPI0Tx
#define EXAMPLE_SPI_RX_SOURCE    kDmaRequestMux0SPI0Rx
#define EXAMPLE_SPI_SOURCE_CLOCK kCLOCK_CoreSysClk
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
