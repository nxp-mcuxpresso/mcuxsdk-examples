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
#define EXAMPLE_SPI_MASTER            SPI7
#define EXAMPLE_SPI_MASTER_IRQ        FLEXCOMM7_IRQn
#define EXAMPLE_SPI_MASTER_CLK_SRC    kCLOCK_Flexcomm7
#define EXAMPLE_SPI_MASTER_CLK_FREQ   CLOCK_GetFlexCommClkFreq(7U)
#define EXAMPLE_SPI_SSEL              1
#define EXAMPLE_DMA                   DMA0
#define EXAMPLE_SPI_MASTER_RX_CHANNEL 18
#define EXAMPLE_SPI_MASTER_TX_CHANNEL 19
#define EXAMPLE_MASTER_SPI_SPOL       kSPI_SpolActiveAllLow

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
