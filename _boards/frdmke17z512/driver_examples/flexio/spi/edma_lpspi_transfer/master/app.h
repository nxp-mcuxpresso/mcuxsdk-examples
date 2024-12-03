/*
 * Copyright 2023 NXP
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

#define MASTER_FLEXIO_SPI_BASEADDR            FLEXIO
#define FLEXIO_SPI_SOUT_PIN                   0U
#define FLEXIO_SPI_SIN_PIN                    7U
#define FLEXIO_SPI_CLK_PIN                    1U
#define FLEXIO_SPI_PCS_PIN                    6U
#define MASTER_FLEXIO_SPI_IRQ                 FLEXIO_IRQn
#define MASTER_FLEXIO_SPI_CLOCK_NAME          (kCLOCK_Flexio0)
#define MASTER_FLEXIO_SPI_CLOCK_SOURCE        (kCLOCK_IpSrcFircAsync)
#define MASTER_FLEXIO_SPI_CLOCK_FREQUENCY     (CLOCK_GetIpFreq(MASTER_FLEXIO_SPI_CLOCK_NAME))
#define FLEXIO_DMA_REQUEST_SOURCE_BASE        kDmaRequestMux0FlexIOChannel0
#define EXAMPLE_FLEXIO_SPI_DMAMUX_BASEADDR    DMAMUX
#define EXAMPLE_FLEXIO_SPI_DMA_LPSPI_BASEADDR DMA0
#define FLEXIO_SPI_TX_DMA_LPSPI_CHANNEL       0U
#define FLEXIO_SPI_RX_DMA_LPSPI_CHANNEL       1U
#define FLEXIO_TX_SHIFTER_INDEX               0U
#define FLEXIO_RX_SHIFTER_INDEX               1U
#define EXAMPLE_TX_DMA_SOURCE                 (FLEXIO_DMA_REQUEST_SOURCE_BASE + FLEXIO_TX_SHIFTER_INDEX)
#define EXAMPLE_RX_DMA_SOURCE                 (FLEXIO_DMA_REQUEST_SOURCE_BASE + FLEXIO_RX_SHIFTER_INDEX)

/*Slave related*/
#define SLAVE_LPSPI_BASEADDR         LPSPI0
#define SLAVE_LPSPI_IRQ_HANDLE       LPSPI0_DriverIRQHandler
#define SLAVE_LPSPI_IRQN             LPSPI0_IRQn
#define SLAVE_LPSPI_CLOCK_NAME       (kCLOCK_Lpspi0)
#define SLAVE_LPSPI_CLOCK_SOURCE     (kCLOCK_IpSrcFircAsync)
#define SLAVE_LPSPI_PCS_FOR_INIT     kLPSPI_Pcs2
#define SLAVE_LPSPI_PCS_FOR_TRANSFER kLPSPI_SlavePcs2
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
