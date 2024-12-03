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
#define EXAMPLE_QSPI_SLAVE_BASEADDR        QSPI0
#define EXAMPLE_QSPI_DMA_RX_REQUEST_SOURCE kDmaCH0_SPI0_RF
#define EXAMPLE_QSPI_DMA_TX_REQUEST_SOURCE kDmaCH1_SPI0_TE
#define EXAMPLE_QSPI_DMA_BASE              (DMA0)
#define EXAMPLE_QSPI_DMA_RX_CHANNEL        kDMA_Channel0
#define EXAMPLE_QSPI_DMA_TX_CHANNEL        kDMA_Channel1
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
