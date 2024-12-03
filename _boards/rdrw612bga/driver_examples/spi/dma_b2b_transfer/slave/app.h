/*
 * Copyright  2018-2021 NXP
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
#define EXAMPLE_SPI_SLAVE            SPI0
#define EXAMPLE_SPI_SLAVE_IRQ        FLEXCOMM0_IRQn
#define EXAMPLE_SPI_SLAVE_RX_CHANNEL 0
#define EXAMPLE_SPI_SLAVE_TX_CHANNEL 1

#define EXAMPLE_DMA            DMA0
#define EXAMPLE_SPI_SSEL       0
#define EXAMPLE_SLAVE_SPI_SPOL kSPI_SpolActiveAllLow
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
