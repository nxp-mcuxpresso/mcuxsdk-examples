/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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
#define EXAMPLE_SPI_MASTER          SPI5
#define EXAMPLE_SPI_MASTER_IRQ      FLEXCOMM5_IRQn
#define EXAMPLE_SPI_MASTER_CLK_SRC  kCLOCK_Flexcomm5
#define EXAMPLE_SPI_MASTER_CLK_FREQ CLOCK_GetFlexCommClkFreq(5)
#define EXAMPLE_SPI_SLAVE           SPI3
#define EXAMPLE_SPI_SLAVE_IRQ       FLEXCOMM3_IRQn
#define EXAMPLE_SPI_SSEL            2
#define EXAMPLE_DMA                 DMA0

#define EXAMPLE_SPI_MASTER_RX_CHANNEL 10
#define EXAMPLE_SPI_MASTER_TX_CHANNEL 11

#define EXAMPLE_SPI_SLAVE_RX_CHANNEL 6
#define EXAMPLE_SPI_SLAVE_TX_CHANNEL 7

#define EXAMPLE_MASTER_SPI_SPOL kSPI_SpolActiveAllLow
#define EXAMPLE_SLAVE_SPI_SPOL  kSPI_SpolActiveAllLow
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
