/*
 * Copyright 2019 NXP
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

#define MASTER_LPSPI_BASEADDR   (LPSPI2)
#define MASTER_LPSPI_IRQ_HANDLE (LPSPI2_DriverIRQHandler)
#define MASTER_LPSPI_IRQN       (LPSPI2_IRQn)

#define MASTER_LPSPI_PCS_FOR_INIT     (kLPSPI_Pcs0)
#define MASTER_LPSPI_PCS_FOR_TRANSFER (kLPSPI_MasterPcs0)

#define MASTER_LPSPI_CLOCK_NAME      (kCLOCK_Lpspi2)
#define MASTER_LPSPI_CLOCK_SOURCE    (kCLOCK_IpSrcFircAsync)
#define MASTER_LPSPI_CLOCK_FREQUENCY (CLOCK_GetIpFreq(MASTER_LPSPI_CLOCK_NAME))

/*Slave related*/
#define SLAVE_FLEXIO_SPI_BASEADDR (FLEXIO0)
#define FLEXIO_SPI_SOUT_PIN       2U
#define FLEXIO_SPI_SIN_PIN        3U
#define FLEXIO_SPI_CLK_PIN        1U
#define FLEXIO_SPI_PCS_PIN        0U

#define SLAVE_FLEXIO_SPI_IRQ          FLEXIO0_IRQn
#define SLAVE_FLEXIO_SPI_CLOCK_NAME   (kCLOCK_Flexio0)
#define SLAVE_FLEXIO_SPI_CLOCK_SOURCE (kCLOCK_IpSrcFircAsync)

/*INTMUX setting*/
#define EXAMPLE_LPSPI_INTMUX_CHANNLE (0U)

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif
