/*
 * Copyright 2017 NXP
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
#define EXAMPLE_SPI_SLAVE_BASEADDR SPI9
#define EXAMPLE_SPI_SLAVE_SPOL     kSPI_SpolActiveAllLow

#define EXAMPLE_SPI_SLAVE_DMA_BASEADDR DMA0
#define EXAMPLE_SPI_SLAVE_RX_CHANNEL   22
#define EXAMPLE_SPI_SLAVE_TX_CHANNEL   23
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
