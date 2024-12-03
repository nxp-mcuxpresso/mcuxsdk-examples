/*
 * Copyright 2024 NXP
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
#define TRANSFER_BAUDRATE 500000U /*! Transfer baudrate - 500k */

#define MASTER_LPSPI_BASEADDR         LPSPI1
#define MASTER_LPSPI_IRQ_HANDLE       LPSPI1_IRQHandler
#define MASTER_LPSPI_IRQN             LPSPI1_IRQn
#define MASTER_LPSPI_PCS_FOR_INIT     kLPSPI_Pcs0
#define MASTER_LPSPI_PCS_FOR_TRANSFER kLPSPI_MasterPcs0
#define MASTER_LPSPI_CLOCK_FREQUENCY  HAL_ClockGetIpFreq(hal_clock_lpspi1)

/*Slave related*/
#define SLAVE_FLEXIO_SPI_BASEADDR FLEXIO1
#define FLEXIO_SPI_SOUT_PIN       2U
#define FLEXIO_SPI_SIN_PIN        3U
#define FLEXIO_SPI_CLK_PIN        4U
#define FLEXIO_SPI_PCS_PIN        5U
#define SLAVE_FLEXIO_SPI_IRQ      FLEXIO1_IRQn
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
