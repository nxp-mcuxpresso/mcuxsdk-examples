/*
 * Copyright 2020 NXP
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
#define EXAMPLE_QSPI_MASTER_BASEADDR QSPI0
#define EXAMPLE_QSPI_CLK_FREQ        CLOCK_GetIpClkSrcFreq(kCLOCK_QSPI0)

#define EXAMPLE_QSPI_DMA_MUX_BASE          (DMAMUX)
#define EXAMPLE_QSPI_DMA_RX_REQUEST_SOURCE kDmaRequestMuxSPI0RF
#define EXAMPLE_QSPI_DMA_TX_REQUEST_SOURCE kDmaRequestMuxSPI0TE
#define EXAMPLE_QSPI_DMA_BASE              (DMA0)
#define EXAMPLE_QSPI_DMA_RX_CHANNEL        kEDMA_Channel0
#define EXAMPLE_QSPI_DMA_TX_CHANNEL        kEDMA_Channel1
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
