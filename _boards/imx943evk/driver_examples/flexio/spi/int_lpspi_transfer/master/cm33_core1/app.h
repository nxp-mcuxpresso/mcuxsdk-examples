/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

#include "board.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
/*Master related*/
#define TRANSFER_SIZE     256U    /*! Transfer dataSize */
#define TRANSFER_BAUDRATE 500000U /*! Transfer baudrate - 500k */

#define MASTER_FLEXIO_SPI_BASEADDR        FLEXIO1
#if BOARD_IMX943_TYPE == BOARD_IMX943_EMULATOR
#warning "For Emulator"
#define FLEXIO_SPI_SOUT_PIN               10U
#define FLEXIO_SPI_SIN_PIN                9U
#define FLEXIO_SPI_CLK_PIN                11U
#define FLEXIO_SPI_PCS_PIN                8U

#define DEMO_LPSPI_CLOCK kCLOCK_Lpspi5
#define SLAVE_LPSPI_BASEADDR         LPSPI5
#define SLAVE_LPSPI_IRQ_HANDLE       LPSPI5_IRQHandler
#define SLAVE_LPSPI_IRQN             LPSPI5_IRQn
#else
#define FLEXIO_SPI_SOUT_PIN               0U
#define FLEXIO_SPI_SIN_PIN                1U
#define FLEXIO_SPI_CLK_PIN                2U
#define FLEXIO_SPI_PCS_PIN                3U

#define DEMO_LPSPI_CLOCK kCLOCK_Lpspi3
#define SLAVE_LPSPI_BASEADDR         LPSPI3
#define SLAVE_LPSPI_IRQ_HANDLE       LPSPI3_IRQHandler
#define SLAVE_LPSPI_IRQN             LPSPI3_IRQn
#endif

#define MASTER_FLEXIO_SPI_IRQ             FLEXIO1_IRQn
#define DEMO_FLEXIO_CLOCK kCLOCK_Flexio1
#define MASTER_FLEXIO_SPI_CLOCK_FREQUENCY CLOCK_GetRate(DEMO_FLEXIO_CLOCK)

/*Slave related*/
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
