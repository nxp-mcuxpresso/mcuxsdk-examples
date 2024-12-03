/*
 * Copyright  2019 NXP
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
#define EXAMPLE_SPI_SLAVE      SPI0
#define EXAMPLE_SPI_SLAVE_SSEL kSPI_Ssel0Assert

#define EXAMPLE_SPI_SLAVE_DMA_BASEADDR DMA0
#define EXAMPLE_SPI_SLAVE_TX_CHANNEL   11
#define EXAMPLE_SPI_SLAVE_RX_CHANNEL   10
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
