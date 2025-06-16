/*
 * Copyright 2025 NXP
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
/* Master related */
#define MASTER_FLEXIO_SPI_BASEADDR  (FLEXIO)
#define FLEXIO_SPI_SOUT_PIN    0U
#define FLEXIO_SPI_SIN_PIN     1U
#define FLEXIO_SPI_CLK_PIN     2U
#define FLEXIO_SPI_PCS_PIN     3U
#define FLEXIO_CLOCK_FREQUENCY CLOCK_GetFreq(kCLOCK_FlexioClk)
#define MASTER_FLEXIO_SPI_CLOCK_FREQUENCY  CLOCK_GetFreq(kCLOCK_FlexioClk)
/*Slave related*/
#define SLAVE_LPSPI_BASEADDR         (LPSPI_0)
#define SLAVE_LPSPI_IRQN             (LPSPI0_IRQn)
#define SLAVE_LPSPI_PCS_FOR_INIT     (kLPSPI_Pcs2)
#define SLAVE_LPSPI_PCS_FOR_TRANSFER (kLPSPI_SlavePcs2)

#define EXAMPLE_FLEXIO_SPI_DMA_LPSPI_BASEADDR EDMA
#define EXAMPLE_FLEXIO_SPI_TX_DMAMUX_BASEADDR DMAMUX_0
#define EXAMPLE_FLEXIO_SPI_RX_DMAMUX_BASEADDR DMAMUX_0
#define FLEXIO_SPI_TX_DMA_LPSPI_CHANNEL    (0U)
#define FLEXIO_SPI_RX_DMA_LPSPI_CHANNEL    (1U)
#define FLEXIO_SPI_TX_DMAMUX_LPSPI_CHANNEL (0U)
#define FLEXIO_SPI_RX_DMAMUX_LPSPI_CHANNEL (1U)
#define FLEXIO_TX_SHIFTER_INDEX            (0U)
#define FLEXIO_RX_SHIFTER_INDEX            (2U)
#define EXAMPLE_TX_DMA_SOURCE              kDmaRequestMux0FLEXIOChannel0
#define EXAMPLE_RX_DMA_SOURCE              kDmaRequestMux0FLEXIOChannel2
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
