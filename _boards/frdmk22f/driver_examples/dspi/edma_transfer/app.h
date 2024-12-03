/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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
#define EXAMPLE_DSPI_MASTER_BASEADDR              SPI0
#define EXAMPLE_DSPI_MASTER_DMA_MUX_BASE          DMAMUX_BASE
#define EXAMPLE_DSPI_MASTER_DMA_BASE              DMA_BASE
#define EXAMPLE_DSPI_MASTER_DMA_RX_REQUEST_SOURCE kDmaRequestMux0SPI0Rx
#define EXAMPLE_DSPI_MASTER_DMA_TX_REQUEST_SOURCE kDmaRequestMux0SPI0Tx
#define DSPI_MASTER_CLK_SRC                       DSPI0_CLK_SRC
#define DSPI_MASTER_CLK_FREQ                      CLOCK_GetFreq(DSPI0_CLK_SRC)
#define EXAMPLE_DSPI_MASTER_PCS_FOR_INIT          kDSPI_Pcs0
#define EXAMPLE_DSPI_MASTER_PCS_FOR_TRANSFER      kDSPI_MasterPcs0

#define EXAMPLE_DSPI_SLAVE_BASEADDR     SPI1
#define EXAMPLE_DSPI_SLAVE_DMA_MUX_BASE DMAMUX_BASE
#define EXAMPLE_DSPI_SLAVE_DMA_BASE     DMA_BASE
/* SPI1 has not separated dma request source*/
#define EXAMPLE_DSPI_SLAVE_DMA_RX_REQUEST_SOURCE kDmaRequestMux0SPI1
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
