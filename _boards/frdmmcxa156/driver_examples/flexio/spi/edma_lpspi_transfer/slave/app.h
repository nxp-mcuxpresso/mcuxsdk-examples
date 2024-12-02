/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define TRANSFER_SIZE     256U    /*! Transfer dataSize */
#define TRANSFER_BAUDRATE 500000U /*! Transfer baudrate - 500k */
/* Slave related */
#define SLAVE_FLEXIO_SPI_BASEADDR (FLEXIO0)
#define FLEXIO_SPI_SIN_PIN        0U
#define FLEXIO_SPI_SOUT_PIN       1U
#define FLEXIO_SPI_CLK_PIN        2U
#define FLEXIO_SPI_PCS_PIN        3U
#define SLAVE_FLEXIO_SPI_IRQ      FLEXIO_IRQn
/* Master related */
#define MASTER_LPSPI_BASEADDR         (LPSPI0)
#define MASTER_LPSPI_IRQN             (LPSPI0_IRQn)
#define MASTER_LPSPI_PCS_FOR_INIT     (kLPSPI_Pcs0)
#define MASTER_LPSPI_PCS_FOR_TRANSFER (kLPSPI_MasterPcs0)
#define MASTER_LPSPI_CLOCK_FREQUENCY  CLOCK_GetLpspiClkFreq(0U)

#define EXAMPLE_FLEXIO_SPI_DMA_LPSPI_BASEADDR DMA0
#define FLEXIO_SPI_TX_DMA_LPSPI_CHANNEL       (0U)
#define FLEXIO_SPI_RX_DMA_LPSPI_CHANNEL       (1U)
#define FLEXIO_TX_SHIFTER_INDEX               0U
#define FLEXIO_RX_SHIFTER_INDEX               2U
#define EXAMPLE_TX_DMA_SOURCE                 kDma0RequestMuxFlexIO0ShiftRegister0Request
#define EXAMPLE_RX_DMA_SOURCE                 kDma0RequestMuxFlexIO0ShiftRegister2Request
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
