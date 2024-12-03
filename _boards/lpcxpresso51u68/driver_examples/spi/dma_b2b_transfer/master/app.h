/*
 * Copyright  2017 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_SPI_MASTER            SPI3
#define EXAMPLE_SPI_MASTER_IRQ        FLEXCOMM3_IRQn
#define EXAMPLE_SPI_MASTER_CLK_SRC    kCLOCK_Flexcomm3
#define EXAMPLE_SPI_MASTER_CLK_FREQ   CLOCK_GetFlexCommClkFreq(3)
#define EXAMPLE_SPI_SSEL              2
#define EXAMPLE_DMA                   DMA0
#define EXAMPLE_SPI_MASTER_RX_CHANNEL 6
#define EXAMPLE_SPI_MASTER_TX_CHANNEL 7
#define EXAMPLE_MASTER_SPI_SPOL       kSPI_SpolActiveAllLow

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
