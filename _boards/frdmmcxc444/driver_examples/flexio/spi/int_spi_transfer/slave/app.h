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
#define BOARD_FLEXIO_BASE   FLEXIO
#define FLEXIO_SPI_MOSI_PIN 6U
#define FLEXIO_SPI_MISO_PIN 7U
#define FLEXIO_SPI_SCK_PIN  5U
#define FLEXIO_SPI_PCS0_PIN 4U

#define BOARD_SPI_MASTER_BASE SPI1
#define SPI_MASTER_CLK_SRC    SPI1_CLK_SRC
#define SPI_MASTER_CLK_FREQ   CLOCK_GetFreq(SPI1_CLK_SRC)
#define SLAVE_SPI_IRQ         UART2_FLEXIO_IRQn
#define MASTER_SPI_IRQ        SPI1_IRQn
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
