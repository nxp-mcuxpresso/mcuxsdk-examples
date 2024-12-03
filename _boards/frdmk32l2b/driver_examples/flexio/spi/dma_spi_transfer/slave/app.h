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
#define BOARD_FLEXIO_BASE                  FLEXIO
#define FLEXIO_SPI_MOSI_PIN                6U
#define FLEXIO_SPI_MISO_PIN                7U
#define FLEXIO_SPI_SCK_PIN                 5U
#define FLEXIO_SPI_PCS0_PIN                4U
#define FLEXIO_CLOCK_FREQUENCY             48000000U
#define FLEXIO_DMA_REQUEST_BASE            kDmaRequestMux0FlexIOChannel0
#define EXAMPLE_FLEXIO_SPI_DMAMUX_BASEADDR DMAMUX0
#define EXAMPLE_FLEXIO_SPI_DMA_BASEADDR    DMA0
#define FLEXIO_SPI_TX_DMA_CHANNEL          0U
#define FLEXIO_SPI_RX_DMA_CHANNEL          1U
#define FLEXIO_TX_SHIFTER_INDEX            0U
#define FLEXIO_RX_SHIFTER_INDEX            1U
#define EXAMPLE_TX_DMA_SOURCE              (FLEXIO_DMA_REQUEST_BASE + FLEXIO_TX_SHIFTER_INDEX)
#define EXAMPLE_RX_DMA_SOURCE              (FLEXIO_DMA_REQUEST_BASE + FLEXIO_RX_SHIFTER_INDEX)

#define BOARD_SPI_MASTER_BASE SPI0
#define SPI_MASTER_CLK_SRC    SPI0_CLK_SRC
#define SPI_MASTER_CLK_FREQ   CLOCK_GetFreq(SPI0_CLK_SRC)
#define SLAVE_SPI_IRQ         UART2_FLEXIO_IRQn
#define MASTER_SPI_IRQ        SPI0_IRQn
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
