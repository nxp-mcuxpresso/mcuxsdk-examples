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
#define BOARD_FLEXIO_BASE      FLEXIO
#define FLEXIO_SPI_MOSI_PIN    6U
#define FLEXIO_SPI_MISO_PIN    7U
#define FLEXIO_SPI_SCK_PIN     5U
#define FLEXIO_SPI_PCS0_PIN    4U
#define FLEXIO_CLOCK_FREQUENCY 48000000U
#define BOARD_SPI_SLAVE_BASE   SPI1
#define SLAVE_SPI_IRQ          SPI1_IRQn
#define MASTER_SPI_IRQ         UART2_FLEXIO_IRQn
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
