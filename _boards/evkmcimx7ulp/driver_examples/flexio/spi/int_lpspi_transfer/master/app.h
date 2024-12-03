/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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

#define MASTER_FLEXIO_SPI_BASEADDR        FLEXIO0
#define FLEXIO_SPI_SOUT_PIN               0U
#define FLEXIO_SPI_SIN_PIN                4U
#define FLEXIO_SPI_CLK_PIN                1U
#define FLEXIO_SPI_PCS_PIN                7U
#define MASTER_FLEXIO_SPI_IRQ             FLEXIO0_IRQn
#define MASTER_FLEXIO_SPI_CLOCK_FREQUENCY CLOCK_GetIpFreq(kCLOCK_Flexio0)

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
