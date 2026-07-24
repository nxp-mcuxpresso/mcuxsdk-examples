/*
 * Copyright 2022, 2026 NXP
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

#define MASTER_FLEXIO_SPI_BASEADDR        (HSP__FLEXIO_1)
#define MASTER_FLEXIO_SPI_IRQ             HSP_FLEXIO1_IRQn
#define MASTER_FLEXIO_SPI_CLOCK_FREQUENCY CLOCK_GetRootClockFreq(kCLOCK_Root_MAIN_flexio1_fclk)

#if defined(RT2660_PRESILICON_DEVELOPMENT) && (RT2660_PRESILICON_DEVELOPMENT == 1)
#define FLEXIO_SPI_PCS_PIN  1U
#define FLEXIO_SPI_SOUT_PIN 3U
#define FLEXIO_SPI_SIN_PIN  2U
#define FLEXIO_SPI_CLK_PIN  0U
#else
#define FLEXIO_SPI_PCS_PIN  0U
#define FLEXIO_SPI_SIN_PIN  1U
#define FLEXIO_SPI_SOUT_PIN 2U
#define FLEXIO_SPI_CLK_PIN  3U
#endif

/*Slave related*/
#define SLAVE_LPSPI_BASEADDR (HSP__LPSPI_1)
#define SLAVE_LPSPI_IRQN     (HSP_LPSPI1_IRQn)

#define SLAVE_LPSPI_PCS_FOR_INIT     (kLPSPI_Pcs0)
#define SLAVE_LPSPI_PCS_FOR_TRANSFER (kLPSPI_SlavePcs0)

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif
