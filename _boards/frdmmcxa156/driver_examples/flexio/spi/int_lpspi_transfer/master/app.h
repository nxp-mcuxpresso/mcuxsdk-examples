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
/*Master related*/
#define TRANSFER_SIZE     256U    /*! Transfer dataSize */
#define TRANSFER_BAUDRATE 500000U /*! Transfer baudrate - 500k */
/* Master related */
#define MASTER_FLEXIO_SPI_BASEADDR        (FLEXIO0)
#define FLEXIO_SPI_SIN_PIN                0U
#define FLEXIO_SPI_SOUT_PIN               1U
#define FLEXIO_SPI_CLK_PIN                2U
#define FLEXIO_SPI_PCS_PIN                3U
#define MASTER_FLEXIO_SPI_CLOCK_FREQUENCY CLOCK_GetFlexioClkFreq()
#define MASTER_FLEXIO_SPI_IRQ             FLEXIO_IRQn
/*Slave related*/
#define SLAVE_LPSPI_BASEADDR         (LPSPI0)
#define SLAVE_LPSPI_IRQN             (LPSPI0_IRQn)
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
