/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_SPI_MASTER              SPI0
#define EXAMPLE_SPI_MASTER_SOURCE_CLOCK kCLOCK_CoreSysClk
#define EXAMPLE_SPI_MASTER_CLK_FREQ     CLOCK_GetFreq(kCLOCK_CoreSysClk)
#define EXAMPLE_SPI_MASTER_IRQ          SPI0_SPI1_IRQn
#define SPI_MASTER_IRQHandler           SPI0_SPI1_IRQHandler
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
