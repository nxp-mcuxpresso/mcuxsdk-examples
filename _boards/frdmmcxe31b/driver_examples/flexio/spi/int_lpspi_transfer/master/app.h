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

#define MASTER_FLEXIO_SPI_BASEADDR FLEXIO
#define FLEXIO_SPI_SOUT_PIN        0U
#define FLEXIO_SPI_SIN_PIN         1U
#define FLEXIO_SPI_CLK_PIN         2U
#define FLEXIO_SPI_PCS_PIN         3U

#define MASTER_FLEXIO_SPI_IRQ             FLEXIO_IRQn
#define MASTER_FLEXIO_SPI_CLOCK_FREQUENCY CLOCK_GetFreq(kCLOCK_FlexioClk)

/*Slave related*/
#define SLAVE_LPSPI_BASEADDR         LPSPI_0
#define SLAVE_LPSPI_IRQ_HANDLE       LPSPI0_DriverIRQHandler
#define SLAVE_LPSPI_IRQN             LPSPI0_IRQn
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
