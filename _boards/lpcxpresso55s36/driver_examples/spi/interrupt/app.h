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
#define EXAMPLE_SPI_MASTER          SPI2
#define EXAMPLE_SPI_MASTER_IRQ      FLEXCOMM2_IRQn
#define EXAMPLE_SPI_MASTER_CLK_SRC  kCLOCK_Flexcomm2
#define EXAMPLE_SPI_MASTER_CLK_FREQ CLOCK_GetFlexCommClkFreq(2U)
#define EXAMPLE_SPI_SLAVE           SPI8
#define EXAMPLE_SPI_SLAVE_IRQ       FLEXCOMM8_IRQn
#define EXAMPLE_SPI_SSEL            0
#define SPI_MASTER_IRQHandler       FLEXCOMM2_IRQHandler
#define SPI_SLAVE_IRQHandler        FLEXCOMM8_IRQHandler

#define EXAMPLE_SPI_SPOL kSPI_SpolActiveAllLow
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
