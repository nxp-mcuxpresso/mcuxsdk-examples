/*
 * Copyright 2023 NXP
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
#define FLEXIO_SPI_SIN_PIN        16U
#define FLEXIO_SPI_SOUT_PIN       17U
#define FLEXIO_SPI_CLK_PIN        18U
#define FLEXIO_SPI_PCS_PIN        19U
#define SLAVE_FLEXIO_SPI_IRQ      FLEXIO_IRQn
/* Master related */
#define MASTER_LPSPI_BASEADDR         (LPSPI3)
#define MASTER_LPSPI_IRQN             (LP_FLEXCOMM3_IRQn)
#define MASTER_LPSPI_PCS_FOR_INIT     (kLPSPI_Pcs0)
#define MASTER_LPSPI_PCS_FOR_TRANSFER (kLPSPI_MasterPcs0)
#define MASTER_LPSPI_CLOCK_FREQUENCY  CLOCK_GetLPFlexCommClkFreq(3U)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
