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
#define EXAMPLE_SPI_MASTER            SPI8
#define EXAMPLE_SPI_MASTER_IRQ        LSPI_HS_IRQn
#define EXAMPLE_SPI_MASTER_CLK_SRC    kCLOCK_Hs_Lspi
#define EXAMPLE_SPI_MASTER_CLK_FREQ   CLOCK_GetHsLspiClkFreq()
#define EXAMPLE_SPI_SSEL              1
#define EXAMPLE_DMA                   DMA0
#define EXAMPLE_SPI_MASTER_RX_CHANNEL 2
#define EXAMPLE_SPI_MASTER_TX_CHANNEL 3
#define EXAMPLE_MASTER_SPI_SPOL       kSPI_SpolActiveAllLow
#define EXAMPLE_SPI_BAUDRATE          19000000U

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
