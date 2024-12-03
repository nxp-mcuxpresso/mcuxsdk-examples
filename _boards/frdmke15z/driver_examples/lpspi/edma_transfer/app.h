/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
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
/*Master related*/
#define EXAMPLE_LPSPI_MASTER_BASEADDR         LPSPI1
#define EXAMPLE_LPSPI_MASTER_PCS_FOR_INIT     kLPSPI_Pcs3
#define EXAMPLE_LPSPI_MASTER_PCS_FOR_TRANSFER kLPSPI_MasterPcs3

#define EXAMPLE_LPSPI_MASTER_CLOCK_NAME   (kCLOCK_Lpspi1)
#define EXAMPLE_LPSPI_MASTER_CLOCK_SOURCE (kCLOCK_IpSrcFircAsync)
#define EXAMPLE_LPSPI_MASTER_CLOCK_FREQ   (CLOCK_GetIpFreq(EXAMPLE_LPSPI_MASTER_CLOCK_NAME))

#define EXAMPLE_LPSPI_MASTER_DMA_MUX_BASE          DMAMUX_BASE
#define EXAMPLE_LPSPI_MASTER_DMA_RX_REQUEST_SOURCE kDmaRequestMux0LPSPI1Rx
#define EXAMPLE_LPSPI_MASTER_DMA_TX_REQUEST_SOURCE kDmaRequestMux0LPSPI1Tx
#define EXAMPLE_LPSPI_MASTER_DMA_BASE              DMA_BASE
#define EXAMPLE_LPSPI_MASTER_DMA_RX_CHANNEL        0U
#define EXAMPLE_LPSPI_MASTER_DMA_TX_CHANNEL        1U
/*Slave related*/
#define EXAMPLE_LPSPI_SLAVE_BASEADDR         LPSPI0
#define EXAMPLE_LPSPI_SLAVE_PCS_FOR_INIT     kLPSPI_Pcs3
#define EXAMPLE_LPSPI_SLAVE_PCS_FOR_TRANSFER kLPSPI_SlavePcs3

#define EXAMPLE_LPSPI_SLAVE_CLOCK_NAME   (kCLOCK_Lpspi0)
#define EXAMPLE_LPSPI_SLAVE_CLOCK_SOURCE (kCLOCK_IpSrcFircAsync)

#define EXAMPLE_LPSPI_SLAVE_DMA_MUX_BASE          DMAMUX_BASE
#define EXAMPLE_LPSPI_SLAVE_DMA_RX_REQUEST_SOURCE kDmaRequestMux0LPSPI0Rx
#define EXAMPLE_LPSPI_SLAVE_DMA_TX_REQUEST_SOURCE kDmaRequestMux0LPSPI0Tx
#define EXAMPLE_LPSPI_SLAVE_DMA_BASE              DMA_BASE
#define EXAMPLE_LPSPI_SLAVE_DMA_RX_CHANNEL        3U
#define EXAMPLE_LPSPI_SLAVE_DMA_TX_CHANNEL        4U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
