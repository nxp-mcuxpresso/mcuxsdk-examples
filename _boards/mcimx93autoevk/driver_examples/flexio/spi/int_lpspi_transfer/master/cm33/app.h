/*
 * Copyright 2023 NXP
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

#define MASTER_FLEXIO_SPI_BASEADDR        FLEXIO1
#define FLEXIO_SPI_SOUT_PIN               2U
#define FLEXIO_SPI_SIN_PIN                3U
#define FLEXIO_SPI_CLK_PIN                4U
#define FLEXIO_SPI_PCS_PIN                5U
#define MASTER_FLEXIO_SPI_IRQ             FLEXIO1_IRQn
#define MASTER_FLEXIO_SPI_CLOCK_FREQUENCY CLOCK_GetIpFreq(kCLOCK_Root_Flexio1)

/*Slave related*/
#define SLAVE_LPSPI_BASEADDR         LPSPI3
#define SLAVE_LPSPI_IRQ_HANDLE       LPSPI3_IRQHandler
#define SLAVE_LPSPI_IRQN             LPSPI3_IRQn
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
