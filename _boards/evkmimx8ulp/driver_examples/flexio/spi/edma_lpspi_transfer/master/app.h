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
/*Master related*/
#define TRANSFER_SIZE     256U    /*! Transfer dataSize */
#define TRANSFER_BAUDRATE 400000U /*! Transfer baudrate - 400k */

#define MASTER_FLEXIO_SPI_BASEADDR            FLEXIO0
#define FLEXIO_SPI_SOUT_PIN                   17U
#define FLEXIO_SPI_SIN_PIN                    18U
#define FLEXIO_SPI_CLK_PIN                    19U
#define FLEXIO_SPI_PCS_PIN                    16U
#define MASTER_FLEXIO_SPI_IRQ                 FLEXIO0_IRQn
#define MASTER_FLEXIO_SPI_CLOCK_FREQUENCY     CLOCK_GetIpFreq(kCLOCK_Flexio0)
#define FLEXIO_DMA_REQUEST_SOURCE_BASE        kDmaRequestMux0FlexIO0Shifter0
#define EXAMPLE_FLEXIO_SPI_DMA_LPSPI_BASEADDR DMA0
#define FLEXIO_SPI_TX_DMA_LPSPI_CHANNEL       0U
#define FLEXIO_SPI_RX_DMA_LPSPI_CHANNEL       1U
#define FLEXIO_TX_SHIFTER_INDEX               0U
#define FLEXIO_RX_SHIFTER_INDEX               1U
#define EXAMPLE_TX_DMA_SOURCE                 (FLEXIO_DMA_REQUEST_SOURCE_BASE + FLEXIO_TX_SHIFTER_INDEX)
#define EXAMPLE_RX_DMA_SOURCE                 (FLEXIO_DMA_REQUEST_SOURCE_BASE + FLEXIO_RX_SHIFTER_INDEX)
#define EXAMPLE_TX_DMA_CHANNEL                kCLOCK_Dma0Ch0
#define EXAMPLE_RX_DMA_CHANNEL                kCLOCK_Dma0Ch1

/*Slave related*/
#define SLAVE_LPSPI_BASEADDR         LPSPI1
#define SLAVE_LPSPI_IRQ_HANDLE       LPSPI1_DriverIRQHandler
#define SLAVE_LPSPI_IRQN             LPSPI1_IRQn
#define SLAVE_LPSPI_PCS_FOR_INIT     kLPSPI_Pcs0
#define SLAVE_LPSPI_PCS_FOR_TRANSFER kLPSPI_SlavePcs0
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
