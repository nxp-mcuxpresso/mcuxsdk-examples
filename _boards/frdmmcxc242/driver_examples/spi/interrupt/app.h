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
#define EXAMPLE_SPI_MASTER              SPI0
#define EXAMPLE_SPI_SLAVE               SPI1
#define EXAMPLE_SPI_MASTER_SOURCE_CLOCK kCLOCK_BusClk
#define EXAMPLE_SPI_MASTER_CLK_FREQ     CLOCK_GetFreq(kCLOCK_BusClk)
#define EXAMPLE_SPI_SLAVE_SOURCE_CLOCK  kCLOCK_SysCoreClk
#define EXAMPLE_SPI_MASTER_IRQ          SPI0_IRQn
#define EXAMPLE_SPI_SLAVE_IRQ           SPI1_IRQn
#define SPI_MASTER_IRQHandler           SPI0_IRQHandler
#define SPI_SLAVE_IRQHandler            SPI1_IRQHandler
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
