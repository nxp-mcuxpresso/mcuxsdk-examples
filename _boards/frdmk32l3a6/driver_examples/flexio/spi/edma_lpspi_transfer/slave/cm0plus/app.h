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
/*Master related*/
#define MASTER_LPSPI_BASEADDR   (LPSPI0)
#define MASTER_LPSPI_IRQ_HANDLE (LPSPI0_DriverIRQHandler)
#define MASTER_LPSPI_IRQN       (LPSPI0_IRQn)

#define MASTER_LPSPI_PCS_FOR_INIT     (kLPSPI_Pcs0)
#define MASTER_LPSPI_PCS_FOR_TRANSFER (kLPSPI_MasterPcs0)

#define MASTER_LPSPI_CLOCK_NAME      (kCLOCK_Lpspi0)
#define MASTER_LPSPI_CLOCK_SOURCE    (kCLOCK_IpSrcFircAsync)
#define MASTER_LPSPI_CLOCK_FREQUENCY (CLOCK_GetIpFreq(MASTER_LPSPI_CLOCK_NAME))

/*Slave related*/
#define SLAVE_FLEXIO_SPI_BASEADDR (FLEXIO0)
#define FLEXIO_SPI_SOUT_PIN       9U
#define FLEXIO_SPI_SIN_PIN        3U
#define FLEXIO_SPI_CLK_PIN        5U
#define FLEXIO_SPI_PCS_PIN        10U

#define SLAVE_FLEXIO_SPI_IRQ          FLEXIO0_IRQn
#define SLAVE_FLEXIO_SPI_CLOCK_NAME   (kCLOCK_Flexio0)
#define SLAVE_FLEXIO_SPI_CLOCK_SOURCE (kCLOCK_IpSrcFircAsync)

#define FLEXIO_DMA_REQUEST_SOURCE_BASE        (kDmaRequestMux1FLEXIO0Channel0)
#define EXAMPLE_FLEXIO_SPI_DMAMUX_BASEADDR    DMAMUX1
#define EXAMPLE_FLEXIO_SPI_DMA_LPSPI_BASEADDR DMA1
#define FLEXIO_SPI_TX_DMA_LPSPI_CHANNEL       0U
#define FLEXIO_SPI_RX_DMA_LPSPI_CHANNEL       1U
#define FLEXIO_TX_SHIFTER_INDEX               0U
#define FLEXIO_RX_SHIFTER_INDEX               1U
#define EXAMPLE_TX_DMA_SOURCE                 (FLEXIO_DMA_REQUEST_SOURCE_BASE + FLEXIO_TX_SHIFTER_INDEX)
#define EXAMPLE_RX_DMA_SOURCE                 (FLEXIO_DMA_REQUEST_SOURCE_BASE + FLEXIO_RX_SHIFTER_INDEX)

/*INTMUX setting*/
#define EXAMPLE_LPSPI_INTMUX_CHANNLE (0U)

#define TRANSFER_SIZE     256U    /*! Transfer dataSize */
#define TRANSFER_BAUDRATE 400000U /*! Transfer baudrate - 400k */

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif
