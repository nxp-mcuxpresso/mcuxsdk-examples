/*
 * Copyright 2026 NXP
 *
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
#define TRANSFER_SIZE     256U    /*! Transfer dataSize */
#define TRANSFER_BAUDRATE 500000U /*! Transfer baudrate - 500k */

#define MASTER_FLEXIO_SPI_BASEADDR (FLEXIO1)
#define FLEXIO_SPI_SOUT_PIN        0U
#define FLEXIO_SPI_SIN_PIN         1U
#define FLEXIO_SPI_CLK_PIN         2U
#define FLEXIO_SPI_PCS_PIN         3U

#define MASTER_FLEXIO_SPI_IRQ FLEXIO1_IRQn

#define MASTER_FLEXIO_SPI_CLOCK_FREQUENCY (CLOCK_GetRootClockFreq(kCLOCK_Root_Flexio1))

#define FLEXIO_DMA_REQUEST_SOURCE_BASE        (kDmaRequestMuxFlexIO1Request0Request1)
#define EXAMPLE_FLEXIO_SPI_DMAMUX_BASEADDR    DMAMUX0
#define EXAMPLE_FLEXIO_SPI_DMA_LPSPI_BASEADDR DMA0
#define FLEXIO_SPI_TX_DMA_LPSPI_CHANNEL       (0U)
#define FLEXIO_SPI_RX_DMA_LPSPI_CHANNEL       (1U)
#define FLEXIO_TX_SHIFTER_INDEX               0U
#define FLEXIO_RX_SHIFTER_INDEX               2U
#define EXAMPLE_TX_DMA_SOURCE                 (kDmaRequestMuxFlexIO1Request0Request1)
#define EXAMPLE_RX_DMA_SOURCE                 (kDmaRequestMuxFlexIO1Request2Request3)

/*Slave related*/
#define SLAVE_LPSPI_BASEADDR   (LPSPI1)
#define SLAVE_LPSPI_IRQ_HANDLE (LPSPI1_DriverIRQHandler)
#define SLAVE_LPSPI_IRQN       (LPSPI1_IRQn)

#define SLAVE_LPSPI_PCS_FOR_INIT     (kLPSPI_Pcs0)
#define SLAVE_LPSPI_PCS_FOR_TRANSFER (kLPSPI_SlavePcs0)

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif
