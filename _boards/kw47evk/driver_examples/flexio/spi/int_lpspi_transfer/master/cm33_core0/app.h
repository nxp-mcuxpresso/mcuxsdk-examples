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
/* Master related */
#define MASTER_FLEXIO_SPI_BASEADDR        (FLEXIO0)
#define FLEXIO_SPI_SIN_PIN                21U
#define FLEXIO_SPI_SOUT_PIN               7U
#define FLEXIO_SPI_CLK_PIN                8U
#define FLEXIO_SPI_PCS_PIN                20U
#define MASTER_FLEXIO_SPI_CLOCK_FREQUENCY CLOCK_GetIpFreq(kCLOCK_Flexio0)
#define MASTER_FLEXIO_SPI_IRQ             FLEXIO0_IRQn
/*Slave related*/
#define SLAVE_LPSPI_BASEADDR         (LPSPI1)
#define SLAVE_LPSPI_IRQN             (LPSPI1_IRQn)
#define SLAVE_LPSPI_PCS_FOR_INIT     (kLPSPI_Pcs0)
#define SLAVE_LPSPI_PCS_FOR_TRANSFER (kLPSPI_SlavePcs0)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
