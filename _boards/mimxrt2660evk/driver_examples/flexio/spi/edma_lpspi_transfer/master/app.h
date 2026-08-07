/*
 * Copyright 2018, 2026 NXP
 * All rights reserved.
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
#define TRANSFER_BAUDRATE 150000U /*! Transfer baudrate - 150k */

#define MASTER_FLEXIO_SPI_BASEADDR        (HSP__FLEXIO_1)
#define MASTER_FLEXIO_SPI_IRQ             HSP_FLEXIO1_IRQn
#define MASTER_FLEXIO_SPI_CLOCK_FREQUENCY CLOCK_GetRootClockFreq(kCLOCK_Root_MAIN_flexio1_fclk)
#define EXAMPLE_TX_DMA_SOURCE             kDmaRequestMux1HspFlexIO1Request0Request8
#define EXAMPLE_RX_DMA_SOURCE             kDmaRequestMux1HspFlexIO1Request2Request10

#define FLEXIO_SPI_PCS_PIN  0U
#define FLEXIO_SPI_SIN_PIN  1U
#define FLEXIO_SPI_SOUT_PIN 2U
#define FLEXIO_SPI_CLK_PIN  3U

#define EXAMPLE_FLEXIO_SPI_DMA_LPSPI_BASEADDR MAIN__EDMA3
#define FLEXIO_SPI_TX_DMA_LPSPI_CHANNEL       0U
#define FLEXIO_SPI_RX_DMA_LPSPI_CHANNEL       1U
#define FLEXIO_TX_SHIFTER_INDEX               0U
#define FLEXIO_RX_SHIFTER_INDEX               2U

/*Slave related*/
#define SLAVE_LPSPI_BASEADDR   (HSP__LPSPI_1)
#define SLAVE_LPSPI_IRQ_HANDLE (LPSPI1_DriverIRQHandler)
#define SLAVE_LPSPI_IRQN       (HSP_LPSPI1_IRQn)

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
