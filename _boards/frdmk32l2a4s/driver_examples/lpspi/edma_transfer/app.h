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
/*Master related*/
#define EXAMPLE_LPSPI_MASTER_BASEADDR (LPSPI0)

#define EXAMPLE_LPSPI_MASTER_CLOCK_NAME   (kCLOCK_Lpspi0)
#define EXAMPLE_LPSPI_MASTER_CLOCK_SOURCE (kCLOCK_IpSrcFircAsync)
#define EXAMPLE_LPSPI_MASTER_CLOCK_FREQ   (CLOCK_GetIpFreq(EXAMPLE_LPSPI_MASTER_CLOCK_NAME))

#define EXAMPLE_LPSPI_MASTER_PCS_FOR_INIT     (kLPSPI_Pcs0)
#define EXAMPLE_LPSPI_MASTER_PCS_FOR_TRANSFER (kLPSPI_MasterPcs0)

#define EXAMPLE_LPSPI_MASTER_DMA_MUX_BASE          (DMAMUX0_BASE)
#define EXAMPLE_LPSPI_MASTER_DMA_RX_REQUEST_SOURCE kDmaRequestMux0LPSPI0Rx
#define EXAMPLE_LPSPI_MASTER_DMA_TX_REQUEST_SOURCE kDmaRequestMux0LPSPI0Tx
#define EXAMPLE_LPSPI_MASTER_DMA_BASE              (DMA0_BASE)
#define EXAMPLE_LPSPI_MASTER_DMA_RX_CHANNEL        0U
#define EXAMPLE_LPSPI_MASTER_DMA_TX_CHANNEL        1U
/*Slave related*/
#define EXAMPLE_LPSPI_SLAVE_BASEADDR (LPSPI2)

#define EXAMPLE_LPSPI_SLAVE_CLOCK_NAME   (kCLOCK_Lpspi2)
#define EXAMPLE_LPSPI_SLAVE_CLOCK_SOURCE (kCLOCK_IpSrcFircAsync)

#define EXAMPLE_LPSPI_SLAVE_PCS_FOR_INIT     (kLPSPI_Pcs0)
#define EXAMPLE_LPSPI_SLAVE_PCS_FOR_TRANSFER (kLPSPI_SlavePcs0)

#define EXAMPLE_LPSPI_SLAVE_DMA_MUX_BASE          (DMAMUX0_BASE)
#define EXAMPLE_LPSPI_SLAVE_DMA_RX_REQUEST_SOURCE kDmaRequestMux0LPSPI2Rx
#define EXAMPLE_LPSPI_SLAVE_DMA_TX_REQUEST_SOURCE kDmaRequestMux0LPSPI2Tx
#define EXAMPLE_LPSPI_SLAVE_DMA_BASE              (DMA0_BASE)
#define EXAMPLE_LPSPI_SLAVE_DMA_RX_CHANNEL        3U
#define EXAMPLE_LPSPI_SLAVE_DMA_TX_CHANNEL        4U

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif
