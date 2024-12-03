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
#define TRANSFER_SIZE     256U    /*! Transfer dataSize */
#define TRANSFER_BAUDRATE 500000U /*! Transfer baudrate - 500k */
/* Slave related */
#define SLAVE_FLEXIO_SPI_BASEADDR (FLEXIO0)
#define FLEXIO_SPI_SIN_PIN        8U
#define FLEXIO_SPI_SOUT_PIN       20U
#define FLEXIO_SPI_CLK_PIN        21U
#define FLEXIO_SPI_PCS_PIN        7U
#define SLAVE_FLEXIO_SPI_IRQ      FLEXIO0_IRQn
/* Master related */
#define MASTER_LPSPI_BASEADDR         (LPSPI0)
#define MASTER_LPSPI_IRQN             (LPSPI0_IRQn)
#define MASTER_LPSPI_PCS_FOR_INIT     (kLPSPI_Pcs0)
#define MASTER_LPSPI_PCS_FOR_TRANSFER (kLPSPI_MasterPcs0)
#define MASTER_LPSPI_CLOCK_FREQUENCY  CLOCK_GetIpFreq(kCLOCK_Lpspi0)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
